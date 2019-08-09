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

void* memset(void* str, int c, size_t n)
{
	char* str_c = (char*) str;
	for (; n > 0; n--)
		str_c[n-1] = (char) c;
	return str;
}