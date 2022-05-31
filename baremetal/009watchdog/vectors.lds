OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0;

    .text : {
              vectors.o
              *(.text)
            }

    .data : {
              *(.data)
            }

    .bss  : {
              *(.bss)
            }
}
