#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

/**
 * malloc_size - Returns the size of the memory block allocated for ptr.
 * @ptr: Pointer to the memory block.
 *
 * Return: The size of the memory block allocated for ptr.
 */
size_t malloc_size(void *ptr)
{
	const size_t memo_size = sizeof(size_t);
	size_t *memo_ptr = (size_t *)ptr - 1;

	if (ptr == NULL)
	{
		return (0);
	}
	return (*memo_ptr - memo_size);
}

/**
 * _realc - Resizes the memory block
 * @ptr: Pointer to the memory block to be resized
 * @old_size: Old size of the memory block
 * @new_size: New size of the memory block
 *
 * Return: Pointer to the resized memory block
 */
void *_realc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	if (ptr == NULL)
	{
		return (malloc(new_size));
	}

	new_ptr = malloc(new_size);
	if (new_ptr)
	{
		size_t copy_size = (new_size < old_size) ? new_size : old_size;

		memcpy(new_ptr, ptr, copy_size);

		free(ptr);
	}
	return (new_ptr);
}

/* Function declarations */
char *copy_string_strcpy(char *address, const char *src);
int compare_string_strcmp(const char *s1, const char *s2);
size_t Length_string_strlen(const char *s);
char *concat_string_strcat(char *address, const char *src);
char *_environ_get(const char *env_var);
char *comand_get(char *command);
char **str_split(char *buf, const char *del);
void comnd_exec(char **args, char **env);

/**
 * copy_string_strcpy - Copy a string from the source to the buffer
 * @address: destination buffer pointer
 * @src: source string pointer
 *
 * Return: Pointer to the beginning of the destination buffer
 */
char *copy_string_strcpy(char *address, const char *src)
{
	char *p = address;

	while ((*p++ = *src++))
	{
	}
	return (address);
}

/**
 * compare_string_strcmp - Compares two strings s1 and s2
 * @s1: pointer to the first string
 * @s2: pointer to the second string
 *
 * Return:
 *   -1 if s1 is less than s2
 *    0 if s1 is equal to s2
 *    1 if s1 is greater than s2
 */
int compare_string_strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL && s2 == NULL)
		return (0);
	if (s1 == NULL)
		return (-1);
	if (s2 == NULL)
		return (1);

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
 * @address: pointer to the destination string
 * @src: pointer to the source string
 *
 * Return: returns a pointer of the modified string dest
 */
char *concat_string_strcat(char *address, const char *src)
{
	char *p = address + Length_string_strlen(address);

	while ((*p++ = *src++))
	{
	}
	return (address);
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
	int n_tokens = 0;
	char *strike = strtok(buf, del);

	while (strike)
	{
		tokens = _realc(tokens, n_tokens * sizeof(char *), (n_tokens + 1) * sizeof(char *));
		if (!tokens)
		{
			perror("Memory allocation error");
			exit(1);
		}

		tokens[n_tokens] = (char *)malloc(strlen(strike) + 1);
		if (!tokens[n_tokens])
		{
			perror("Memory allocation error");
			exit(1);
		}

		strcpy(tokens[n_tokens], strike);

		n_tokens++;
		strike = strtok(NULL, del);
	}

	tokens = _realc(tokens, n_tokens * sizeof(char *), (n_tokens + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("Memory allocation error");
		exit(1);
	}
	tokens[n_tokens] = NULL;
	return (tokens);
}

/**
 * comnd_exec - attempts to execute a command specified by args[0]
 * @args: pointer to an array of strings
 * @env: pointer to an array of strings
 * Return: The function does not return any value
 */
void comnd_exec(char **args, char **env)
{
	char *comnd = comand_get(args[0]);

	if (comnd)
	{
		if (execve(comnd, args, env) == -1)
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
 * free_tokens - Frees the memory allocated for the tokens array.
 * @tokens: Pointer to the tokens array to be freed.
 * Return: None.
 */
void free_tokens(char **tokens)
{
	char **token_ptr = tokens;

	while (*token_ptr)
	{
		free(*token_ptr);
		token_ptr++;
	}
	free(tokens);
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
	char *buf = NULL;
	size_t buf_size = 0;
	char **argums = NULL;
	pid_t pid;
	int status, x_character;

	(void)ac;
	(void)av;

	while (1)
	{
		write(1, "$ ", 2);
		x_character = getline(&buf, &buf_size, stdin);
		if (x_character == -1)
		{
			write(1, "\n", 1);
			break;
		}
		argums = str_split(buf, " \t\n");
		if (compare_string_strcmp(argums[0], "exit") == 0)
		{
			free_tokens(argums);
			break;
		}
		pid = fork();
		if (pid == 0)
		{
			comnd_exec(argums, env);
		}
		else
		{
			wait(&status);
			free_tokens(argums);
		}
	}
	free(buf);
	return (0);
}
