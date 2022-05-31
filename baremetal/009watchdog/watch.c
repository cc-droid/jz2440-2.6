/*
 * Author: ccdroid
 * Date: 
 * Function:
 *
 * Copyright (C) 2022 ccdroid (ccdroid@163.com)
 * Licensed under GPLv2 or later.
 */
#define INT_BASE 0x4a000000
#define SRCPND      (*(volatile unsigned long*)INT_BASE)
#define INTMOD      (*(volatile unsigned long*)(INT_BASE+0x4))
#define INTMSK      (*(volatile unsigned long*)(INT_BASE+0x8))
#define INTPND      (*(volatile unsigned long*)(INT_BASE+0x10))
#define SUBSRCPND   (*(volatile unsigned long*)(INT_BASE+0x18))
#define INTSUBMSK   (*(volatile unsigned long*)(INT_BASE+0x1c))

#define WTBASE 0x53000000
#define WTCON       (*(volatile unsigned long*)WTBASE)
#define WTDAT       (*(volatile unsigned long*)(WTBASE + 4))
#define WTCNT       (*(volatile unsigned long*)(WTBASE+8))

static void (*print) (const char *fmt, ...) = (void*)0x33f97ff8;
void do_irq(void);

void watchdog(void){
 
    int *p = 0x33000000;
    *p = do_irq;
    __asm__ __volatile__(
        "mrs r0, cpsr\n"
        "bic r0, r0, #0x80\n"
        "msr cpsr, r0\n"
        :
        :
        :"r0"
    );
        
    INTMSK &= ~(1<<9);
    INTSUBMSK &= ~(1<<13);

    WTDAT = 0x8000;
    WTCNT = 0x8000;
    WTCON = (1<<2)|(1<<5)|(0x80<<8);

}

void do_irq(void){

   print("irq ....\n");

   SUBSRCPND = SUBSRCPND;
   SRCPND = INTPND;
   INTPND = INTPND;
}
