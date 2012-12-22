//
//  main.c
//  control-tower
//
//  Created by Youri Tolstoy on 4/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../shared.h"

int file_exists(const char * path)
{
    FILE * f;
    if ((f = fopen(path, "r")))
    {
        fclose(f);
        return 1;
    }
    return 0;
}

int main(int argc, const char * argv[])
{
    // Load atis
    // Don't load if ATIS server is writing in the file
    // Done by checking if atis-1.lock exists or not
    while (file_exists(SHARED_FILE_PATH
                       ATIS_NAME
                       ".lock")) {
        usleep(100);
    }
    
    // Let's load atis file content in memory
    FILE * atisFile = fopen(SHARED_FILE_PATH
                            ATIS_NAME, "r");
    if (atisFile)
    {
        char c;
        int i = 0;
        atis atis;
        while ((fread(&c, sizeof(char), 1, atisFile) > 0) && i < 1000) {
            atis.content[i] = c;
            i++;
        }
        printf("ATIS content : %s",atis.content);
    }
    else
    {
        printf("ATIS FILE DOESN'T EXISTS");
    }
}

