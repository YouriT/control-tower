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
#include "shared.h"
#include <errno.h>
#include <string.h>




int main(int argc, const char * argv[])
{
    
    char currentDir[MAX_PATH];
    getcwd(currentDir,MAX_PATH);
    
    char atisPath[MAX_PATH];
    char lockPath[MAX_PATH];
    char fifoPath[MAX_PATH];
    
    sprintf(atisPath, "%s/%s", currentDir, ATIS_NAME);
    sprintf(lockPath, "%s.lock", atisPath);
    sprintf(fifoPath, "%s/%s", currentDir, FIFO_IN_NAME);
    
    // Create a fifo channel to let pilot's talk
    // Check FIFO existance
    FILE * fifo;
    while (1)
    {
        // Load atis
        // Don't load if ATIS server is writing in the file
        // Done by checking if atis-1.lock exists or not
        while (file_exists(lockPath)) {
            sleep(1);
        }
        
        // Let's load atis file content in memory
        FILE * atisFile = fopen(atisPath, "r");
        
        atis atis;
        if (atisFile)
        {
            char c;
            int i = 0;
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
            exit(EXIT_FAILURE);
        }
        
        
        if ((fifo = fopen(fifoPath, "r")))
        {
            // Fifo already exists, just read it
            printf("COM received on main channel..\n");
            com_mess * decoded_message = read_message(fifo);
            if (decoded_message->header == HEADER_HI)
            {
                // Pilot requests to write on new pipe
                char temp[MAX_PATH] = {0};
                sprintf(temp, "%s/%s", currentDir, decoded_message->message);
                printf("Pilot path %s\n",temp);
                printf("DC%s needs ATIS, proceeding..\n", decoded_message->message);
                FILE * pilotFifo = fopen(temp, "w");
                com_mess * mess_to_send = encode_message(HEADER_ATIS, atis.content);
                send_message(mess_to_send, pilotFifo);
                
                fclose(pilotFifo);
            }
        }
        else if(mkfifo(fifoPath, 0666) != 0)
        {
            printf("Error while creating FIFO file :\ncode : %d\nmessage : %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (fifo)
            fclose(fifo);
    }
    exit(EXIT_SUCCESS);
}

