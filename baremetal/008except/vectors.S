.global _vectors
_vectors:

@ enter new vectors, this will overlap original uboot vectors
@ must copy uboot vectors to new ram area, now skip
vectors:
    b reset
    b undefined
    b swi
    b abort_prefetch
    b abort_data
    .word 0x0
    b irq
    b fiq

reset:
undefined:
    mov r13, #0x32000000     @ und_sp, switch sp,need alloc
    stmfd sp!, {r0-r14}      @ store svc mode register

    adr r0, info
    ldr r1, print
    mov lr, pc
    mov pc, r1

                             @ restore svc and svc mode register,then jump to return address
    ldmfd sp, {r0-r13, pc}^  @ hint: mrs r0, spsr; msr cpsr, r0
switch_svc:                  @ switch undefined to svc mode, continue main_loop in uboot
    b switch_svc
swi:
abort_prefetch:
abort_data:
irq:
fiq:

print:
    .word 0x33f97ff8
info:
    .asciz "enter undefined mode\n"
