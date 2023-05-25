#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comands.h"

int main(int argc, const char *argv[]) {
    if(!argv[1] || strcmp(argv[1], "-h") == 0 ) {
        printHelpMenu();
        return 0;
    }

    char *fileType = malloc(5 * sizeof(char));
    char *command = malloc(5 * sizeof(char));

    strcpy(fileType, argv[1]);
    strcpy(command, argv[2]);

    executeCommand(command, argv);

    free(fileType);
    free(command);

    return 0;
}
