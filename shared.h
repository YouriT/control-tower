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

typedef struct {
    int size;
    char content[MAX_BUF];
    int lastUp;
} atis;

#endif
