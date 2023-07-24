#include "main.h"

/**
 * concat_string_strcat - appends the source string src to string dest.
 * @adress: pointer to the destination string
 * @srcpy: pointer to the source string
 *
 * Return: returns a pointer of the modified string dest
 */
char *concat_string_strcat(char *adress, const char *srcpy)
{
	char *p = adress + Length_string_strlen(adress);

	while ((*p++ = *srcpy++))
	{
	}
	return (adress);
}
