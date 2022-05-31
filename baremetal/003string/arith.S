.global _start
.global print_info
.global info

.text
_start:
    mov r0, #1
    mov r1, #2
    add r0, r0, r1

    b print_info

info:
    .ascii "r0(1)+r1(2) =%d\n"
