#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

/**
 * _memcpy - Copies 'n' bytes from the memory area to another
 * @dest: Pointer to the destination memory area.
 * @src: Pointer to the source memory area.
 * @n: Number of bytes to be copied.
 *
 * Return: A pointer to the destination memory area.
 */
void *_memcpy(void *address, const void *src, size_t n)
{
	char *a = address;
	const char *s = src;

	if (address == NULL || src == NULL)
	{
		return NULL;
	}

	while (n--)
	{
		*a++ = *s++;
	}
	return (address);
}

/**
 * _malloc_size - Returns the size of the memory block allocated for ptr.
 * @ptr: Pointer to the memory block.
 *
 * Return: The size of the memory block allocated for ptr.
 */
static size_t _malloc_size(void *ptr)
{
	const size_t memo_size = sizeof(size_t);
	size_t *memo_ptr = (size_t *)ptr - 1;

	if (ptr == NULL)
	{
		return 0;
	}
	return (*memo_ptr - memo_size);
}

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
		return NULL;
	}

	if (ptr == NULL)
	{
		return malloc(new_size);
	}

	if (new_ptr)
	{
		size_t old_size = _malloc_size(ptr);
		size_t copy_size = (new_size < old_size) ? new_size : old_size;

		_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
/* Function declarations */
char *copy_string_strcpy(char *adress, const char *srcpy);
int compare_string_strcmp(const char *s1, const char *s2);
size_t Length_string_strlen(const char *s);
char *concat_string_strcat(char *adress, const char *srcpy);
char *_environ_get(const char *env_var);
char *comand_get(char *command);
char **str_split(char *buf, const char *del);
void comnd_exec(char **argums, char **env);

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

/**
 * Length_string_strlen - calculates the length of the input string s.
 * @s: pointer to the input string
 *
 * Return: returns a size_t value representing the length of the string s.
 */
size_t Length_string_strlen(const char *s)
{
	const char *p = s;

	while (*p)
	{
		p++;
	}
	return ((size_t)(p - s));
}

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

#include <stdbool.h>

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


/**
 * str_split - splits a string into an array of tokens
 * @buf: pointer to the string to be split
 * @del: pointer to the delimiter string used for splitting
 * Return: returns a pointer to a dynamically allocated array of pointers
 */
char **str_split(char *buf, const char *del)
{
	char **tokens = NULL;
	int num_tokens = 0;
	char *strike = strtok(buf, del);

	while (strike)
	{
		tokens = _realloc(tokens, (num_tokens + 1) * sizeof(char *));
		if (!tokens)
		{
			perror("Memory allocation error");
			exit(1);
		}
		tokens[num_tokens] = strike;
		num_tokens++;
		strike = strtok(NULL, del);
	}
	tokens = _realloc(tokens, (num_tokens + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("Memory allocation error");
		exit(1);
	}
	tokens[num_tokens] = NULL;
	return (tokens);
}

/**
 * comnd_exec - attempts to execute a command specified by argums[0]
 * @argums: pointer to an array of strings
 * @env: pointer to an array of strings
 * Return: The function does not return any value
 */
void comnd_exec(char **argums, char **env)
{
	char *comnd = comand_get(argums[0]);

	if (comnd)
	{
		if (execve(comnd, argums, env) == -1)
		{
			perror("Execution error");
			free(comnd);
			exit(1);
		}
	}
	else
	{
		printf("Command not found\n");
		exit(1);
	}
}

/**
 * main - Simple shell program that takes user input
 * @ac: The number of command-line arguments (unused).
 * @av: An array of command-line argument strings (unused).
 * @env: An array of environment variables.
 * Return: Returns 0 on successful execution.
 */
int main(int ac, char **av, char **env)
{
	char *buf;
	size_t buf_size = 0;
	char **argums;
	pid_t pid;
	int status, x_character;

	(void)ac;
	(void)av;

	buf = NULL;

	while (1)
	{
		write(1, "$ ", 2);
		x_character = getline(&buf, &buf_size, stdin);
		if (x_character == -1)
		{
			write(1, "\n", 1);
			exit(1);
		}
		argums = str_split(buf, " \t\n");
		if (compare_string_strcmp(argums[0], "exit") == 0)
			exit(0);
		pid = fork();
		if (pid == 0)
		{
			comnd_exec(argums, env);
		}
		else
		{
			wait(&status);
			free(argums);
		}
	}
	free(buf);
	return (0);
}
