OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0;

    .text : {
              mode.o
              *(.text)
            }

    .data : {
              *(.data)
            }

    .bss  : {
              *(.bss)
            }
}
