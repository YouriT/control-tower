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
    char fifoName[250];
    sprintf(fifoName, "%ld", microsec);

    // Make pilot fifo
    char fifoPath[MAX_PATH];
    char fifoNamePath[MAX_PATH];

    getcwd(fifoPath, MAX_PATH);
    strcpy(fifoNamePath,fifoPath);
    mkfifo(fifoPath, 0666);

    strcat(fifoPath, fifoName);
    strcat(fifoNamePath, FIFO_IN_NAME);




    // Talk to control-tower
    printf(" - Waiting for signal -\n\n");
    FILE * ct;
    if ((ct = fopen(fifoNamePath, "w")))
    {
        printf("Roger, here is DC%s.\nPlease provide ATIS. Over.\n", fifoName);
        com_mess * mess2send = encode_message(HEADER_HI, fifoName);
        send_message(mess2send, ct);
        fclose(ct);
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
        printf("Debut boucle\n");
        FILE * fifo;
        if ((fifo = fopen(fifoPath, "r")))
        {
            // Fifo already exists, just read it
            printf("Opened !\n");
            com_mess * ct_mess = read_message(fifo);
            if (ct_mess->header == HEADER_ATIS && ct_mess->size != strlen(ct_mess->message))
            {
                printf("ATIS OK, DC%s taking off ! Over.\n", fifoName);
                unlink(fifoPath);
                fclose(fifo);
                fclose(ct);
                break;
            }


            if ((ct = fopen(fifoNamePath, "w")))
            {
                printf("ATIS KO, please send again !\n");
                com_mess * mess2resend = encode_message(HEADER_HI, fifoName);
                send_message(mess2resend, ct);
                printf("Fin boucle\n");
                fclose(ct);
            }
        }
    }
    return 0;
}

