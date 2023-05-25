#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void printHelpMenu() {
    printf("Commmand syntax: ./steg [-option_name]... [arguments]\n\n");
    printf("----------------------Available File formats-----------------------\n");
    printf("\t\t\tPPM -with P6 header\n");
    printf("\n");
    printf("----------------------------Options--------------------------------\n");
    printf("\t  -h print help menu\n");
    printf("\t  -c code message\n");
    printf("\t  -d decode message\n");
    printf("\t  -t print timestamp when the file was modified\n");
    printf("\t  -l create log file with the time of the curent files\n");
    printf("\t  -lc clear log file\n");
    printf("\t  -s print size of input and output files\n");
    printf("\n");
    printf("----------------------------Options Syntax-------------------------\n");
    printf("\t  -c [input_file] [output_file] [message_file]\n");
    printf("\t  -d [input_file]\n");
    printf("\t  -t [file1] [file2]\n");
    printf("\t  -l [file1] [file2]\n");
    printf("\t  -lc [log_file]\n");
    printf("\t  -s print size of input and output files\n");
    printf("\n");

}

void timestamp(const char *filePath) {
    struct stat attrib;
    stat(filePath, &attrib);
    char date[10];
    strftime(date, 10, "%d-%m-%y", gmtime(&(attrib.st_ctime)));
    printf("The file %s was last modified at %s\n", filePath, date);
    date[0] = 0;
}

void logChanges(FILE *log, const char *files[]) {
    struct stat attrib;
    stat(files[0], &attrib);
    char date[10];
    strftime(date, 10, "%d-%m-%y", gmtime(&(attrib.st_ctime)));
    fprintf(log, "File %s was modified at: %s\n", files[3], date);
    stat(files[1], &attrib);
    strftime(date, 10, "%d-%m-%y", gmtime(&(attrib.st_ctime)));
    fprintf(log, "File %s was modified at: %s\n", files[4], date);
    fclose(log);
}

void clearLog(FILE *log) {
    fprintf(log, "%s", " ");
    fclose(log);
}

void executeCommand(const char *command, const char *filePaths[]) {
    if (strcmp(command, "-t") == 0) {
        timestamp(filePaths[0]);
        return;

    } else if (strcmp(command, "-l") == 0) {
        FILE *log = fopen(filePaths[2], "aw");
        logChanges(log, filePaths);
        return;
    } else if (strcmp(command, "-lc") == 0) {
        FILE *log = fopen(filePaths[2], "w");
        clearLog(log);
        return;
    } else if (strcmp(command, "-s") == 0) {
        return;
    }
}

