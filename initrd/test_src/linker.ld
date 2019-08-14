OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 1024 * 1024 * 1024;

    .text : ALIGN(4)
    {
        start.o(.text)
        *(.text)
    }

    .data : ALIGN(4)
    {
        *(.data)
    }

    .rodata : ALIGN(4)
    {
        *(.rodata*)
    }
}