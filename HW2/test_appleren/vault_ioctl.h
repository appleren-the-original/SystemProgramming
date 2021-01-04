#ifndef __VAULT_H
#define __VAULT_H


#include <linux/ioctl.h> // _IO, _IOW

#define VAULT_IOC_MAGIC 'k'
#define VAULT_IOC_SETKEY _IOW(VAULT_IOC_MAGIC, 0, char*)
#define VAULT_IOC_CLEAR _IO(VAULT_IOC_MAGIC, 1)
#define VAULT_IOC_MAXNR 1


#endif
