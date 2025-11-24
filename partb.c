#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s FILEIN FILEOUT\n", argv[0]);
        return 1;
    }

    char* infile = argv[1];
    char* outfile = argv[2];

    int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Redirect stdout to file
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }

    printf("Category,Count\n");
    fflush(stdout);

    // Prepare args for exec
    char* args[] = {"./parta", infile, NULL};
    execv("./parta", args);

    // Only reaches here if exec fails
    perror("execv");
    close(fd);
    return 1;
}
