//
//  shared.h
//  control-tower, atis, pilot
//
//  Created by Youri Tolstoy, Gregory Bishop & Kevin Da Costa on 4/12/12.
//

#ifndef control_tower_prefix_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define control_tower_prefix_h

// File names
#define FIFO_IN_NAME "fifo-in"
#define ATIS_NAME "atis-1"
#define ATIS_LOCK ATIS_NAME ".lock"

// String lengths
#define MAX_BUF 1024
#define MAX_PATH 1024
#define ATIS_LENGTH 42

// Headers for com_mess struct
#define HEADER_HI 1
#define HEADER_ATIS 2

// Declare some global vars
char currentDir[MAX_PATH];
char atisPath[MAX_PATH];
char lockPath[MAX_PATH];
char fifoPath[MAX_PATH];
char pilotFifoPath[MAX_PATH];

// Structs
typedef struct {
    char content[MAX_BUF];
} atis;

typedef struct {
    int header;
    size_t size;
    char * message;
} com_mess;


// Functions definitions
int file_exists(const char * path);
com_mess * encode_message(int header, const char * message);
com_mess * read_message(FILE * fd);
void send_message(com_mess * com, FILE * fd);
void free_message(com_mess * com);
void initPaths();

#endif
