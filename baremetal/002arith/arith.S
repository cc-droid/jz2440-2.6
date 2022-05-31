.global _start
.global print_info

_start:
    mov r0, #1
    mov r1, #2
    add r0, r0, r1

    b print_info

