//
//  pilot.c
//
//  Created by Youri Tolstoy, Gregory Bishop & Kevin Da Costa on 4/12/12.
//

#include "shared.h"

int main(int argc, const char * argv[])
{
    // Create a unique pilot name base on system timestamp
    struct timeval time;
    gettimeofday(&time, NULL);
    long microsec = time.tv_sec;
    char pilotName[250];
    sprintf(pilotName, "%ld", microsec);
    
    // Init needed directories
    initPaths();
    sprintf(pilotFifoPath, "%s/%s", currentDir, pilotName);
    
    // Create pilot private fifo
    mkfifo(pilotFifoPath, 0666);
    
    // Write ATIS request to fifo-in
    printf(" - Waiting for signal -\n\n");
    FILE * ctFifoFd;
    if ((ctFifoFd = fopen(fifoPath, "w")) != NULL)
    {
        printf("Roger, here is DC%s.\nPlease provide ATIS. Over.\n", pilotName);
        com_mess * mess2send = encode_message(HEADER_HI, pilotName);
        send_message(mess2send, ctFifoFd);
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
        if ((pilotFifoFd = fopen(pilotFifoPath, "r")) != NULL)
        {
#ifdef DEBUG
            printf("Opened !\n");
#endif
            com_mess * ct_mess = read_message(pilotFifoFd);
            // ATIS message correctly received
            if (ct_mess->header == HEADER_ATIS && ct_mess->size == strlen(ct_mess->message))
            {
                printf("ATIS OK, DC%s taking off ! Over.\n", pilotName);
                free_message(ct_mess);
                fclose(pilotFifoFd);
                unlink(pilotFifoPath);
                break;
            }
            
            // ATIS message not correctly received, rerequest ATIS
            if ((ctFifoFd = fopen(fifoPath, "w")) != NULL)
            {
                printf("ATIS KO, please send again !\n");
                com_mess * mess2resend = encode_message(HEADER_HI, pilotName);
                send_message(mess2resend, ctFifoFd);
            }
        }
    }
    
    exit(EXIT_SUCCESS);
}

