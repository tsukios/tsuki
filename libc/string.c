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

void* memcpy(void* dest, const void* src, size_t n)
{
	uint8_t* dest_b = (uint8_t*) dest;
	uint8_t* src_b = (uint8_t*) src;
	for (; n > 0; n--)
		dest_b[n-1] = src_b[n-1];
	return dest;
}

void* memset(void* str, int c, size_t n)
{
	char* str_c = (char*) str;
	for (; n > 0; n--)
		str_c[n-1] = (char) c;
	return str;
}