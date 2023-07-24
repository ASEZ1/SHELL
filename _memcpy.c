#include "main.h"

/**
 * _memcpy - Copies 'n' bytes from the memory area to another
 * @adress: Pointer to the destination memory area.
 * @srcpy: Pointer to the source memory area.
 * @n: Number of bytes to be copied.
 *
 * Return: A pointer to the destination memory area.
 */
void *_memcpy(void *adress, const void *srcpy, size_t n)
{
	char *a = adress;
	const char *s = srcpy;

	if (adress == NULL || srcpy == NULL)
	{
		return (NULL);
	}
	while (n--)
	{
		*a++ = *s++;
	}
	return (adress);
}
