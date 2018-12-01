#include "fs.h"

/* Global variables and data structures */
// char strs[numOfStrings][stringLengths]
char* FAT[100][5]; // 100 rows with 5 columns - Name, Dir/Pointer, Index Number, Parent, and Date
char physicalDir[100][MAX_BLOCK_SIZE];
char open_file_array[100][MAX_FILE_NAME_LEN];

int main(int argc, char** argv) {
    // check that argv[1] is a disk, if not exit
    // if (argc == 2) { // only 2 args should be the file system executable and the disk wished to be used
    //     // open virtual disk to be accessed
    //     FILE *disk_ptr = fopen(argv[1], "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
    //     if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
    //         printf("Failed to open disk!\n");// print error message
    //         exit(0); // exit program
    //     }

    //     printf("User-specified disk opened successfully!\n"); // FOR TESTING

    //     rest of code will go here when done testing :)

    // } else { // too many or two few arguments, print error and exit
    //     printf("You entered too many or too few arguments; you should only enter the executable followed by the disk you wish to use..\n"); // print error
    //     exit(0); // exit program
    // }

    // open virtual disk to be accessed
    FILE *disk_ptr = fopen("Drive5MB", "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
    if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
        printf("Failed to open disk!\n");// print error message
        exit(0); // exit program
    }

    printf("Disk opened successfully!\n"); // FOR TESTING
    printf("%s", getTime()); // FOR TESTING

    

    // format disk - assign space for boot sector, root directory (the start of physical directory), and the FAT

    // prompt user to manipulate file system with certain commands

    return 0;
}