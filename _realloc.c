#include "main.h"

/**
 * _realloc - Resizes the memory block
 * @ptr: Pointer to the memory block to be resized
 * @new_size: New size of the memory block
 *
 * Return: Pointer to the resized memory block
 */
void *_realloc(void *ptr, size_t new_size)
{
	void *new_ptr = malloc(new_size);

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
	{
		return (malloc(new_size));
	}
	if (new_ptr)
	{
		size_t old_size = malloc_size(ptr);
		size_t copy_size = (new_size < old_size) ? new_size : old_size;

		_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
