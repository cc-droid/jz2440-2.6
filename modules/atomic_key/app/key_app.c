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
#include <poll.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>

static int fd;

void print_usage(void);

void key_signal(int signo){
    char status;
    int ret = read(fd,&status, 1); 
    if(ret<0){
        printf("read key io failed.\n");
        return;
    }
    printf("key status:%d\n", status);

    return;

}

/* key_app <status>
 */

int main(int argc, char* argv[]){
    int ret;
    int oflags;

    if(argc != 2){
        print_usage();
        return -1;
    }
    
    signal(SIGIO, key_signal);
    fd = open("/dev/atomic_key", O_RDWR);
    if(fd<0){
        printf("open atomic_key failed.\n");
        return -EIO;
    }
    
    fcntl(fd, F_SETOWN, getpid());
    oflags = fcntl(fd,F_GETFL);
    fcntl(fd, F_SETFL, oflags|FASYNC);

     while(1){ 
        sleep(1000);    
     }

    close(fd);
    return ret;
}

void print_usage(void){
    printf("Usage:\n");
    printf("\t led_app <on|off|status>");
}
