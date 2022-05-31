@ apcs rules
@ r0-r3 <----------> a0-a3 一般用于参数传递，被调用者不保证调用完成后保持调用者原来的值
@ r4-r10 <----------> v1-v7 可用于临时存储变量，被调用者保证调用完成后保持调用者原来的值
@ apcs只是c语言的约定规则，并不是编译器内置的规则，可以人为打破，但为了可移植性要遵守
@ 标准C库函数遵循apcs

.global _start

.text
_start:
    mov ip, sp
    stmfd sp!, {fp, ip, lr, pc}
    sub fp, ip, #4

    @ call printf in uboot
    ldr r0, =info
    ldr r1, =name  @ equal===> printf("%s, hello world\n", "ccdroid");
    mov lr, pc     @ store next instruction address, print funtion return lr
    ldr pc, print  @ jump to print function

    sub sp, fp, #12
    ldmfd sp, {fp,sp,pc}

print:
    .word 0x33f97ff8 @ addr of printf in uboot
info:
    .asciz "%s, hello world\n"
name:
    .asciz "ccdroid"
