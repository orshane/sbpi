#include "exec.h"
#include <stdio.h>
#include <stdlib.h>

int run(const char *cmd) {
    printf("Executing: %s\n", cmd);
    int ret = system(cmd);
    return ret == 0;
}


