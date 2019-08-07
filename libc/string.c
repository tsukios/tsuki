#include <string.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char* strncpy(char* dest, const char* src, size_t n)
{
	for (; n > 0; n--)
		dest[n-1] = src[n-1];
	return dest;
}