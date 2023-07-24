#include "main.h"

/**
 * malloc_size - Returns the size of the memory block allocated for ptr.
 * @ptr: Pointer to the memory block.
 *
 * Return: The size of the memory block allocated for ptr.
 */
static size_t malloc_size(void *ptr)
{
	const size_t memo_size = sizeof(size_t);
	size_t *memo_ptr = (size_t *)ptr - 1;

	if (ptr == NULL)
	{
		return (0);
	}
	return (*memo_ptr - memo_size);
}
