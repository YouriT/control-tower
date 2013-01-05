//
//  main.c
//  atis
//
//  Created by Youri Tolstoy on 22/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_LENGTH 40



int main(int argc, const char * argv[])
{
    while(1){


    FILE * lockFile = fopen(SHARED_FILE_PATH
                            ATIS_NAME
                            ".lock", "w");
    if (!lockFile)
    {
        printf("Lock file couldn't be created");
        exit(EXIT_FAILURE);
    }

    FILE * atisFile = fopen(SHARED_FILE_PATH
                            ATIS_NAME,"w+");
    if (!atisFile)
    {
        printf("ATIS file couldn't be opened");
        exit(EXIT_FAILURE);
    }

    char meteo[48];
    int i = 0;
    int j;
    srand((int)(time(NULL)));
    while (i < 35)
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

    unlink(SHARED_FILE_PATH
           ATIS_NAME
           ".lock");

    sleep(10);
    }
}


