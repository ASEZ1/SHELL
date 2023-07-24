#include "main.h"

/**
 * compare_string_strcmp - Compares two strings s1 and s2
 * @s1: pointer to the first string
 * @s2: pointer to the second string
 *
 * Return: An integer indicating the comparison result.
 */
int compare_string_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((int)(unsigned char)(*s1) - (int)(unsigned char)(*s2));
}
