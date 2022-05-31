/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */


void main(void){
    void (*printf) (const char *fmt, ...);

    printf = (void*)0x33f97ff8;
    printf("hello,world\n");
    
    return;
}
