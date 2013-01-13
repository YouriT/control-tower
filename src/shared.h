//
//  prefix.h
//  control-tower
//
//  Created by Youri Tolstoy on 4/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#ifndef control_tower_prefix_h
#define control_tower_prefix_h

#define SHARED_FILE_PATH "/Users/Youri/Xcode/control-tower/"
#define FIFO_IN_NAME "fifo-in"
#define ATIS_NAME "atis-1"
#define MAX_BUF 1024

#define HEADER_HI 1
#define HEADER_ATIS 2

typedef struct {
    int size;
    char content[MAX_BUF];
    int lastUp;
} atis;

typedef struct {
    int header;
    size_t size;
    char * message;
} com_mess;

int file_exists(const char * path);

com_mess * encode_message(int header, const char * message);
com_mess * read_message(FILE * fd);
void send_message(com_mess * com, FILE * fd);

#endif
