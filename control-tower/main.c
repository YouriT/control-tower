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
#include <errno.h>
#include <string.h>

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
        while ((fread(&c, sizeof(char), 1, atisFile) > 0) && i < MAX_BUF)
        {
            atis.content[i] = c;
            i++;
        }
        if (i == MAX_BUF)
            printf("Buffer of ATIS file as run out of memory\n");
        else
            printf("ATIS content : %s\n",atis.content);
        
        fclose(atisFile);
    }
    else
    {
        printf("ATIS FILE DOESN'T EXISTS\n");
    }
    
    // Create a fifo cannal to let pilot's talk
    // Check FIFO existance
    
    while (1)
    {
        FILE * fifo;
        if ((fifo = fopen(SHARED_FILE_PATH
                          FIFO_IN_NAME, "r")))
        {
            // Fifo already exists, just read it
            char buf[MAX_BUF];
            fread(buf, sizeof(char), MAX_BUF, fifo);
            printf("content : %s\n",buf);
        }
        else if(mkfifo(SHARED_FILE_PATH
                       FIFO_IN_NAME, 0666) != 0)
        {
            printf("Error while creating FIFO file :\ncode : %d\nmessage : %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

