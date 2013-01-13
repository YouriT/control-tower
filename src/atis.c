//
//  main.c
//  atis
//
//  Created by Youri Tolstoy on 22/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shared.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_LENGTH 40



int main(int argc, const char * argv[])
{
    while(1){

    char path[MAXPATHLEN];
    getwd(path);
    strcat(path, ATIS_NAME);

    FILE * lockFile = fopen(path
                            ".lock", "w");
    if (!lockFile)
    {
        printf("Lock file couldn't be created");
        exit(EXIT_FAILURE);
    }

    FILE * atisFile = fopen(path,"w+");
    if (!atisFile)
    {
        printf("ATIS file couldn't be opened");
        exit(EXIT_FAILURE);
    }

    char meteo[42];
    int i = 0;
    int j;
    srand((int)(time(NULL)));
    while (i < 40)
    {
        j = 0;
        while (j < 5)
        {
            int n = 26 * (rand() / (RAND_MAX + 1.0));
            char c = (char)(n+65);
            meteo[i+j] = c;
            j++;

        }
        meteo[i+j] = ' ';
        i += j + 1;
    }

    printf("Meteo : %s\nSize : %zd\n",meteo,strlen(meteo));

    fwrite(meteo, sizeof(char), strlen(meteo), atisFile);

    fclose(atisFile);
    fclose(lockFile);

    unlink(path
           ".lock");

    sleep(10);
    }
}


