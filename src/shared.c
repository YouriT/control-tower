//
//  shared.c
//  control-tower
//
//  Created by Youri Tolstoy on 25/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#include <stdio.h>
#include "shared.h"
#include <string.h>
#include <stdlib.h>

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
}

com_mess * encode_message(int header, const char * message)
{
    com_mess * ret = (com_mess *)malloc(sizeof(com_mess));
    ret->header = header;
    ret->size = strlen(message);
    ret->message = (char *)malloc(sizeof(char) * (ret->size+1));
    memset(ret->message, 0, ret->size);
    sprintf(ret->message, "%s", message);
    return ret;
}

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
    memset(ret->message, 0, ret->size);
    fread(ret->message, sizeof(char), ret->size, fd);
#ifdef DEBUG
    printf("Message : %s\n--------------------------------------------\n",ret->message);
#endif
    return ret;
}
