#include "fs.h"

/* Global variables and data structures */
// char strs[numOfStrings][stringLengths]
char* FAT[100][5]; // 100 rows with 5 columns - Name, Dir/Pointer, Index Number, Parent, and Date
char physicalDir[100][MAX_BLOCK_SIZE];
char open_file_array[100][MAX_FILE_NAME_LEN];
FILE *disk_ptr; // virtual disk file to be opened and accessed
int startingIndex = 0;

int main(int argc, char** argv) {
    char line[100]; // line to use for user inputted commands
    char* fatCols = "File Name | FileOrDir | Index Number | Parent | Date and Time\n";

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
            disk_ptr = fopen(argv[1], "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
            if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n");// print error message
                exit(0); // exit program
            }
            printf("User-specified virtual disk opened successfully!\n\n"); // FOR TESTING

            int i, j = 0;
            for (i = 0; i < 100; i++) {
                for (j = 0; j < 5; j++) {
                    FAT[i][j] = "."; // set all entries to "." which means they're empty
                    strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
                    //printf("%s ", FAT[i][j]);
                }
                // printf(" | row = %d\n", i); // FOR TESTING
            }

            fputs(fatCols, disk_ptr); // write FAT columns to virtual disk file's first line to be used
            createFile("root"); // create root directory and thus inserting it into the FAT

        } else { // virtual disk file is NOT empty AND exists within the project directory
            printf("Virtual disk has content stored within it!\n\n"); // print message FOR TESTING
            // Open virtual disk to be accessed
            disk_ptr = fopen(argv[1], "a+"); // open virtual disk with appending and reading privileges so it does not overwrite the disk's contents
            rewind(disk_ptr); // rewind to beginning of file since opening in append mode brings file pointer to end of file
            if (!disk_ptr) { // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n");// print error message
                exit(0); // exit program
            }
            printf("User-specified virtual disk opened successfully!\n"); // FOR TESTING




/************************************************* START CURRENTLY WORKING ON **************************************************/

            // Extract characters from file and store in character ch
            // char ch;
            // while(!feof(disk_ptr)) {
            //     ch = fgetc(disk_ptr);
            //     if (ch == '\n') {
            //         startingIndex++;
            //     }
            // }
            // startingIndex -= 1; // since first line was technically the FAT column names
            // printf("Starting index = %d\n", startingIndex); // FOR TESTING

            // printf("%s | ", FAT[0][0]);
            // printf("%s | ", FAT[0][1]);
            // printf("%s | ", FAT[0][2]);
            // printf("%s | ", FAT[0][3]);
            // printf("%s", FAT[0][4]);

            rewind(disk_ptr); // rewind disk file pointer again to parse each line into appropriate spot within data structures
            char currentDiskContent[1000];
            startingIndex = -1; // to avoid parsing through first line of virtual disk which just had the column headers for the FAT
            while(!feof(disk_ptr)) {
				if(fgets(currentDiskContent, 1000, disk_ptr)) {
					// if (currentDiskContent[strlen(currentDiskContent)-1] == '\n') { // remove trailing newline if there is one in command
					// 	currentDiskContent[strlen(currentDiskContent)-1] = '\0'; // replace that newline character with null character
					// }
					// argv = parseFile(currentDiskContent); // parse line from file
                    // int i = 0;
					// for(i = 0; i < 100; i++) {
                    //     printf("argv: %s\n", argv[i]);
                    // }
                    startingIndex++;
                    printf("startingIndex inside read: %d\n", startingIndex);
                    int j = 0;
                    if(startingIndex >= 0) { // want to avoid parsing and using the part of the virtual disk that just contains the FAT column names

                        // Parse currentDiskContent
                        char *rest = NULL;
                        char *token;
                        for (token = strtok_r(currentDiskContent, "|\n\t\r\a", &rest); ((token != NULL) && (j < 5)); token = strtok_r(NULL, "|\n\t\r\a", &rest), j++) {   
                            printf("token:%s\n", token);
                            FAT[startingIndex][j] = token;
                        }
                    }

                    printf("printf says %s\n", currentDiskContent);
				}
			}
            printf("\nWINNER: %s | ", FAT[0][0]);
            printf("%s | ", FAT[0][1]);
            printf("%s | ", FAT[0][2]);
            printf("%s | ", FAT[0][3]);
            printf("%s\n", FAT[0][4]);

            printf("\nWINNER #2: %s | ", FAT[1][0]);
            printf("%s | ", FAT[1][1]);
            printf("%s | ", FAT[1][2]);
            printf("%s | ", FAT[1][3]);
            printf("%s\n", FAT[1][4]);

            printf("\nWINNER #3: %s | ", FAT[2][0]);
            printf("%s | ", FAT[2][1]);
            printf("%s | ", FAT[2][2]);
            printf("%s | ", FAT[2][3]);
            printf("%s\n", FAT[2][4]);




/************************************************* END CURRENTLY WORKING ON **************************************************/




            // // Set remainder of indices in FAT to "." to mark them as empty
            // int i, j = 0;
            // for (i = startingIndex; i < 100; i++) {
            //     for (j = 0; j < 5; j++) {
            //         FAT[i][j] = "."; // set all entries to "." which means they're empty
            //         strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
            //         printf("%s ", FAT[i][j]);
            //     }
            //     printf(" | row = %d\n", i); // FOR TESTING
            // }


             // then read contents of file into data structures (file table, physical dir, and open_file_array (what files are open etc.))y

        }

        // Prompt user to manipulate file system with certain commands
        printf("Welcome to my file system! Please use one of the following commands below to manipulate it.\n");
        printf("Create a file with: create 'filename' and include its extension\n");
        printf("Delete a file with: delete 'filename' and include its extension\n");
        printf("Create a directory with: createDir 'directoryname'\n");
        printf("Delete a directory with: deleteDir 'directoryname'\n");
        printf("Print the contents of a file with: cat 'filename'\n");
        printf("Print the current contents of a directory with: printDir\n");
        printf("Print the directory hierarchy with: printHierarchy\n\n");

        // Read command from user, exit on EOF conditions (Ctrl + D or user enters "exit")
        while(1) {
            printf("myfs >> ");
            if (fgets(line, sizeof(line), stdin) == NULL) break; // get command from stdin, breaks if Ctrl + D entered
            if (line[strlen(line)-1] == '\n') { // remove trailing newline if there is one
				line[strlen(line)-1] = '\0'; // replace that newline with null character
			}
            printf("Command entered: %s\n\n", line); //print command user wishes to execute within the file system

            // tokenize what the user entered to separate the space, then check to see their command and file name, pass that file/dir name to the functions to be used

            argv = parseCommand(line, argv, &argc); // tokenize and parse user entered command
            printf("argv1 = %s\n", argv[0]); // FOR TESTING
            printf("argv2 = %s\n", argv[1]);
            printf("argv3 = %s\n", argv[2]);
            printf("argc = %d\n", argc);
            

            if (strcmp(line, "create") == 0) { // if user wants to create file
                // parse command to get filename wanting to be created
                printf("User wants to create a file..\n"); // FOR TESTING
                createFile(argv[1]);

            } else if (strcmp(line, "createDir") == 0) { // user wants to create directory
                // parse command to get filename wanting to be created
                printf("User wants to create a directory..\n"); // FOR TESTING
            } else if (strcmp(line, "delete") == 0) { // user wants to delete a file
                // parse command to get filename wanting to be created
                printf("User wants to delete a file..\n"); // FOR TESTING
            } else if (strcmp(line, "deleteDir") == 0) { // user wants to delete a directory
                // parse command to get filename wanting to be created
                printf("User wants to delete a directory..\n"); // FOR TESTING
            } else if (strcmp(line, "cat") == 0) { // user wants to print the contents of a file
                // parse command to get filename wanting to be created
                printf("User wants to print the contents of a file..\n"); // FOR TESTING
            } else if (strcmp(line, "printHierarchy") == 0) { // user wants to print current structure of files
                // parse command to get filename wanting to be created
                printf("User wants to print the current directory hierarchy..\n"); // FOR TESTING
            } else if (strcmp(line, "printDir") == 0) { // user wants to print contents of current directory
                // parse command to get filename wanting to be created
                printf("User wants to print the contents of a directory..\n"); // FOR TESTING
            } else if (strcmp(line, "exit") == 0){ // exit if user enters "exit"
                printf("Exiting file system..\n"); // print a messsage
                //fclose(disk_ptr); // close the disk
                return 0;
            } else {
                printf("Enter a valid command..\n"); // print message for user to enter a valid command
            }
        }

        return 0;

    } else { // too many or two few arguments, print error and exit
        printf("You entered too many or too few arguments; you should only enter the executable followed by the disk you wish to use..\n"); // print error
        exit(0); // exit program
    }
}






























/*
    Creates a file with the name provided, allows the user to add content to that file, and adds an entry to the FAT
    Accepts a file name to search for within the FAT
*/
void createFile(char* filename) {
    // ensure you're within a dir

    if(strcmp(filename, "root") == 0) {
        insertEntry(0, filename, "dir", "0", "none", getTime());
    } else {
        // find empty entry in the FAT to create the new file at
        int emptyIndex = findEmptyEntryFAT();
        printf("Found an empty entry at row: %d\n", emptyIndex); // FOR TESTING
        // find empty index in physical directory to store file contents in
        int emptyBlock = findEmptyBlock();
        printf("Found an empty block at index = %d\n", emptyBlock);
        // convert emptyBlock to a string and assign that to the appropriate column in the FAT entry when inserting entry
        char* indexNumber;
        sprintf(indexNumber, "%d", emptyBlock);
        printf("Index number as a string: %s\n", indexNumber);
        // insert entry into FAT - 
        insertEntry(emptyIndex, filename, "file", indexNumber, "/", getTime());

        // prompt user to enter file contents to store at empty physical dir index

    }
    
} 

/*
    Deletes a file with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to FREESPACE for future use)
    Accepts a file name to search for within the FAT
*/
void deleteFile(char* filename) {
    
}

/*
    Adds the file by filename to the open_file_array, returns 1 on success and 0 on failure
    Accepts a file name to search for within the FAT to be added to the open_file_array
*/
int openFile(char* filename) {

    return 1;
}

/*
    Removes the file by filename from the open_file_array, returns 1 on success and 0 on failure
    Accepts a file name to search for within the open_file_array to remove
*/
int closeFile(char* filename) {

    return 1;
}

/*
    Creates a directory with the name provided and adds an entry to the FAT
    Accepts a directory name from the user to be used in the creation of that directory
*/
void createDirectory(char* dirname) {

}

/*
    Deletes a directory with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to FREESPACE for future use)
    Accepts a directory name to search for within the FAT
*/
void deleteDirectory(char* dirname) {
    
}

/*
    Finds the file in the FAT, and thus in the physical directory and prints its contents to screen for the user
    Accepts a filename to 
*/
void readFile(char* filename) {

}

/*
    Copies a file from your host OS to your file system
    Accepts a file name from your host OS to copy over to your file system, this name will also be the name of the file in my FAT
*/
void writeToFile(char* filename) {

}

/*
    Prints the current directory hierarchy
*/
void printDirHierarchy() {
    
}

/*
    Prints the current directory's contents
*/
void printDirContent() {
    
}

/* Parse command read in from user, increment argc for each string in command */
char** parseCommand(char* command, char** argv, int *argc) {       
	int i = 0;
	*(argc) = 0; // make sure for each command, argc is set back to 0
    for (i = 0; i < 100; i++) {
        argv[i] = strtok_r(command, " \t\r\a", &command); // seperate each string by space and store that string in the respective index of params C-String array, nice thing about strtok_r is that it removes empty spaces all together and all other delimiters
		
        if (argv[i] == NULL) break;
		(*argc)++; // increment argc for every string in the command
    }
    argv[strlen(*argv) + 1] = NULL; // set first location in array after C-Strings to NULL
	return argv; // return argv array to be executed on
}

/* Parse current disk content to read into appropriate data structures */
char** parseFile(char* currentDiskContent) {       
	int i = 0;
    char** argv;
    int *argc;
	*(argc) = 0; // make sure for each command, argc is set back to 0
    for (i = 0; i < 100; i++) {
        argv[i] = strtok_r(currentDiskContent, " \t\r\a", &currentDiskContent); // seperate each string by space and store that string in the respective index of params C-String array, nice thing about strtok_r is that it removes empty spaces all together and all other delimiters
		
        if (argv[i] == NULL) break;
		(*argc)++; // increment argc for every string in the command
    }
    argv[strlen(*argv) + 1] = NULL; // set first location in array after C-Strings to NULL
	return argv; // return argv array to be executed on
}









/* Function helpers */

/*
    Gets the current date and time for FAT entries
    Accepts no argument and returns a string that will contain the date and time
*/
char* getTime() {
    time_t newTime = time(NULL); // get the time
    char* time = ctime(&newTime); // get time in string
    //time[strlen(time) - 1] = 0; // remove newline character from the end of that time string for printing purposes - note that NULL character is now at the end
    return time; // use ctime() to get formatted date and time as a string
}

/*
    Finds a row at which the FAT is empty to be used in creating a new file
    Returns the index of a row to be used at which the FAT is empty
*/
int findEmptyEntryFAT() {
    int i = 0;
    for (i = 0; i < 100; i++) { // go through FAT, if filename column is "." then that entry is empty
        if (strcmp(FAT[i][0], ".") == 0) { // check if the current row's filename is ".", indicating an empty entry
            return i; // return index of empty row in FAT
        }
    }
    printf("Never found an empty entry in the FAT..\n"); // never found an index of an empty entry
    return -1; // return -1 to indicate failure
}

/*
    Finds an empty index at which to store a file's contents in the physical directory
    Returns the index of a row to be used at which the FAT is empty
*/
int findEmptyBlock() {
    int i = 0;
    for (i = 0; i < 100; i++) { // go through FAT, if filename column is "." then that entry is empty
        if (strcmp(physicalDir[i], ".") == 0) { // check if the current row's filename is ".", indicating an empty entry
            return i; // return index of empty row in FAT
        }
    }
    printf("Never found an empty entry in the FAT..\n"); // never found an index of an empty entry
    return -1; // return -1 to indicate failure
}

/*
    Inserts an entry with the appropriate fields into the FAT at the first empty index
*/
void insertEntry(int emptyIndex, char* name, char* fileOrDir, char* indexNumber, char* parent, char* timestamp) {
    // assign all values to appropriate spots
    FAT[emptyIndex][0] = name;
    FAT[emptyIndex][1] = fileOrDir;
    FAT[emptyIndex][2] = indexNumber;
    FAT[emptyIndex][3] = parent;
    FAT[emptyIndex][4] = timestamp;
    // write that info to the virutal disk file where FAT is shown with separators for each entry
    fprintf(disk_ptr, "%s | ", FAT[emptyIndex][0]);
    fprintf(disk_ptr, "%s | ", FAT[emptyIndex][1]);
    fprintf(disk_ptr, "%s | ", FAT[emptyIndex][2]);
    fprintf(disk_ptr, "%s | ", FAT[emptyIndex][3]);
    fprintf(disk_ptr, "%s", FAT[emptyIndex][4]);
}