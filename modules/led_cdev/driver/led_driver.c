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
#include <linux/string.h>
#include <linux/kernel.h>

#define LED_NAME "led_driver"
#define LED_STATUS_ON 0
#define LED_STATUS_OFF 1

struct led_device{
    char* name;
    char status;
    struct cdev cdev;
};

static struct led_device led_device;
static dev_t led_no;
static struct class* led_class;
static struct class_device* led_class_device;

#define GPFCON 0x56000050
#define IOMAP_SIZE 0x10
static volatile unsigned long* gpfconf = NULL;
static volatile unsigned long* gpfdata= NULL;

static void led_on(void){
    printk(KERN_INFO"led_on...\n");
    *gpfdata &= ~(1<<4);
    *gpfdata &= ~(1<<5);
    *gpfdata &= ~(1<<6);
}
static void led_off(void){
    printk(KERN_INFO"led_off...\n");
    *gpfdata |= (1<<4);
    *gpfdata |= (1<<5);
    *gpfdata |= (1<<6);
}


static int led_open(struct inode *inode, struct file *filp){
    //dump_stack();
    gpfconf = ioremap(GPFCON, IOMAP_SIZE);
    if(gpfconf == NULL){
        printk(KERN_ERR"gpfconf ioremap failed.\n");
        return -EINVAL;
    }
    gpfdata = gpfconf + 1;

    /*set GPF 4 5 6 7 output*/
    *gpfconf |= (1<<8);
    *gpfconf |= (1<<10);
    *gpfconf |= (1<<12);

    *gpfconf &= ~(1<<9);
    *gpfconf &= ~(1<<11);
    *gpfconf &= ~(1<<13);

    return 0;
}

 ssize_t led_read(struct file *filp, char __user *buff, size_t count, loff_t *off){
     int ret = -1;
     char status[10] ={};
     if(led_device.status == LED_STATUS_ON){
         memcpy(status,"on",10);
     }else if(led_device.status == LED_STATUS_OFF){
         memcpy(status,"off",10);
     }
     ret = copy_to_user(buff,status,sizeof(status));
    if(ret<0){
        printk(KERN_ERR"%s:copy to user failed.\n", LED_NAME);
        return -EIO;
    } 
     return ret;
 }
 ssize_t led_write(struct file *filp, const char __user *buff, size_t count, loff_t *off){
     int ret;

    ret = copy_from_user(&led_device.status, buff, 1);
    if(ret<0){
        printk(KERN_ERR"%s:copy from user failed.\n", LED_NAME);
        return -EIO;
    }
    if(led_device.status == LED_STATUS_ON){
        led_on();
    }else if(led_device.status == LED_STATUS_OFF){
        led_off();
    }else{
        printk(KERN_ERR"invalid led status.\n");
        return -EIO;
    }

    return ret;
 }

int led_close(struct inode *inode, struct file *filp){
    printk(KERN_INFO"led release\n");
    return 0;
}

static struct file_operations led_ops={
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_close,
};



static int __init led_init(void){
    int ret;

    led_device.name = LED_NAME;
    led_device.status = 0;

    ret = alloc_chrdev_region(&led_no, 0, 1, led_device.name);
    if(ret<0){
        printk(KERN_ERR"alloc %s region failed.\n", led_device.name);
        return -EIO;
    }

    cdev_init(&led_device.cdev, &led_ops);
    ret = cdev_add(&led_device.cdev, led_no, 1);
    if(ret<0){
        printk(KERN_ERR"cdev add %s failed.\n", led_device.name);
        return -EIO;
    }

    led_class = class_create(THIS_MODULE, "led");
	if (IS_ERR(led_class)) {
		return PTR_ERR(led_class);
	}

	led_class_device = class_device_create(led_class, NULL,led_no,NULL, led_device.name);
	if (IS_ERR(led_class_device)) {
		return PTR_ERR(led_class_device);
	}

    return ret;
}
module_init(led_init);

static void __exit led_exit(void){
    iounmap(gpfconf);
    unregister_chrdev_region(led_no,1);
    class_device_unregister(led_class_device);
    class_destroy(led_class);
    cdev_del(&led_device.cdev);
}
module_exit(led_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccdroid");
MODULE_DESCRIPTION(" cdev's led driver");
