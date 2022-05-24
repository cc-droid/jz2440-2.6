/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm-generic/errno-base.h>
#include <linux/device.h>


#define NAME "led_driver"

static int MAJOR;
volatile unsigned long* gpfcon = NULL;
volatile unsigned long* gpfdata = NULL;
static struct class* led_class = NULL;
static struct class_device* led_device = NULL;
static dev_t led_no;


static ssize_t led_read(struct file *filp, char __user *buf, size_t count, loff_t *offset){
    printk(KERN_INFO"led_read...\n");
    return 0;
}

static int led_open (struct inode *inode, struct file *filp){
    printk(KERN_INFO"led_open...\n");
    return 0;
}

static void led_on(void){
    *gpfdata &= ~(1<<4);
    *gpfdata &= ~(1<<5);
    *gpfdata &= ~(1<<6);
}
static void led_off(void){
    *gpfdata |= (1<<4);
    *gpfdata |= (1<<5);
    *gpfdata |= (1<<6);
}
static ssize_t led_write (struct file *filp, const char __user *buf, size_t count, loff_t *offset){
    char status;
    int ret;
    printk(KERN_INFO"led_write...\n");
    ret = copy_from_user(&status, buf, count);
    if(ret <0){
        printk(KERN_ERR"copy_from_user failed.\n");
        return ret;
    }
    printk(KERN_INFO"status:%d\n", status);
    if(status == 1){
        led_on();
    }else if(status == 0){
        led_off();
    }else{
        printk(KERN_INFO"invalid led status.\n");
    }
    return 0;
}
static struct file_operations led_ops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write

};


static int __init led_init(void){
    int ret = -1;
    printk(KERN_INFO"led_init...\n");
    /* set output*/
    gpfcon = ioremap((unsigned long*)0x56000050,16);
    gpfdata = gpfcon + 1;
    *gpfcon |=(1<<8);
    *gpfcon &= ~(1<<9);
    *gpfcon |=(1<<10);
    *gpfcon &= ~(1<<11);
    *gpfcon |=(1<<12);
    *gpfcon &= ~(1<<13);


    ret = register_chrdev(0, NAME, &led_ops);
    if(ret < 0){
        printk(KERN_ERR"register led_driver failed.\n");
        goto register_failed;
    }

    MAJOR = ret;
    led_class = class_create(THIS_MODULE, "led_class");
    if(led_class == NULL){
        printk(KERN_ERR"led class created failed\n");
        return -EFAULT;
    }
    led_no = MKDEV(MAJOR,0);
    led_device = class_device_create(led_class, NULL, led_no, NULL, "led_driver");
    if(led_device == NULL){
        printk(KERN_ERR"led device created failed\n");
        return -EFAULT;
    }


register_failed:
    unregister_chrdev(0,NAME);
    return ret;
}
module_init(led_init);

static void __exit led_exit(void){
    printk(KERN_INFO"led_exit...\n");
    unregister_chrdev(MAJOR,NAME);
    iounmap(gpfcon);
    class_device_unregister(led_device);
    class_destroy(led_class);

}
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccdroid");
MODULE_DESCRIPTION("a simple led driver.");
