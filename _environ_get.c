#include "main.h"

/**
 * _environ_get - retrieves the value of an environment variable.
 * @env_var: pointer to the environment variable name to be retrieved
 *
 * Return: returns a pointer to the value of the specified environment variable
 */
char *_environ_get(const char *env_var)
{
	char **environ = __environ;
	char *key;
	int i = 0;

	while (environ[i])
	{
		int j = 0;

		key = environ[i];
		while (env_var[j] && key[j] && env_var[j] == key[j])
		{
			j++;
		}
		if (env_var[j] == '\0' && key[j] == '=')
		{
			return (key + j + 1);
		}
		i++;
	}
	return (NULL);
}
