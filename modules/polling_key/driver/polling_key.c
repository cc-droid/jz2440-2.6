/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#define KEY_NAME "polling_key"
#define DEV_CNT  1

#define GPFCON 0x56000050
#define GPFDAT (GPFCON+4)

static volatile unsigned long* gpfcon;
static volatile unsigned long* gpfdat;
static int polling_key_open (struct inode *inode, struct file *filp){

   gpfcon = ioremap(GPFCON,16);
   if(!gpfcon){
       printk(KERN_ERR"ioremap gpfcon failed.\n");
       return -EIO;
   }
   gpfdat = gpfcon + 1;

   /* gpf0 input)*/
   *gpfcon &= ~(0x3);
    printk(KERN_INFO"open polling key.\n");
   return 0;
}

static ssize_t polling_key_read (struct file *filp, char __user *buff, size_t count, loff_t *offset){
    int ret;
    char status;
    //printk(KERN_INFO"read polling key.\n");
    status = (*gpfdat)&0x1;
    //printk(KERN_INFO"status:%d.\n", status);
	//ret = put_user(status, (unsigned long __user *)buff);
    ret = copy_to_user(buff, &status, sizeof(status));
    if(ret<0){
        printk(KERN_ERR"put to user failed/\n");
        return -EIO;
    }

    return ret;

}

static dev_t key_no;
static struct cdev key_dev;
static struct file_operations key_ops = {
    .owner = THIS_MODULE,
    .open = polling_key_open,
    .read = polling_key_read,
};
static struct class *key_class;
static struct class_device *key_class_device;

static int __init polling_key_init(void){
    int ret;
    ret = alloc_chrdev_region(&key_no, 0, DEV_CNT, KEY_NAME);
    if(ret<0){
        printk(KERN_ERR"alloc chrdev region:%s failed.\n", KEY_NAME);
        goto alloc_failed;
    }

    cdev_init(&key_dev, &key_ops);
    ret = cdev_add(&key_dev, key_no, DEV_CNT);
    if(ret<0){
        printk(KERN_ERR"add cdev :%s failed.\n", KEY_NAME);
        return -EIO;
    }

    key_class = class_create(THIS_MODULE, KEY_NAME);
    if(!key_class){
        printk(KERN_ERR"create %s class failed.\n", KEY_NAME);
        goto cls_create_failed;
    }
    key_class_device = class_device_create(key_class, NULL, key_no, NULL, KEY_NAME); 
    if(!key_class_device){
        printk(KERN_ERR"class device register :%s failed.\n", KEY_NAME);
        goto cls_device_create_failed;
    }
    return ret;
cls_device_create_failed:
    class_device_unregister(key_class_device);
cls_create_failed:
    class_destroy(key_class);
alloc_failed:
    unregister_chrdev_region(key_no,DEV_CNT);

    return ret;
}
module_init(polling_key_init);

static void __exit polling_key_exit(void){
    
    iounmap(gpfcon);
    class_device_unregister(key_class_device);
    class_destroy(key_class);
    cdev_del(&key_dev);
    unregister_chrdev_region(key_no,DEV_CNT);
}
module_exit(polling_key_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccdroid");
MODULE_DESCRIPTION("a simple polling key driver.");

