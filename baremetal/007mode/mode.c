/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#define USR 0x10
#define SVC 0x13
#define IRQ 0x12
#define UND 0x1a

void print_mode(void){
    unsigned long cpsr;
    void (*printf) (const char *fmt, ...) = (void*)0x33f97ff8;

    __asm__ __volatile__(
        "mrs %0,cpsr\n"
        :"=r"(cpsr)
    );
    
    switch(cpsr&0x1f){
        case USR:
            printf("mode->usr\n");
            break;
        case SVC:
            printf("mode->svc\n");
            break;
        case IRQ:
            printf("mode->irq\n");
            break;
        case UND:
            printf("mode->undefine\n");
            break;
        default:
            printf("mode->invalid\n");

    }
}
