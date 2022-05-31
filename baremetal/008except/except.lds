OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0;

    .text : {
              except.o
              *(.text)
            }

    .data : {
              *(.data)
            }

    .bss  : {
              *(.bss)
            }
}
