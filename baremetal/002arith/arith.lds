OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0;

    .text : {
              arith.o
              *(.text)
            }

    .data : {
              *(.data)
            }

    .bss  : {
              *(.bss)
            }
}
