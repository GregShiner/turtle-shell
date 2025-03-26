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
// int parse_input(char *input, char **argv) {}

int main(int argc, char *argv[]) {
    while (true) {
        printf("$ ");
        fflush(stdout);
        char **new_argv;

        char *input = read_input();
        pid_t pid = fork();
        switch (pid) {
            case -1:
                // Fork failed
                perror("fork");
                exit(EXIT_FAILURE);
                break;
            case 0:
                // Child
                int err = execvp(input, NULL);
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
        free(input);
    }
}
