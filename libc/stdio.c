#include <stdio.h>

int puts(const char* str)
{
    const char* nl = "\n";
    unsigned int ret;

    __asm__ (
        "movl $1, %%eax;"
        "int $0x80;"
        "movl $1, %%eax;"
        "movl %%ecx, %%ebx;"
        "int $0x80;"
            : "=a" (ret)
            : "b" (str), "c" (nl) );

    if (ret != 0)
        // set error indicator here
        return EOF;
    else
        return 0;
}