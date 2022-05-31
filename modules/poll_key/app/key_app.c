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



#define LED_STATUS_ON 0
#define LED_STATUS_OFF 1

void print_usage(void);

/* led_app <on|off|status>
 */

int main(int argc, char* argv[]){
    int fd;
    char status;
    int ret;
    struct pollfd fds[1];

    if(argc != 2){
        print_usage();
        return -1;
    }

    fd = open("/dev/interrupt_key", O_RDWR);
    if(fd<0){
        printf("open interrupt_key failed.\n");
        return -EIO;
    }

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    while(1){ 
        ret = poll(fds, 1, 5000);
        if(ret == 0){
            printf("poll time out.\n");
        }else{
            ret = read(fd,&status, sizeof(status));
            if(ret<0){
                printf("read interrupt_key failed.\n");
                return -EIO;
            }
            if(status == 0){
                printf("interrupt_key:DOWN.\n");
            }
        }
    }

    close(fd);
    return ret;
}

void print_usage(void){
    printf("Usage:\n");
    printf("\t led_app <on|off|status>");
}
