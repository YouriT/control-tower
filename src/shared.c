//
//  shared.c
//  control-tower, atis, pilot
//
//  Created by Youri Tolstoy, Gregory Bishop & Kevin Da Costa on 4/12/12.
//

#include "shared.h"

// Init files paths needed by atis, control-tower and pilot
void initPaths()
{
    getcwd(currentDir, MAX_PATH);
    
    sprintf(atisPath, "%s/%s", currentDir, ATIS_NAME);
    sprintf(lockPath, "%s/%s", currentDir, ATIS_LOCK);
    sprintf(fifoPath, "%s/%s", currentDir, FIFO_IN_NAME);
}

// Check the file existance in argument
int file_exists(const char * path)
{
    FILE * f;
    if ((f = fopen(path, "r")) != NULL)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

// Write the message struct given in arg into the opened file descriptor
// Pre : fd is already opened and writable
// Post : fd closed and message dealloc
void send_message(com_mess * com, FILE * fd)
{
    fwrite(&com->header, sizeof(int), 1, fd);
#ifdef DEBUG
    printf("--------------------------------------------\nHeader : %d\n",com->header);
#endif
    
    com->size = strlen(com->message);
#ifdef DEBUG
    printf("Length : %zu\n",com->size);
#endif
    
    fwrite(&com->size, sizeof(size_t), 1, fd);
    fwrite(com->message, sizeof(char), strlen(com->message), fd);
#ifdef DEBUG
    printf("Message : %s\n--------------------------------------------\n",com->message);
#endif
    free_message(com);
    fclose(fd);
}

// Encode header and string message to a com_mess struct by putting size of string
// Pre : message is allocated and not empty, header is a proper int from HEADER_XXXX
// Post : a com_mess is allocated, filled and returned
com_mess * encode_message(int header, const char * message)
{
    com_mess * ret = (com_mess *)malloc(sizeof(com_mess));
    ret->header = header;
    ret->size = strlen(message);
    ret->message = (char *)malloc(sizeof(char) * (ret->size+1));
    memset(ret->message, 0, ret->size+1);
    sprintf(ret->message, "%s", message);
    return ret;
}

// Read a message from a fifo file
// Pre : given fd is opened and readable
// Post : a com_mess is allocated, filled and returned
com_mess * read_message(FILE * fd)
{
    com_mess * ret = (com_mess *)malloc(sizeof(com_mess));
    fread(&ret->header, sizeof(int), 1, fd);
#ifdef DEBUG
    printf("--------------------------------------------\nHeader : %d\n",ret->header);
#endif
    
    fread(&ret->size, sizeof(size_t), 1, fd);
#ifdef DEBUG
    printf("Length : %zu\n",ret->size);
#endif
    
    ret->message = (char *)malloc(sizeof(char)*(ret->size+1));
    memset(ret->message, 0, ret->size+1);
    fread(ret->message, sizeof(char), ret->size, fd);
#ifdef DEBUG
    printf("Message : %s\n--------------------------------------------\n",ret->message);
#endif
    return ret;
}

// Free a com_mess struct
void free_message(com_mess * com)
{
    free(com->message);
    free(com);
}
