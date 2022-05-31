.section .text
.global _except
_except:
    mov ip, sp
    stmfd sp!, {fp,ip,lr,pc}
    sub fp, ip, #4

    .word 0x77777777                    @ this is a undefined istruction

    sub sp, fp, #12
    ldmfd sp, {fp,sp,pc}
