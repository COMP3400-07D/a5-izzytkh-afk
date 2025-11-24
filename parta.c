#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];
    
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s FILE\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int upper = 0, lower = 0, digit = 0, space = 0, other = 0;
    ssize_t bytes;

    // Read in 8-byte chunks
    while ((bytes = read(fd, buffer, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < bytes; i++) {
            unsigned char c = buffer[i];
            if (isupper(c)) upper++;
            else if (islower(c)) lower++;
            else if (isdigit(c)) digit++;
            else if (isspace(c)) space++;
            else other++;
        }
    }

    if (bytes < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    close(fd);

    printf("Upper,%d\n", upper);
    printf("Lower,%d\n", lower);
    printf("Number,%d\n", digit);
    printf("Space,%d\n", space);
    printf("Others,%d\n", other);

    return 0;
}
