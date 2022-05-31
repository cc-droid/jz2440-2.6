/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * led_app <on|off>
 *
 */
void print_usage(){
    printf("Usage:\n");
    printf("led_app <on|off>\n");
    return;
}
int main(int argc, char* argv[]){
    int fd = -1;
    char status;
    int ret;

    if(argc != 2){
        print_usage();
    }
    fd = open("/dev/led_driver", O_RDWR);
    if(fd < 0){
        printf("open led_driver failed. \n");
        return -1;
    }
    if(!strcmp(argv[1],"on")){
        status = 1;
    }else if (!strcmp(argv[1],"off")){
        status = 0;
    }else{
        print_usage();
        return -1;
    }
    ret = write(fd,&status,sizeof(status));
    if(ret<0){
        printf("write led_driver failed.\n");
        return -1;
    }

    return 0;
}
