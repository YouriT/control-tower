//
//  control-tower
//
//  Created by Youri Tolstoy, Gregory Bishop & Kevin Da Costa on 4/12/12.
//

#include "shared.h"

int main(int argc, const char * argv[])
{
    // Init needed files paths
    initPaths();
    
    // Init some vars
    FILE * fifo;
    int creating_fifo = 0;
    atis atis;
    while (1)
    {
        // Loop is not creating a fifo
        if (creating_fifo == 0)
        {
            // Load atis
            // Don't load if ATIS server is writing in the file
            // Done by checking if atis-1.lock exists or not
            while (file_exists(lockPath)) {
                sleep(1);
            }
            
            // Let's load atis file content in memory
            FILE * atisFile = fopen(atisPath, "r");
            
            if (atisFile != NULL)
            {
                char c;
                int i = 0;
                memset(atis.content, 0, MAX_BUF);
                // fill atis struct with correct ATIS
                while ((fread(&c, sizeof(char), 1, atisFile) > 0) && i < MAX_BUF-1)
                {
                    atis.content[i] = c;
                    i++;
                }
                if (i == MAX_BUF-1)
                {
                    printf("Buffer of ATIS file has run out of memory\n");
                    exit(EXIT_FAILURE);
                }
                else
                    printf("ATIS content : %s\n",atis.content);
                
                
                fclose(atisFile);
            }
            else
            {
                printf("ATIS FILE DOESN'T EXIST\n");
                exit(EXIT_FAILURE);
            }
        }
        else
            creating_fifo = 0;
        
#ifdef DEBUG
        printf("Fifo in path : %s\n", fifoPath);
#endif
        int makeFifo = -1;
        // Check fifo in channel existance
        if ((fifo = fopen(fifoPath, "r")))
        {
            // Fifo already exists, just read it
            printf("COM received on main channel..\n");
            com_mess * decoded_message = read_message(fifo);
            
            if (decoded_message->header == HEADER_HI)
            {
                // Pilot requests to CT to write ATIS on his pipe
                char temp[MAX_PATH];
                sprintf(temp, "%s/%s", currentDir, decoded_message->message);
#ifdef DEBUG
                printf("Pilot path %s\n",temp);
#endif
                printf("DC%s needs ATIS, proceeding..\n", decoded_message->message);
                free_message(decoded_message);
                
                FILE * pilotFifo = fopen(temp, "w");
                com_mess * mess_to_send = encode_message(HEADER_ATIS, atis.content);
                send_message(mess_to_send, pilotFifo);
            }
            fclose(fifo);
        }
        // Create a fifo channel to let pilot's talk
        // Check FIFO existance
        else if((makeFifo = mkfifo(fifoPath, 0666) != 0))
        {
            printf("Error while creating FIFO file :\ncode : %d\nmessage : %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (makeFifo == 0)
        {
            creating_fifo = 1;
        }
    }
    exit(EXIT_SUCCESS);
}

