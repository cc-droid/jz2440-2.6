@ apcs rules
@ r0-r3 <----------> a0-a3 一般用于参数传递，被调用者不保证调用完成后保持调用者原来的值
@ r4-r9 <----------> v1-v7 可用于临时存储变量，被调用者保证调用完成后保持调用者原来的值
@ apcs只是c语言的约定规则，并不是编译器内置的规则，可以人为打破，但为了可移植性要遵守
@ 标准C库函数遵循apcs

.global _start

.text
_start:
    ldr r4, =0x32000000
    str lr, [r4]  @store go function return address


    @ call printf in uboot
    mov r5, r0
    mov r6, r1    @ apcs rules, r0 r1 changed， but maybe r0 r1 are using at go or main_loop
    ldr r0, =info
    ldr r1, =name  @ equal===> printf("%s, hello world\n", "ccdroid");
    mov lr, pc     @ store next instruction address, print funtion return lr
    ldr pc, print  @ jump to print function
    mov r0, r5
    mov r1, r6    @ restore r0 r1

    ldr lr, [r4]  @ restore go function return address 
    mov pc, lr    @ jump go next address, just main_loop function


print:
    .word 0x33f97ff8 @ addr of printf in uboot
info:
    .ascii "%s, hello world\n"
name:
    .ascii "ccdroid"
