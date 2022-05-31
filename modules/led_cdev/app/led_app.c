/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define LED_STATUS_ON 0
#define LED_STATUS_OFF 1

void print_usage(void);

/* led_app <on|off|status>
 */

int main(int argc, char* argv[]){
    int fd;
    char status;
    char info[10] = {};

    if(argc != 2){
        print_usage();
        return -EINVAL;
    }

    fd = open("/dev/led_driver", O_RDWR);
    if(fd<0){
        printf("open led driver failed.\n");
        return -EIO;
    }

    if(!strcmp(argv[1],"on")){
        status = LED_STATUS_ON;
        write(fd,&status,sizeof(status));
    }else if(!strcmp(argv[1],"off")){
        status = LED_STATUS_OFF;
        write(fd,&status,sizeof(status));
    }else if(!strcmp(argv[1],"status")){
        read(fd,info,sizeof(info));
        printf("led status:%s\n", info);
    }else{
        print_usage();
    }

    //ioctl

    close(fd);
    return 0;
}

void print_usage(void){
    printf("Usage:\n");
    printf("\t led_app <on|off|status>");
}
