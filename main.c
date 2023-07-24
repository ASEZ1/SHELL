#include <main.h>

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
		tokens = (char **)_realloc(tokens, (num_tokens + 1) * sizeof(char *));
		if (!tokens)
		{
			perror("Memory allocation error");
			exit(1);
		}
		tokens[num_tokens] = strike;
		num_tokens++;
		strike = strtok(NULL, del);
	}
	tokens = (char **)_realloc(tokens, (num_tokens + 1) * sizeof(char *));
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
