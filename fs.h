/*
    Header file for all included libraries, function prototypes, and global variables
*/
#ifndef helpers
    #include <stdio.h> // for fopen, fclose, fgets
    #include <stdlib.h> // malloc, realloc, free
    #include <string.h> // for strsep, strcmp and other string functions
    #include <time.h> // include for getting timestamp
    #include <sys/stat.h> // for stat function, to see if file exists in a directory to be executed on

    /* defined constants */
    #define MAX_BLOCK_SIZE 512 // 512 bytes
    #define MIN_FILE_SIZE 32768 // 32768 chars (or bytes)
    #define MIN_FILE_NAME_LEN 12 // 8 characters for name and 4 for extension (including ".")
    #define MAX_FILE_NAME_LEN 19 // 15 characters for name and 4 for extensions (including ".")

    /* function prototypes */
    char* getTime();
    
#endif
