#define _GNU_SOURCE
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define INPUT_ALLOC 5

char *read_input() {
    // HOLY FUCK JUST USE getline(3)
    char *buf = malloc(sizeof(char) * INPUT_ALLOC);
    long bytes = read(STDIN_FILENO, buf, INPUT_ALLOC);
    long total_bytes = bytes;
    while (bytes == INPUT_ALLOC) {
        buf = realloc(buf, total_bytes + INPUT_ALLOC);
        bytes = read(STDIN_FILENO, buf + total_bytes, INPUT_ALLOC);
        total_bytes += bytes;
    }
    if (bytes == -1) {
        perror("womp");
        exit(EXIT_FAILURE);
    }
    buf[total_bytes - 1] = '\0';
    return buf;
}

#define ARGV_MALLOC 5
#define ARGV_DELIM " "
char **parse_input(char *input) {
    size_t capacity = ARGV_MALLOC;
    char **argv = malloc(capacity * sizeof(char*));

    char *token = strtok(input, ARGV_DELIM);
    size_t i = 0;
    argv[i] = token;

    while ((token = strtok(NULL, ARGV_DELIM))) {
        i++;
        if (i == capacity) {
            capacity += ARGV_MALLOC;
            argv = realloc(argv, capacity * sizeof(char*));
        }
        argv[i] = token;
    }
    argv[i + 1] = NULL;
    return argv;
}
// strtok(3)

int main(int argc, char *argv[]) {
    while (true) {
        printf("$ ");
        fflush(stdout);

        char *line = NULL;
        size_t size = 0;
        ssize_t bytes;
        bytes = getline(&line, &size, stdin);
        line[bytes - 1] = '\0';
        char **new_argv = parse_input(line);
        if (strcmp(new_argv[0], "q") == 0) {
            free(line);
            free(new_argv);
            exit(EXIT_SUCCESS);
        }
        pid_t pid = fork();
        switch (pid) {
            case -1:
                // Fork failed
                perror("fork");
                exit(EXIT_FAILURE);
                break;
            case 0:
                // Child
                int err = execvp(new_argv[0], new_argv);
                // execvp only returns in case of a failure
                perror("turtle-shell");
                // printf("whoops %d\n", err);
                exit(EXIT_FAILURE);
                break;
            default:
                // Parent
                waitpid(-1, NULL, 0);
                break;
        }
        free(line);
        free(new_argv);
    }
}
