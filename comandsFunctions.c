#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "messageCode.h"
#include "comands.h"

// Declaram numerele magice utilizate
#define  lungime_maxima 1000
#define dimensiune_format 10

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
    printf("-------------------------Options Syntax----------------------------\n");
    printf("\t  -c [input_file] [output_file] [message_file]\n");
    printf("\t  -d [input_file]\n");
    printf("\t  -t [file1] [file2]\n");
    printf("\t  -l [log_file] [file1] [file2]\n");
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

void getFileSize(const char *filePaths[]) {
    FILE *input = fopen(filePaths[2], "r");
    fseek(input, 0L, SEEK_END);
    long int size1 = ftell(input);
    printf("Size of input file: %ld\n", size1);
    fclose(input);

    FILE *output = fopen(filePaths[3], "r");
    fseek(output, 0L, SEEK_END);
    long int size2 = ftell(output);
    printf("Size of output file: %ld\n", size2);
    fclose(output);
}

void readAndDecode(const char *filePaths[]) {
    unsigned int width, height, max_color;
    unsigned int i, j;
    unsigned char red, green, blue;
    char format[dimensiune_format];
    char word[lungime_maxima];

    // Deschiderea fisierului care contine imaginea cu mesajul secret codificat
    FILE *input = fopen(filePaths[2], "rb");

    fscanf(input, "%s", format);   //Formatul
    fscanf(input, "%u", &width);   // Latimea
    fscanf(input, "%u", &height);  // Inaltimea
    fscanf(input, "%u", &max_color);   // Valoarea maxima pentru culoare

    char newline_buffer = '\n';
    fscanf(input, "%c", &newline_buffer);

    TMatrice **image = (TMatrice**)malloc(height * sizeof(TMatrice*));
    for(i = 0; i < height; i++) {
        image[i] = (TMatrice*)malloc(width * sizeof(TMatrice));
    }

    // Citirea matricei de pixeli din noul fișierul de intrare (cel care contine mesajul secret codificat)
    for(i = 0; i < height; i++) {
        for(j = 0; j< width; j++) {
            fread(&red, sizeof(char), 1, input);
            fread(&green, sizeof(char), 1, input);
            fread(&blue, sizeof(char), 1, input);
            image[i][j].r = red;
            image[i][j].g = green;
            image[i][j].b = blue;
        }
    }

    // Apelarea functiei de decodificare
    char cuvant2[lungime_maxima];
    decodifica(cuvant2, image, lungime_maxima, width);
    cuvant2[lungime_maxima] = '\0';

    // Afișarea mesajului secret care a fost codificat
    char *p = strchr(cuvant2,'$');
    p[0] = '\0';
    printf("%s\n",cuvant2);

    // Eliberarea memoriei alocate
    for (i = 0; i < height; i++) {
        free(image[i]);
    }

    free(image);

    //Inchiderea fisierului
    fclose(input);
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
        getFileSize(filePaths);
        return;
    } else if (strcmp(command, "-d") == 0) {
        readAndDecode(filePaths);
    } else if (strcmp(command, "-c") == 0) {
        printf("Atentie! Mesajul care se doreste a fi codificat trebuie sa se termine in '$' \n");

        // Deschiderea fișierelor
        FILE *f1 = fopen(filePaths[2],"rb");
        FILE *f2 = fopen(filePaths[3], "wb");
        FILE *f3 = fopen(filePaths[4],"rt");

        unsigned int latime,inaltime,culoare_maxima;
        unsigned int i,j;
        unsigned char rosu,verde,albastru;
        char sir[dimensiune_format];
        char cuvant[lungime_maxima];

        fgets(cuvant, sizeof(cuvant), f3);  //textul de codificat

        fscanf(f1,"%s",sir);    //Formatul
        fscanf(f1,"%u",&latime);    // Latimea
        fscanf(f1,"%u",&inaltime);  // Inaltimea
        fscanf(f1,"%u",&culoare_maxima);    // Valoarea maxima pentru culoare

        char newline = '\n';
        fscanf(f1,"%c",&newline);

        TMatrice **a = (TMatrice**)malloc(inaltime*sizeof(TMatrice*));
        if (a == NULL)
            return;

        for (i = 0;i < inaltime;i++) {
            a[i] = (TMatrice*)malloc(latime*sizeof(TMatrice));
            if (a[i] == NULL)
                return;
        }

        // Citirea matricei de pixeli din fișierul de intrare
        for (i = 0; i < inaltime; i++) {
            for (j = 0; j< latime; j++) {
                fread(&rosu,sizeof(char),1,f1);
                fread(&verde,sizeof(char),1,f1);
                fread(&albastru,sizeof(char),1,f1);
                a[i][j].r = rosu;
                a[i][j].g = verde;
                a[i][j].b = albastru;
            }
        }

        // Codificarea și scrierea informațiilor în fișierul de ieșire
        if (sizeof(cuvant) > inaltime * latime * 3) {
            printf("Mesajul secret este prea lung pentru a fi codificat\n");
            return;
        }

        // Apelarea functiei de codificare
        code(a, latime, filePaths);

        fprintf(f2, "%s\n", sir);
        fprintf(f2, "%u %u\n", latime, inaltime);
        fprintf(f2, "%u\n", culoare_maxima);

        for (i = 0; i < inaltime; i++) {
            for (j = 0; j < latime; j++) {
                fwrite(&a[i][j].r, sizeof(char), 1, f2);
                fwrite(&a[i][j].g, sizeof(char), 1, f2);
                fwrite(&a[i][j].b, sizeof(char), 1, f2);
            }
        }

        // Inchiderea fisierului care contine imaginea cu mesajul secret codificat
        fclose(f2);

        // Eliberarea memoriei alocate
        for (unsigned int i = 0; i < inaltime; i++) {
            free(a[i]);
        }
        free(a);
    }
}