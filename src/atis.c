//
//  atis.c
//
//  Created by Youri Tolstoy, Gregory Bishop & Kevin Da Costa on 22/12/12.
//

#include "shared.h"

int main(int argc, const char * argv[])
{
    // Init needed file paths
    initPaths();
    
    while(1)
    {
        FILE * lockFile = fopen(lockPath, "w");
        if (lockFile == NULL)
        {
            printf("Lock file couldn't be created\n");
            exit(EXIT_FAILURE);
        }
        
        FILE * atisFile = fopen(atisPath,"w+");
        if (atisFile == NULL)
        {
            printf("ATIS file couldn't be opened\n");
            exit(EXIT_FAILURE);
        }
        
        char meteo[ATIS_LENGTH] = {0};
        int i = 0;
        int j;
        srand((int)(time(NULL)));
        // Generate a ATIS_LENGTH ATIS message (randomized)
        while (i < ATIS_LENGTH-1)
        {
            j = 0;
            while (j < 5)
            {
                int n = 26 * (rand() / (RAND_MAX + 1.0));
                char c = (char)(n+65);
                meteo[i+j] = c;
                j++;
            }
            meteo[i+j] = (char)32;
            i += j + 1;
            if (i == ATIS_LENGTH)
                meteo[ATIS_LENGTH-1] = '\0';
        }
        
        printf("Meteo : \"%s\"\nSize : %zd\n",meteo,strlen(meteo));
        
        // Write & close ATIS to atis file
        fwrite(meteo, sizeof(char), strlen(meteo), atisFile);
        
        fclose(atisFile);
        fclose(lockFile);
        
        unlink(lockPath);
        
        sleep(10);
    }
    exit(EXIT_SUCCESS);
}


