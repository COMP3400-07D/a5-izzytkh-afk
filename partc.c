#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s FILEIN\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // Parent: runs parta
        close(pipefd[0]); // close read end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        char* args[] = {"./parta", argv[1], NULL};
        execv("./parta", args);
        perror("execv");
        return 1;
    } else {
        // Child: runs sort
        close(pipefd[1]); // close write end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char* args[] = {"sort", "-t,", "-k2", "-n", NULL};
        execvp("sort", args);
        perror("execvp");
        return 1;
    }

    return 0;
}
