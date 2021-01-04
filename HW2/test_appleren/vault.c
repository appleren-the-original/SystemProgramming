#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/switch_to.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include "encryption.h"

#include <linux/version.h>  /* in order to check kernel version */


//  check if linux/uaccess.h is required for copy_*_user
//instead of asm/uaccess
//required after linux kernel 4.1+ ?
#ifndef __ASM_ASM_UACCESS_H
    #include <linux/uaccess.h>
#endif


#include "vault_ioctl.h"



#define VAULT_MAJOR 0
#define VAULT_NR_DEVS 4



int vault_major = VAULT_MAJOR;
int vault_minor = 0;
int vault_nr_devs = VAULT_NR_DEVS;

char* key = "adcb";
int karray[4] = {1, 4, 3, 2};
int *key_array = karray;


module_param(vault_major, int, S_IRUGO);
module_param(vault_minor, int, S_IRUGO);
module_param(vault_nr_devs, int, S_IRUGO);



struct vault_dev {
    char* data;
    unsigned long size;
    struct semaphore sem;
    struct cdev cdev;
};

struct vault_dev* vault_devices;



int vault_trim(struct vault_dev *dev) {
    if(dev->data){
        kfree(dev->data);
    }
    dev->data = NULL;
    dev->size = 0;
    return 0;
}

int vault_open(struct inode *inode, struct file *filp) {
    /*
		...
    */
    struct vault_dev *dev;
    dev = container_of(inode->i_cdev, struct vault_dev, cdev);
    filp->private_data = dev;
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
        if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;
        vault_trim(dev);
        up(&dev->sem);
    }
    return 0;
}

int vault_release(struct inode *inode, struct file *filp) {
    return 0;
}

ssize_t vault_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
   
	char* res;
	struct vault_dev *dev = filp->private_data;
	ssize_t retval = 0;
   
	if(down_interruptible(&dev->sem))
        return -ERESTARTSYS;
    
    if (*f_pos >= dev->size)
        goto out;
    
    if (*f_pos + count > dev->size)
        count = dev->size - *f_pos;
    
    if (dev->data == NULL) 
        goto out;
        
    dec(key_array, key, dev->data, &res);
    if(copy_to_user(buf, res, count)){
	//if(copy_to_user(buf, res, count)){
        retval = -EFAULT;
        goto out;
    }
    *f_pos += count;
    retval = count;
out:
    //kfree(res);
    up(&dev->sem);
    return retval;

}


ssize_t vault_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
    
    char* write_;
    //char* res;
    struct vault_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;
    int pad;
    
    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;
    
    write_ = kmalloc(count, GFP_KERNEL);
    //dev->data = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(write_, buf, count)){
        retval = -EFAULT;
        goto out;
    }
    //pad = enc(key_array, key, write_, &dev->data);
    //encrypt(key_array, len(key), write_, 1, &dev->data);
    //printk(KERN_ALERT  "write input: %s\n device content: %s\n", buf, dev->data);
    pad = enc(key_array, key, write_, &dev->data);
    //*f_pos = *f_pos + pad + count;
    //retval = count + pad;
    *f_pos = *f_pos + count + pad;
    retval = count;
    
    // update size of the device
    if (dev->size < *f_pos)
        dev->size = *f_pos;
    
out:
    //kfree(write_);
    up(&dev->sem);
    return retval;
}


long vault_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	
	int err = 0;
	int retval = 0;
	char* temp;
	int count;
	//int i;
	struct vault_dev *dev = filp->private_data;

	// check if the ioctl type & number is correct; if not, declare inappropriate ioctl
	if (_IOC_TYPE(cmd) != VAULT_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > VAULT_IOC_MAXNR) return -ENOTTY;

	// check
	if (_IOC_DIR(cmd) & _IOC_READ)
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
            err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
        #else
            err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
        #endif
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
            err =  !access_ok((void __user *)arg, _IOC_SIZE(cmd));
        #else
            err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
        #endif
	if (err) return -EFAULT;
	
	switch(cmd) {
		case VAULT_IOC_SETKEY:
			printk("ARG VALUE: %ld\n", arg);
			temp = (char *) arg;
			printk("TEMP POINTER VALUE: %p\n", temp);
			printk("TEMP POINTER POINTS TO: %s\n", temp); // prints correct value
			count = len(temp);
			//count = 10;
			printk("COUNT VALUE: %d\n", count); // prints correct value
			kfree(key);
			key = NULL;
			key = kmalloc(count, GFP_KERNEL);
			//for(i=0; i<count; i++, temp++)
				//get_user(key[i], temp);
			if (copy_from_user(key, temp, count)){
				retval = -EFAULT;
				goto out;
			}
			//set_keyArray(&key_array, key);
			printk(KERN_INFO "key is set to: %s", key);
			break;
			
		case VAULT_IOC_CLEAR:
			retval = vault_trim(dev);
			break;
	  
		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}

out:
	return retval;

}

loff_t vault_llseek(struct file *filp, loff_t off, int whence)
{
    struct vault_dev *dev = filp->private_data;
    loff_t newpos;

    switch(whence) {
        case 0: /* SEEK_SET */
            newpos = off;
            break;

        case 1: /* SEEK_CUR */
            newpos = filp->f_pos + off;
            break;

        case 2: /* SEEK_END */
            newpos = dev->size + off;
            break;

        default: /* can't happen */
            return -EINVAL;
    }
    if (newpos < 0)
        return -EINVAL;
    filp->f_pos = newpos;
    return newpos;
}


struct file_operations vault_fops = {
    .owner =    		THIS_MODULE,
    .llseek = 			vault_llseek,
    .read =     		vault_read,
    .write =    		vault_write,
    .unlocked_ioctl =  	vault_ioctl,
    .open =     		vault_open,
    .release =  		vault_release,
};


void vault_cleanup_module(void) {
    //dev_t devno = MKDEV(vault_major, vault_minor);
    int i;
    dev_t devno = MKDEV(vault_major, vault_minor);
    if(vault_devices) {
        for (i = 0; i < vault_nr_devs; i++) {
            vault_trim(vault_devices+i);
            cdev_del(&vault_devices[i].cdev);
        }
        kfree(vault_devices);
    }
    unregister_chrdev_region(devno, vault_nr_devs);
}

int vault_init_module(void) {
    int result, i;
    int err;
    struct vault_dev* dev;
    
    dev_t devno = 0;
    // set_keyArray(&key_array, key);
    
    if (vault_major) {
        devno = MKDEV(vault_major, vault_minor);
        result = register_chrdev_region(devno, vault_nr_devs, "vault");
    } else {
        result = alloc_chrdev_region(&devno, vault_minor, vault_nr_devs, "vault");
        vault_major = MAJOR(devno);
    }
    if (result < 0) {
        printk(KERN_WARNING "vault: can't get major %d\n", vault_major);
        return result;
    }
    
    vault_devices = kmalloc(vault_nr_devs * sizeof(struct vault_dev), GFP_KERNEL);
    if (!vault_devices) {
        result = -ENOMEM;
        goto fail;
    }
    // initialize content with 0 / NULL.
    memset(vault_devices, 0, vault_nr_devs * sizeof(struct vault_dev));
    
    // Initialize each device
    for(i=0; i<vault_nr_devs; i++){
		dev = &vault_devices[i];
        sema_init(&dev->sem, 1); // initialize semaphore value as 1
        devno = MKDEV(vault_major, vault_minor + i); // minor values are sequential 
        cdev_init(&dev->cdev, &vault_fops);
        dev->cdev.owner = THIS_MODULE;
        dev->cdev.ops = &vault_fops;
        err = cdev_add(&dev->cdev, devno, 1);
        if (err)
            printk(KERN_NOTICE "Error %d adding vault%d", err, i);
	}
    return 0; // success

  fail:
    vault_cleanup_module();
    return result;
}


module_init(vault_init_module);
module_exit(vault_cleanup_module);



