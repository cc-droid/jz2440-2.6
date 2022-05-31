OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0;

    .text : {
              watch.o
              *(.text)
            }

    .data : {
              *(.data)
            }

    .bss  : {
              *(.bss)
            }
}
