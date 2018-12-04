#include "fs.h"

/* Global variables and data structures */
// char strs[numOfStrings][stringLengths]
char* FAT[100][5]; // 100 rows with 5 columns - Name, Dir/Pointer, Index Number, Parent, and Date
char physicalDir[100][MAX_BLOCK_SIZE];
char open_file_array[100][MAX_FILE_NAME_LEN];

int main(int argc, char** argv) {
    char line[100]; // line to use for user inputted commands

    // FOR TESTING - test manually putting in valuesklj
    FAT[0][0] = "myfile.txt"; // name of file/directory
    FAT[0][1] = "file"; // whether it is a directory or a pointer (and thus a file)
    FAT[0][2] = "0"; // index number where the file's contents are stored in the physical directory
    FAT[0][3] = "/"; // the parent of the file/directory
    FAT[0][4] = getTime(); // the date and time for when this file was last accessed for any reason
    printf("%s\n", FAT[0][0]);
    printf("%s\n", FAT[0][1]);
    printf("%s\n", FAT[0][2]);
    printf("%s\n", FAT[0][3]);
    printf("%s\n", FAT[0][4]);
    printf("index num of FAT entry: %d\n\n", atoi(FAT[0][2])); // FOR TESTING

    // check that argv[1] is a disk, if not exit

    if (argc == 2) { // only 2 args should be the file system executable and the disk wished to be used
        
        //printf("%s", getTime()); // FOR TESTING

        // Format disk - assign space for boot sector, root directory (the start of physical directory), and the FAT
        struct stat stat_record; // used to see if virtual disk is empty - see 'man stat' for more details
        if(stat(argv[1], &stat_record)) {// execute stat(), print error if there is one and exit program
            printf("%s\n", strerror(errno)); // print error from OS
            printf("Please enter a valid filename within this directory to use next time..\n"); // prompt user to enter valid virtual disk filename
            exit(0); // exit program
         } else if (stat_record.st_size <= 1) {// the virtual disk file is empty BUT exists within the project directory
            printf("Empty virtual disk!\n"); // print message FOR TESTING
            // Open virtual disk to be accessed
            FILE *disk_ptr = fopen(argv[1], "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
            if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n");// print error message
                exit(0); // exit program
            }
            printf("User-specified virtual disk opened successfully!\n\n"); // FOR TESTING
            
            // insert entry into file table to represent root directory

        } else { // virtual disk file is NOT empty AND exists within the project directory
            printf("Virtual disk has content stored within it!\n\n"); // print message FOR TESTING
            // Open virtual disk to be accessed
            FILE *disk_ptr = fopen(argv[1], "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
            if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n");// print error message
                exit(0); // exit program
            }
            printf("User-specified virtual disk opened successfully!\n"); // FOR TESTING

             // then read contents of file into data structures (file table, physical dir, and open_file_array (what files are open etc.))y
             
        }

        // Prompt user to manipulate file system with certain commands
        printf("Welcome to my file system! Please use one of the following commands below to manipulate it.\n");
        printf("Create a file with: create 'filename'\n");
        printf("Delete a file with: delete 'filename'\n");
        printf("Create a directory with: create 'directoryname'\n");
        printf("Delete a directory with: delete 'directoryname'\n");
        printf("Print the contents of a file with: cat 'filename'\n");
        printf("Add to a file with: write 'filename'\n");
        printf("Print the directory hierarchy with: print hiearchy\n\n");

        // Read command from user, exit on EOF conditions (Ctrl + D or user enters "exit")
        while(1) {
            printf("myfs >> ");
            if (fgets(line, sizeof(line), stdin) == NULL) break; // get command from stdin, breaks if Ctrl + D entered
            printf("Command entered: %s\n", line); //print command user wishes to execute within the file system

            // tokenize what the user entered to separate the space, then check to see their command and file name, pass that file/dir name to the functions to be used

            if (strcmp(line, "exit\n") == 0) { // exit if user enters "exit"
                printf("Exiting file system..\n"); // print a messsage
                //fclose(disk_ptr); // close the disk
                return 0;
            }
        }

        return 0;

    } else { // too many or two few arguments, print error and exit
        printf("You entered too many or too few arguments; you should only enter the executable followed by the disk you wish to use..\n"); // print error
        exit(0); // exit program
    }

    
}