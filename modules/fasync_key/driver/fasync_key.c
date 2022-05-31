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
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/gpio.h>

#define KEY_NAME "fasync_key"
#define DEV_CNT  1

#define GPFCON 0x56000050
#define GPFDAT (GPFCON+4)

static volatile unsigned long* gpfcon = NULL;
//static volatile unsigned long* gpfdat = NULL;

typedef struct _pin_desc{
    unsigned int pin;
    unsigned int key_value;
}pin_desc;

static pin_desc pin_descs[4] ={
    {S3C2410_GPF0,1},
};
static DECLARE_WAIT_QUEUE_HEAD(key_waitq);
static unsigned int key_pressed;
static struct fasync_struct* fasync_key;

static irqreturn_t key_irq(int irq, void *dev_id){
    pin_desc *key_desc = (pin_desc*)dev_id;

    key_desc->key_value = s3c2410_gpio_getpin(key_desc->pin);
    if(key_desc->key_value == 0){
        key_pressed = 1;
        wake_up_interruptible(&key_waitq);
    }

    kill_fasync (&fasync_key, SIGIO, POLL_IN);

	return IRQ_HANDLED;
}

static int fasync_key_fasync (int fd, struct file *filp, int on)
{
    return fasync_helper (fd, filp, on, &fasync_key);
}

static unsigned int fasync_key_poll (struct file *filp, struct poll_table_struct *table){
    unsigned int mask = 0;
    poll_wait(filp, &key_waitq, table);

    if(key_pressed){
        mask |= POLLIN|POLLRDNORM;
    }

    return mask;
}

static ssize_t fasync_key_read (struct file *filp, char __user *buff, size_t count, loff_t *offset){
    int ret;
    char status;

    wait_event_interruptible(key_waitq, key_pressed);
    key_pressed = 0;
    
    status = pin_descs[0].key_value;
    ret = copy_to_user(buff, &status, sizeof(status));
    if(ret<0){
        printk(KERN_ERR"put to user failed/\n");
        return -EIO;
    }

    return ret;

}

static int fasync_key_open (struct inode *inode, struct file *filp){
    int ret;
    ret = request_irq(IRQ_EINT0, key_irq, IRQT_BOTHEDGE, KEY_NAME, &pin_descs[0]);
    if(ret<0){
        printk(KERN_ERR"%s, request_irq failed.\n", KEY_NAME);
        goto request_failed;
    }

    return 0;

request_failed:
    free_irq(IRQ_EINT0,&pin_descs[0]);
    return -EIO;
}

static int fasync_key_close(struct inode *inode, struct file *filp){
    
    free_irq(IRQ_EINT0,&pin_descs[0]);

    return 0;
}



static dev_t key_no;
static struct cdev key_dev;
static struct file_operations key_ops = {
    .owner = THIS_MODULE,
    .open = fasync_key_open,
    .read = fasync_key_read,
    .release = fasync_key_close,
    .poll = fasync_key_poll,
    .fasync =fasync_key_fasync,
};
static struct class *key_class;
static struct class_device *key_class_device;

static int __init fasync_key_init(void){
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
module_init(fasync_key_init);

static void __exit fasync_key_exit(void){
    
    iounmap(gpfcon);
    class_device_unregister(key_class_device);
    class_destroy(key_class);
    cdev_del(&key_dev);
    unregister_chrdev_region(key_no,DEV_CNT);
}
module_exit(fasync_key_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccdroid");
MODULE_DESCRIPTION("a simple interrupt key driver.");

