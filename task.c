#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *custom_strcpy(char *dest, const char *src) {
    char *p = dest;
    while ((*p++ = *src++)) {}
    return dest;
}
int custom_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (int)(unsigned char)(*s1) - (int)(unsigned char)(*s2);
}
size_t custom_strlen(const char *s) {
    const char *p = s;
    while (*p) {
        p++;
    }
    return (size_t)(p - s);
}
char *custom_strcat(char *dest, const char *src) {
    char *p = dest + custom_strlen(dest);
    while ((*p++ = *src++)) {}
    return dest;
}
char *_getenv(const char *env_var) {
    int i = 0;

    extern char **environ;
    char *key;

    while (environ[i]) {
        int j = 0;

        key = environ[i];
        while (env_var[j] && key[j] && env_var[j] == key[j]) {
            j++;
        }
        if (env_var[j] == '\0' && key[j] == '=') {
            return key + j + 1; 
        }
        i++;
    }
    return NULL;
}
char *get_command(char *command) {
    char *path = _getenv("PATH");
    char *token;
    char *cmd_full;
    struct stat st;

    token = strtok(path, ":");
    while (token) {
        size_t len_token = custom_strlen(token);
        size_t len_cmd = custom_strlen(command);
        cmd_full = (char *)malloc(len_token + len_cmd + 2);
        if (!cmd_full) {
            perror("Memory allocation error");
            exit(1);
        }
        custom_strcpy(cmd_full, token);
        custom_strcat(cmd_full, "/");
        custom_strcat(cmd_full, command);
        if (stat(cmd_full, &st) == 0) {
            return cmd_full;
        }
        free(cmd_full);
        token = strtok(NULL, ":");
    }
    return NULL;
}
char **split_string(char *buffer, const char *del) {
    char **tokens = NULL;
    int num_tokens = 0;
    char *token = strtok(buffer, del);
    while (token) {
        tokens = (char **)realloc(tokens, (num_tokens + 1) * sizeof(char *));
        if (!tokens) {
            perror("Memory allocation error");
            exit(1);
        }
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL, del);
    }
    tokens = (char **)realloc(tokens, (num_tokens + 1) * sizeof(char *));
    if (!tokens) {
        perror("Memory allocation error");
        exit(1);
    }
    tokens[num_tokens] = NULL;
    return tokens;
}
void execute_command(char **args, char **env) {
    char *cmd = get_command(args[0]);
    if (cmd) {
        if (execve(cmd, args, env) == -1) {
            perror("Execution error");
            free(cmd); 
            exit(1);
        }
    } else {
        printf("Command not found\n");
        exit(1);
    }
}

int main(int ac, char **av, char **env) {
    
    char *buffer;
    size_t buffer_size = 0;
    char **args;
    pid_t pid;
    int status, n_chars;

    (void)ac;
    (void)av;

    buffer = NULL;

    while (1) {
        write(1, "$ ", 2);
        n_chars = getline(&buffer, &buffer_size, stdin);
        if (n_chars == -1) {
            write(1, "\n", 1);
            exit(1);
        }
        args = split_string(buffer, " \t\n");
        if (custom_strcmp(args[0], "exit") == 0)
            exit(0);
        pid = fork();
        if (pid == 0) {
            execute_command(args, env);
        } else
            wait(&status);
    }
    return (0);
}