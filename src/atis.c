//
//  main.c
//  atis
//
//  Created by Youri Tolstoy on 22/12/12.
//  Copyright (c) 2012 Youri Tolstoy. All rights reserved.
//

#include <stdio.h>
#include "shared.h"
#include <time.h>

typedef struct {
    char content[30];
} message;


int main(int argc, const char * argv[])


{
        FILE * lockFile = fopen(SHARED_FILE_PATH
                       ATIS_NAME
                       ".lock", "w")));
        message meteo;

        FILE * atisFile;
        if(atisFILE= fopen(SHARED_FILE_PATH
                       ATIS_NAME,"w+"))){
         int i = 0;
         int j;
         while (i <= 30) {
             j =0;
             while (j<=5){
            int n = rand() % 26;
            char c = (char)(n+65);
            strcat(meteo.content,c);
            i++;
            j++;
             }
            strcat(meteo.content,' ');
            i++;

         }
         fwrite(meteo.content,sizeof(message), 1, atisFile);
         fclose(atisFile);
         fclose(lockFile);
         remove(SHARED_FILE_PATH
                       ATIS_NAME
                       ".lock");

         }



        }
      }
  }
