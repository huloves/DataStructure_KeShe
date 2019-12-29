#include "my_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_error(char *type, int line) {
    printf("file\nline: %d\n", line);
    perror(type);
}
