/*
    Header file for all included libraries, function prototypes, and global variables
*/
#ifndef helpers
    #include <stdio.h> // for fopen, fclose, fgets
    #include <stdlib.h> // malloc, realloc, free
    #include <string.h> // for strsep, strcmp and other string functions
    #include <time.h> // include for getting timestamp
    #include <sys/stat.h> // for stat function, used to see if virtual disk is empty and if not, read in contents of virtual disk to data structures
    #include <errno.h> // used in conjunction with stat.h to see if virtual disk file is empty

    /* defined constants */
    #define MAX_BLOCK_SIZE 512 // 512 bytes
    #define MIN_FILE_SIZE 32768 // 32768 chars (or bytes)
    #define MIN_FILE_NAME_LEN 12 // 8 characters for name and 4 for extension (including ".")
    #define MAX_FILE_NAME_LEN 19 // 15 characters for name and 4 for extensions (including ".")

    /* function prototypes */
    char* getTime(); // gets the date and time to be used
    void createFile(char* filename); // creates a file with the name provided, allows the user to add content to that file, and adds an entry to the FAT
    void deleteFile(char* filename); // deletes a file with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to FREESPACE for future use)
    void createDirectory(char* dirname); // creates a directory with the name provided and adds an entry to the FAT
    void deleteDirectory(char* dirname); // deletes a directory with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to FREESPACE for future use)
    void readFile(char* filename); // finds the file in the FAT, and thus in the physical directory and prints its contents to screen for the user
    void writeToFile(char* filename); // allow the user to write to a file already created
    void printDirHierarchy(); // prints the current directory hierarchy

    int openFile(char* filename); // adds the file by filename to the open_file_array, returns 1 on success and 0 on failure
    int closeFile(char* filename); // closes the file if it's open, returns 1 on success and 0 on failure




    
#endif