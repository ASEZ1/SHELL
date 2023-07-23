#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Function declarations */
char *copy_string_strcpy(char *dest, const char *src);
int compare_string_strcmp(const char *s1, const char *s2);
size_t Length_string_strlen(const char *s);
char *concat_string_strcat(char *dest, const char *src);
char *_environ_get(const char *env_var);
char *cmd_get(char *command);
char **str_split(char *buffer, const char *del);
void cmd_exec(char **args, char **env);

/**
 * copy_string_strcpy - Copy a string from the source to the buffer
 * @dest: destination buffer pointer
 * @src: source string pointer
 *
 * Return: Pointer to the beginning of the destination buffer
 */
char *copy_string_strcpy(char *dest, const char *src)
{
	char *p = dest;

	while ((*p++ = *src++))
	{
	}
	return (dest);
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
 * @dest: pointer to the destination string
 * @src: pointer to the source string
 *
 * Return: returns a pointer of the modified string dest
 */
char *concat_string_strcat(char *dest, const char *src)
{
	char *p = dest + Length_string_strlen(dest);

	while ((*p++ = *src++))
	{
	}
	return (dest);
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

/**
 * cmd_get - searches for a specified command
 * @command: pointer to the command to be searched for
 *
 * Return: returns a pointer to a string representing the full path
 */
char *cmd_get(char *command)
{
	char *path = _environ_get("PATH");
	char *token;
	char *cmd_full;
	struct stat st;

	token = strtok(path, ":");

	while (token)
	{
		size_t len_token = Length_string_strlen(token);

		size_t len_cmd = Length_string_strlen(command);

		cmd_full = (char *)malloc(len_token + len_cmd + 2);

		if (!cmd_full)
		{
			perror("Memory allocation error");
			exit(1);
		}
		copy_string_strcpy(cmd_full, token);
		concat_string_strcat(cmd_full, "/");
		concat_string_strcat(cmd_full, command);
		if (stat(cmd_full, &st) == 0)
		{
			return (cmd_full);
		}
		free(cmd_full);
		token = strtok(NULL, ":");
	}
	return (NULL);
}

/**
 * str_split - splits a string into an array of tokens
 * @buffer: pointer to the string to be split
 * @del: pointer to the delimiter string used for splitting
 * Return: returns a pointer to a dynamically allocated array of pointers
 */
char **str_split(char *buffer, const char *del)
{
	char **tokens = NULL;
	int num_tokens = 0;
	char *token = strtok(buffer, del);

	while (token)
	{
		tokens = (char **)realloc(tokens, (num_tokens + 1) * sizeof(char *));
		if (!tokens)
		{
			perror("Memory allocation error");
			exit(1);
		}
		tokens[num_tokens] = token;
		num_tokens++;
		token = strtok(NULL, del);
	}
	tokens = (char **)realloc(tokens, (num_tokens + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("Memory allocation error");
		exit(1);
	}
	tokens[num_tokens] = NULL;
	return (tokens);
}

/**
 * cmd_exec - attempts to execute a command specified by args[0]
 * @args: pointer to an array of strings
 * @env: pointer to an array of strings
 * Return: The function does not return any value
 */
void cmd_exec(char **args, char **env)
{
	char *cmd = cmd_get(args[0]);

	if (cmd)
	{
		if (execve(cmd, args, env) == -1)
		{
			perror("Execution error");
			free(cmd);
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
	char *buffer;
	size_t buffer_size = 0;
	char **args;
	pid_t pid;
	int status, n_chars;

	(void)ac;
	(void)av;

	buffer = NULL;

	while (1)
	{
		write(1, "$ ", 2);
		n_chars = getline(&buffer, &buffer_size, stdin);
		if (n_chars == -1)
		{
			write(1, "\n", 1);
			exit(1);
		}
		args = str_split(buffer, " \t\n");
		if (compare_string_strcmp(args[0], "exit") == 0)
			exit(0);
		pid = fork();
		if (pid == 0)
		{
			cmd_exec(args, env);
		}
		else
		{
			wait(&status);
			free(args);
		}
	}
	free(buffer);
	return (0);
}

