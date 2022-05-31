/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */

extern void* info;

void print_info(int value){
    void (*printf) (const char *fmt, ...);

    printf = (void*)0x33f97ff8;
    printf("================begin=========================\n");
    printf(info, value);
    printf("================end===========================\n");
    
    return;
}
