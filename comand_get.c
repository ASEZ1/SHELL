#include "main.h"

/**
 * comand_get - searches for a specified command
 * @command: pointer to the command to be searched for
 *
 * Return: returns a pointer to a string representing the full path
 */
char *comand_get(char *command)
{
	char *path, *strike, *comnd_all;
	struct stat strst;


	if (command[0] == '/')
	{
		if (stat(command, &strst) == 0)
		{

			return (command);
		}
	}

	path = _environ_get("PATH");
	strike = strtok(path, ":");

	while (strike)
	{
		size_t len_token = Length_string_strlen(strike);
		size_t len_cmd = Length_string_strlen(command);

		comnd_all = (char *)malloc(len_token + len_cmd + 2);

		if (!comnd_all)
		{
			perror("Memory allocation error");
			exit(1);
		}
		copy_string_strcpy(comnd_all, strike);
		concat_string_strcat(comnd_all, "/");
		concat_string_strcat(comnd_all, command);
		if (stat(comnd_all, &strst) == 0)
		{
			return (comnd_all);
		}
		free(comnd_all);
		strike = strtok(NULL, ":");
	}
	return (NULL);
}
