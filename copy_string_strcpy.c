#include "main.h"

/**
 * copy_string_strcpy - Copy a string from the source to the buffer
 * @adress: destination buffer pointer
 * @srcpy: source string pointer
 *
 * Return: Pointer to the beginning of the destination buffer
 */
char *copy_string_strcpy(char *adress, const char *srcpy)
{
	char *p = adress;

	while ((*p++ = *srcpy++))
	{
	}
	return (adress);
}
