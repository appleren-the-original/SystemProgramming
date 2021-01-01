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


#include <linux/version.h>  /* in order to check kernel version */


//  check if linux/uaccess.h is required for copy_*_user
//instead of asm/uaccess
//required after linux kernel 4.1+ ?
#ifndef __ASM_ASM_UACCESS_H
    #include <linux/uaccess.h>
#endif



#define VAULT_MAJOR 0
#define VAULT_NR_DEVS 4



int vault_major = VAULT_MAJOR;
int vault_minor = 0;
int vault_nr_devs = VAULT_NR_DEVS;

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


int vault_open(struct inode *inode, struct file *filp) {
    /*
		...
    */
    
    return 0;
}

int vault_release(struct inode *inode, struct file *filp) {
    return 0;
}

ssize_t vault_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    /*
		...
    */
}


ssize_t vault_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
    /*
		...
    */
}


long vault_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	/*
	 ...
	*/
	switch(cmd) {
		case 0:
			//...
			break;
		case 1:
			//...
			break;
	  
		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}

}


struct file_operations vault_fops = {
    .owner =    		THIS_MODULE,
    .read =     		vault_read,
    .write =    		vault_write,
    .unlocked_ioctl =  	vault_ioctl,
    .open =     		vault_open,
    .release =  		vault_release,
};


void vault_cleanup_module(void) {
    dev_t devno = MKDEV(vault_major, vault_minor);
	
	/*
	 ...
	*/
}

int vault_init_module(void) {
    int result, i;
    int err;
    dev_t devno = 0;
    struct vault_dev* dev;
    
    if (scull_major) {
        devno = MKDEV(vault_major, vault_minor);
        result = register_chrdev_region(devno, vault_nr_devs, "vault");
    } else {
        result = alloc_chrdev_region(&devno, vault_minor, vault_nr_devs, "scull");
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



