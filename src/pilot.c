//
//  main.c
//  pilot
//
//  Created by Youri Tolstoy on 22/12/12.
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
#include <sys/time.h>

int main(int argc, const char * argv[])
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long microsec = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
    char pilotName[250];
    sprintf(pilotName, "%ld", microsec);
    
    // Make pilot fifo
    char currentDir[MAX_PATH];
    getcwd(currentDir, MAX_PATH);
    
    char ctFifoInPath[MAX_PATH];
    char pilotFifoPath[MAX_PATH];
    
    sprintf(ctFifoInPath, "%s/%s", currentDir, FIFO_IN_NAME);
    sprintf(pilotFifoPath, "%s/%s", currentDir, pilotName);
    
    mkfifo(pilotFifoPath, 0666);
    
    // Talk to control-tower
    printf(" - Waiting for signal -\n\n");
    FILE * ctFifoFd;
    if ((ctFifoFd = fopen(ctFifoInPath, "w")))
    {
        printf("Roger, here is DC%s.\nPlease provide ATIS. Over.\n", pilotName);
        com_mess * mess2send = encode_message(HEADER_HI, pilotName);
        send_message(mess2send, ctFifoFd);
        free(mess2send);
        fclose(ctFifoFd);
    }
    else
    {
        printf("Fatal error while talking to control tower. Aborting execution.");
        exit(EXIT_FAILURE);
    }
    
    
    printf("Waiting for ATIS..\n");
    // Listen on pilot fifo
    while (1)
    {
        FILE * pilotFifoFd;
        if ((pilotFifoFd = fopen(pilotFifoPath, "r")))
        {
            // Fifo already exists, just read it
            printf("Opened !\n");
            com_mess * ct_mess = read_message(pilotFifoFd);
            if (ct_mess->header == HEADER_ATIS && ct_mess->size == strlen(ct_mess->message))
            {
                printf("ATIS OK, DC%s taking off ! Over.\n", pilotName);
                fclose(pilotFifoFd);
                unlink(pilotFifoPath);
                fclose(ctFifoFd);
                free(ct_mess);
                break;
            }
            
//            if (ct_mess != NULL)
//                free(ct_mess);
            
            if ((ctFifoFd = fopen(ctFifoInPath, "w")))
            {
                printf("ATIS KO, please send again !\n");
                com_mess * mess2resend = encode_message(HEADER_HI, pilotName);
                send_message(mess2resend, ctFifoFd);
                free(mess2resend);
                fclose(ctFifoFd);
            }
            
//            if (pilotFifoFd != NULL)
//                fclose(pilotFifoFd);
        }
    }
//    if (ctFifoFd != NULL)
//        fclose(ctFifoFd);
    
    exit(EXIT_SUCCESS);
}

