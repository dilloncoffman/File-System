#include "fs.h"

/* Global variables and data structures */
// char strs[numOfStrings][stringLengths]
char *FAT[100][5]; // 100 rows with 5 columns - Name, Dir/Pointer, Index Number, Parent, and Date
char physicalDir[100][MAX_BLOCK_SIZE];
char open_file_array[100][MAX_FILE_NAME_LEN];
FILE *disk_ptr;        // virtual disk file to be opened and accessed
int startingIndex = 0; // global variable to keep track of number of filled indices in the FAT
int x = 0;             // global loop counter variable
char **argv;           // global argv to use in other functions other than main()

int main(int argc, char **argv)
{
    char line[100]; // line to use for user inputted commands
    char *fatCols = "File Name | FileOrDir | Index Number | Parent | Date and Time\n";

    if (argc >= 2 && argc < 4)
    { // only 2 args should be the file system executable and the disk wished to be used
        // Format disk - assign space for boot sector, root directory (the start of physical directory), and the FAT
        struct stat stat_record; // used to see if virtual disk is empty - see 'man stat' for more details
        if (stat(argv[1], &stat_record))
        {                                                                                       // execute stat(), print error if there is one and exit program
            printf("%s\n", strerror(errno));                                                    // print error from OS
            printf("Please enter a valid filename within this directory to use next time..\n"); // prompt user to enter valid virtual disk filename
            exit(0);                                                                            // exit program
        }
        else if (stat_record.st_size <= 1)
        {                                    // the virtual disk file is empty BUT exists within the project directory
            printf("Empty virtual disk! Creating root directory..\n\n"); // print message FOR TESTING
            // Open virtual disk to be accessed
            disk_ptr = fopen(argv[1], "a"); // open virtual disk with appending privileges so it does not overwrite the disk's contents
            if (!disk_ptr)
            {                                     // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n"); // print error message
                exit(0);                          // exit program
            }
            #ifdef TESTING
            printf("User-specified virtual disk opened successfully!\n"); // FOR TESTING
            #endif
            int i, j = 0;
            for (i = 0; i < 100; i++)
            {
                for (j = 0; j < 5; j++)
                {
                    FAT[i][j] = ".";             // set all entries to "." which means they're empty
                    strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
                    //printf("%s ", FAT[i][j]);
                }
                // printf(" | row = %d\n", i); // FOR TESTING
            }

            fputs(fatCols, disk_ptr);     // write FAT columns to virtual disk file's first line to be used
            createDirectory("/", "none"); // create root directory and thus inserting it into the FAT, parent for root directory is none
            // Set remainder of indices in FAT to "." to mark them as empty
            for (i = startingIndex; i < 100; i++)
            {
                for (j = 0; j < 5; j++)
                {
                    FAT[i][j] = ".";             // set all entries to "." which means they're empty
                    strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
                    #ifdef TESTING
                    printf("%s ", FAT[i][j]);
                    #endif
                }
                #ifdef TESTING
                printf(" | row = %d\n", i); // FOR TESTING
                #endif
            }
        }
        else
        {                                                             // virtual disk file is NOT empty AND exists within the project directory
            
            printf("Virtual disk has content already stored within it!\n"); // print message which means you're appending to an already filled virtual disk
            // Open virtual disk to be accessed
            disk_ptr = fopen(argv[1], "a+"); // open virtual disk with appending and reading privileges so it does not overwrite the disk's contents
            //rewind(disk_ptr);               // rewind to beginning of file since opening in append mode brings file pointer to end of file
            if (!disk_ptr)
            {                                     // if disk_ptr is NULL, then we failed to open disk
                printf("Failed to open disk!\n"); // print error message
                exit(0);                          // exit program
            }
            #ifdef TESTING
            printf("User-specified virtual disk opened successfully!\n"); // FOR TESTING
            #endif
            //rewind(disk_ptr);
            // int i, j = 0;
            // for (i = startingIndex; i < 100; i++) {
            //     for (j = 0; j < 5; j++) {
            //         FAT[i][j] = "."; // set all entries to "." which means they're empty
            //         strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
            //         printf("%s ", FAT[i][j]);
            //     }
            //     printf(" | row = %d\n", i); // FOR TESTING
            // }
            // fgets(line, 100, disk_ptr);
            char fileLine[2048];                         // fileLine to parse
            rewind(disk_ptr);                            // rewind to beginning to file since opening file with appending will put file pointer at end
            fgets(fileLine, sizeof(fileLine), disk_ptr); // using fgets here to avoid parsing the first line of the file which is just the FAT column names

            while (!feof(disk_ptr)) // go through whole virtual disk file
            {
                fgets(fileLine, sizeof(fileLine), disk_ptr);
                #ifdef TESTING
                printf("fileLine b4: %s\n", fileLine); // print the fileLine got using fgets()
                #endif
                char *rest;                            // used to keep track of the rest of the fileLine to parse
                char *token;                           // token parsed to insert into FAT or other data structure
                x = 0;                                 // counter to keep track of indices
                #ifdef TESTING
                printf("Tokens laid out: ");
                #endif
                for (token = strtok_r(fileLine, "|\t\r\a", &rest), x = 0; ((token != NULL) && (x < 5)); token = strtok_r(NULL, "|\t\r\a", &rest), x++) // for each token in the fileLine read in
                {
                    FAT[startingIndex][x] = (char *)malloc(sizeof(char) * sizeof(fileLine)); // clear a space in memory for assigning string into table
                    strcpy(FAT[startingIndex][x], token);                                    // put it in its proper spot
                    #ifdef TESTING
                    printf("%s ", FAT[startingIndex][x]);
                    #endif
                }
                #ifdef TESTING
                printf("\n");
                printf("startingIndex inside read: %d\n", startingIndex);
                printf("fileLine after: %s\n\n", fileLine);
                #endif
                startingIndex++; // increment startingIndex to be used later when adding more files to file
                
            }

            // Set remainder of indices in FAT to "." to mark them as empty
            int i, j = 0;
            for (i = startingIndex; i < 100; i++)
            {
                for (j = 0; j < 5; j++)
                {
                    FAT[i][j] = ".";             // set all entries to "." which means they're empty
                    strcpy(physicalDir[i], "."); // set all physical directory spaces to "." which means its empty
                    #ifdef TESTING
                    printf("Setting remaind of entries in FAT to '.' which means they're empty.\n");
                    printf("%s ", FAT[i][j]);
                    #endif
                }
                #ifdef TESTING
                printf(" | row = %d\n", i); // FOR TESTING
                #endif
            }

            // then read contents of file into data structures (file table, physical dir, and open_file_array (what files are open etc.))y
        }


        // Read command from user, exit on EOF conditions (Ctrl + D or user enters "exit")
        while (1)
        {
            #ifdef TESTING
            printDirHierarchy(); // print hierarchy with each command FOR TESTING
            #endif

            // Prompt user to manipulate file system with certain commands
            printf("Welcome to my file system! Please use one of the following commands below to manipulate it.\n");
            printf("NOTE: You can create/delete files under a specific diretory by providing a third argument after the name of the file/dir you want to create like so: 'create newfile.txt dir'\n");
            printf("Create a file with: create 'filename' and include its extension\n");
            printf("Delete a file with: delete 'filename' and include its extension\n");
            printf("Create a directory with: createDir 'directoryname'\n");
            printf("Delete a directory with: deleteDir 'directoryname'\n");
            printf("Print the contents of a file with: cat 'filename'\n");
            printf("Print the current contents of a directory with: printDir 'directoryname'\n");
            printf("Print the directory hierarchy with: printHierarchy\n\n");

            printf("myfs >> "); // prompt for user to enter

            if (fgets(line, sizeof(line), stdin) == NULL)
                break; // get command from stdin, breaks if Ctrl + D entered
            if (line[strlen(line) - 1] == '\n')
            {                                  // remove trailing newline if there is one
                line[strlen(line) - 1] = '\0'; // replace that newline with null character
            }
            printf("Command entered: %s\n\n", line); //print command user wishes to execute within the file system

            

            argv = parseCommand(line, argv, &argc); // tokenize what the user entered and return to be used in functions
            #ifdef TESTING
            printf("argv1 = %s\n", argv[0]);        // FOR TESTING
            printf("argv2 = %s\n", argv[1]);
            printf("argv3 = %s\n", argv[2]);
            printf("argc = %d\n", argc);
            #endif

            if (strcmp(line, "create") == 0)
            { // if user wants to create file
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to create a file..\n"); // FOR TESTING
                #endif
                if (argc == 3)
                { // user is specifying a parent for the file to be created
                    createFile(argv[1], argv[2]);
                }
                else if (argc == 2)
                { // argc is 2 which means a user is just specifying a new file to create without specifying a parent
                    createFile(argv[1], "/");
                }
                else
                {                                                      // something went wrong
                    printf("An error occured creating your file.. try again!\n"); // print error message
                }
            }
            else if (strcmp(line, "createDir") == 0)
            { // user wants to create directory
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to create a directory..\n"); // FOR TESTING
                #endif
                if (argc == 3)
                { // user is specifying a parent for the file to be created
                    createDirectory(argv[1], argv[2]);
                }
                else if (argc == 2)
                { // argc is 2 which means a user is just specifying a new file to create without specifying a parent
                    createDirectory(argv[1], "/");
                }
                else
                {                                                           // something went wrong
                    printf("An error occured creating your directory..\n"); // print error message
                }
            }
            else if (strcmp(line, "delete") == 0)
            { // user wants to delete a file
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to delete a file..\n"); // FOR TESTING
                #endif
                deleteFile(argv[1]);
            }
            else if (strcmp(line, "deleteDir") == 0)
            { // user wants to delete a directory
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to delete a directory..\n"); // FOR TESTING
                #endif
                deleteDirectory(argv[1]);
            }
            else if (strcmp(line, "cat") == 0)
            { // user wants to print the contents of a file
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to print the contents of a file..\n"); // FOR TESTING
                #endif
            }
            else if (strcmp(line, "printHierarchy") == 0)
            { // user wants to print current structure of files
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to print the current directory hierarchy..\n"); // FOR TESTING
                #endif
                printDirHierarchy(); // print the current directory hierarchy
            }
            else if (strcmp(line, "printDir") == 0)
            { // user wants to print contents of current directory
                // parse command to get filename wanting to be created
                #ifdef TESTING
                printf("User wants to print the contents of a directory..\n"); // FOR TESTING
                #endif
                printDirContent(argv[1]); // print the specified directory's content - the entries that have parent equal to the directory's name
            }
            else if (strcmp(line, "exit") == 0)
            {                                      // exit if user enters "exit"
                printf("Exiting file system..\n"); // print a messsage
                //fclose(disk_ptr); // close the disk
                return 0;
            }
            else
            {
                printf("Enter a valid command..\n"); // print message for user to enter a valid command
            }
        }

        return 0;
    }
    else
    {                                                                                                                                       // too many or two few arguments, print error and exit
        printf("You entered too many or too few arguments; you should only enter the executable followed by the disk you wish to use..\n"); // print error
        exit(0);                                                                                                                            // exit program
    }
}

/*
    Creates a file with the name provided, allows the user to add content to that file, and adds an entry to the FAT
    Accepts a file name to search for within the FAT and a parent name to assign to the specific file in its FAT entry
*/
void createFile(char *filename, char *parent)
{
    if ((!filename) || (!parent)) { // be sure filename and parent args are 
        printf("Error: Either no filename name was specified or no parent directory was specified..try again!\n\n");
        return;
    }

    // be sure that filename has an ending extension like ".txt"
    if((strstr(filename, ".txt")) || strstr(filename, ".")) { // as long as the filename has a text extension or some extension 
        if (findParent(parent))
        {
            // find empty entry in the FAT to create the new file at
            int emptyIndex = findEmptyEntryFAT();
            #ifdef TESTING
            printf("Found an empty entry at row: %d\n", emptyIndex); // FOR TESTING
            #endif
            // find empty index in physical directory to store file contents in
            int emptyBlock = findEmptyBlock();
            #ifdef TESTING
            printf("Found an empty block at index = %d\n", emptyBlock);
            #endif
            // convert emptyBlock to a string and assign that to the appropriate column in the FAT entry when inserting entry
            char indexNumber[sizeof(int) * 4 + 1]; // make sure there's proper space for converting integer to string to be placed in FAT
            sprintf(indexNumber, "%d", emptyBlock);
            #ifdef TESTING
            printf("Index number as a string: %s\n", indexNumber);
            #endif
            // insert entry into FAT -
            insertEntry(emptyIndex, filename, "file", indexNumber, parent, getTime());
            // } else { // otherwise root directory does not exist - print error message
            //     printf("Was not able to create a directory as the root directory does not seem to exist..\n");
            // }

            // prompt user to enter file contents to store at empty physical dir index
        }
        else
        {
            printf("Parent for file you wish to create was not found! Try again!\n\n");
        }
    } else {
        printf("Please specify an extension for the file to be created..\n\n");
        return;
    }
}

/*
    Deletes a file with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to FREESPACE for future use)
    Accepts a file name to search for within the FAT
*/
void deleteFile(char *filename)
{
    printf("Deleting %s from file system..\n\n", filename);
    int i = 0;
    int j = 0;
    startingIndex = 0;
    disk_ptr = fopen("Drive2MB", "r+"); // open for reading/writing to file
    for (i = 0; i < 100; i++)
    {                                                                                     // go through FAT, if filename column is "." then that entry is empty
        #ifdef TESTING
        printf("filename was: %s and length was: %lu\n", filename, strlen(filename));     // FOR TESTING
        printf("file/dir names: %s and length was: %lu\n", FAT[i][0], strlen(FAT[i][0])); // FOR TESTING
        #endif
        char *temp = (char *)malloc(sizeof(char) * strlen(FAT[i][0]));
        temp = strdup(FAT[i][0]);      // makes handling the string name easier to manipulate
        #ifdef TESTING
        printf("TEMP IS: %s\n", temp); // FOR TESTING
        #endif
        //temp[strlen(temp)-1] = 0; // remove newline from end of string to compare if it's the same as teh file name
        startingIndex++; // increase the index which will be used to go to the line of the file entry in the FAT
        if (strcmp(temp, filename) == 0)
        { // check if the current row's filename is ".", indicating an empty entry
            // set all columns in that row to "." which means that row is not empty
            #ifdef TESTING
            printf("Attempting to delete entry: %s..\n", temp); // FOR TESTING
            printf("INDEX IS: %d\n", startingIndex);            // FOR TESTING
            #endif
            rewind(disk_ptr);                                   // rewind disk pointer to beginning of file
            char *newLine = (char *)malloc(sizeof(char) * 140);
            while (startingIndex >= 0)
            {                                              // while startingIndex is greater than OR equal to 0
                fgets(newLine, sizeof(newLine), disk_ptr); // get each new line from the disk file
                startingIndex--;                           // decrease the index
            }
            #ifdef TESTING
            printf("newline is: %s\n", newLine); // FOR TESTING making sure get right line
            #endif

            for (j = 0; j < 5; j++)
            {
                #ifdef TESTING
                printf("Deleting column in FAT from specified entry..\n");
                #endif
                //FAT[i][j] = (char*)realloc(FAT[i][j], sizeof(char)*100); // clear a space in memory for assigning string into table
                //fprintf(disk_ptr, ". "); // update virtual disk file
                strcpy(FAT[i][j], "."); // update data structure
                // update it's physical block allocation as well
            }
            break; // stop looping through FAT to find file
        }
    }
}

/*
    Adds the file by filename to the open_file_array, returns 1 on success and 0 on failure
    Accepts a file name to search for within the FAT to be added to the open_file_array
*/
int openFile(char *filename)
{
    // pseudocode in documentation
    return 1;
}

/*
    Removes the file by filename from the open_file_array, returns 1 on success and 0 on failure
    Accepts a file name to search for within the open_file_array to remove
*/
int closeFile(char *filename)
{
    // pseudocode in documentation
    return 1;
}

/*
    Creates a directory with the name provided and adds an entry to the FAT
    Accepts a directory name from the user to be used in the creation of that directory
*/
void createDirectory(char *dirname, char *parent)
{
    if (strcmp(dirname, "/") == 0) // if parent is root create the first entry
    {
        insertEntry(0, dirname, "dir", "0", "none", getTime());
    }
    else
    {
        // find empty entry in the FAT to create the new file at
        int emptyIndex = findEmptyEntryFAT();
        #ifdef TESTING
        printf("Found an empty entry at row: %d\n", emptyIndex); // FOR TESTING
        #endif
        // find empty index in physical directory to store file contents in
        int emptyBlock = findEmptyBlock();
        #ifdef TESTING
        printf("Found an empty block at index = %d\n", emptyBlock); // FOR TESTING
        #endif
        // convert emptyBlock to a string and assign that to the appropriate column in the FAT entry when inserting entry
        char indexNumber[sizeof(int) * 4 + 1];
        sprintf(indexNumber, "%d", emptyBlock);
        #ifdef TESTING
        printf("Index number as a string: %s\n", indexNumber); // FOR TESTING
        #endif
        // insert entry into FAT -
        insertEntry(emptyIndex, dirname, "dir", indexNumber, parent, getTime());

        // prompt user to enter file contents to store at empty physical dir index
    }
}

/*
    Deletes a directory with the name passed in, searches for the name in the FAT and sets all FAT columns for that entry to NULL (except the Name column which will get set to . for future use)
    Accepts a directory name to search for within the FAT
*/
void deleteDirectory(char *dirname)
{
    printf("Deleting %s directory from file system..\n\n", dirname);
    int i = 0;
    int j = 0;
    startingIndex = 0;
    disk_ptr = fopen("Drive2MB", "r+"); // open for reading/writing to file
    for (i = 0; i < 100; i++)
    { // go through FAT, if filename column is "." then that entry is empty
        #ifdef TESTING
        printf("filename was: %s and length was: %lu\n", dirname, strlen(dirname));
        printf("file/dir names: %s and length was: %lu\n", FAT[i][0], strlen(FAT[i][0]));
        #endif

        char *temp = (char *)malloc(sizeof(char) * strlen(FAT[i][0]));
        temp = strdup(FAT[i][0]); // makes handling the string name easier to manipulate

        #ifdef TESTING
        printf("TEMP IS: %s\n", temp); // FOR TESTING
        #endif
        //temp[strlen(temp)-1] = 0; // remove newline from end of string to compare if it's the same as teh file name
        startingIndex++; // increase the index which will be used to go to the line of the file entry in the FAT
        if (strcmp(temp, dirname) == 0)
        { // check if the current row's dirname is ".", indicating an empty entry
            // set all columns in that row to "." which means that row is not empty
            #ifdef TESTING
            printf("Attempting to delete entry: %s\n", temp);
            printf("INDEX IS: %d\n", startingIndex);
            #endif
            rewind(disk_ptr); // rewind disk pointer to beginning of file
            char *newLine = (char *)malloc(sizeof(char) * 140);
            while (startingIndex >= 0)
            {                                              // while startingIndex is greater than OR equal to 0
                fgets(newLine, sizeof(newLine), disk_ptr); // get each new line from the disk file
                startingIndex--;                           // decrease the index
            }
            #ifdef TESTING
            printf("newline is: %s\n", newLine); // FOR TESTING making sure get right line
            #endif

            for (j = 0; j < 5; j++)
            {
                #ifdef TESTING
                printf("Deleting column in FAT for specified entry..\n");
                #endif
                //FAT[i][j] = (char*)realloc(FAT[i][j], sizeof(char)*100); // clear a space in memory for assigning string into table
                //fprintf(disk_ptr, ". "); // update virtual disk file
                strcpy(FAT[i][j], ".");  // update data structure
                // update it's physical block allocation as well
            }
            break; // stop looping through FAT to find directory
        }
    }
}

/*
    Finds the file in the FAT, and thus in the physical directory and prints its contents to screen for the user
    Accepts a filename to 
*/
void readFile(char *filename)
{
    // pseudocode in README
}

/*
    Copies a file from your host OS to your file system
    Accepts a file name from your host OS to copy over to your file system, this name will also be the name of the file in my FAT
*/
void writeToFile(char *filename)
{
    // pseudocode in README
}

/*
    Prints the specified directories contents
*/
void printDirContent(char* dirname)
{
    if (!dirname) {
        printf("No directory name specified..try again!\n\n");
        return;
    }

    printf("Below are the contents of directory %s:\n", dirname);
    int i = 0;
    for (i = 0; i < 100; i++)
    { // loop through the FAT
        char *temp = (char *)malloc(sizeof(char) * strlen(FAT[i][3]));
        temp = strdup(FAT[i][3]); // makes handling the string name easier to manipulate
        char *tempType = (char *)malloc(sizeof(char) * strlen(FAT[i][1]));
        tempType = strdup(FAT[i][1]); // makes handling the string name easier to manipulate

        if(strcmp(temp, dirname) == 0) { // find every entry that has parent field equal to dirname and print them out to screen
            for (x = 0; x < 5; x++) // for each column
            {   
                if (x == 4) {
                    printf("%s", FAT[i][x]); // if column is date and time, don't print a pipe after it for readability
                } else {
                    printf("%s | ", FAT[i][x]); // otherwise print a pipe after for readability
                }
            }
            int j = 0;
            while(j != 1) {
                printf("\n");
                j++;
            }
        }
    }
    printf("\n");
}

/*
    Loops through the FAT printing each row, empty or filled
*/
void printDirHierarchy()
{
    printf("Printing directory hierarchy below:\n");
    printf("File Name | FileOrDir | Index Number | Parent | Date and Time\n");
    int i = 0;
    for (i = 0; i < 100; i++)
    { // loop through the FAT, print out each row to show
        for (x = 0; x < 5; x++)
        {
            if (x == 4) {
                printf("%s", FAT[i][x]); // if column is date and time, don't print a pipe after it for readability
            } else {
                printf("%s | ", FAT[i][x]); // otherwise print a pipe after for readability
            }
        }
        printf("\n"); // FOR TESTING
    }
}

/* Parse command read in from user, increment argc for each string in command */
char **parseCommand(char *command, char **argv, int *argc)
{
    int i = 0;
    *(argc) = 0; // make sure for each command, argc is set back to 0
    for (i = 0; i < 100; i++)
    {
        argv[i] = strtok_r(command, " \t\r\a", &command); // seperate each string by space and store that string in the respective index of params C-String array, nice thing about strtok_r is that it removes empty spaces all together and all other delimiters

        if (argv[i] == NULL)
            break;
        (*argc)++; // increment argc for every string in the command
    }
    argv[strlen(*argv) + 1] = NULL; // set first location in array after C-Strings to NULL
    return argv;                    // return argv array to be executed on
}

/* Function helpers */

/*
    Gets the current date and time for FAT entries
    Accepts no argument and returns a string that will contain the date and time
*/
char *getTime()
{
    time_t newTime = time(NULL);  // get the time
    char *time = ctime(&newTime); // get time in string
    time[strlen(time) - 1] = 0;   // remove newline character from the end of that time string for printing purposes - note that NULL character is now at the end
    return time;                  // use ctime() to get formatted date and time as a string
}

/*
    Finds a row at which the FAT is empty to be used in creating a new file
    Returns the index of a row to be used at which the FAT is empty
*/
int findEmptyEntryFAT()
{
    int i = 0;
    for (i = 0; i < 100; i++)
    { // go through FAT, if filename column is "." then that entry is empty
        if (strcmp(FAT[i][0], ".") == 0)
        {             // check if the current row's filename is ".", indicating an empty entry
            return i; // return index of empty row in FAT
        }
    }
    printf("Never found an empty entry in the FAT..\n"); // never found an index of an empty entry
    return -1;                                           // return -1 to indicate failure
}

/*
    Finds an empty index at which to store a file's contents in the physical directory
    Returns the index of a row to be used at which the FAT is empty
*/
int findEmptyBlock()
{
    int i = 0;
    for (i = 0; i < 100; i++)
    { // go through FAT, if filename column is "." then that entry is empty
        if (strcmp(physicalDir[i], ".") == 0)
        {             // check if the current row's filename is ".", indicating an empty entry
            return i; // return index of empty row in FAT
        }
    }
    printf("Never found an empty entry in the FAT..\n"); // never found an index of an empty entry
    return -1;                                           // return -1 to indicate failure
}

int findParent(char *parent)
{
    int i = 0;
    int j = 0;
    startingIndex = 0;
    #ifdef TESTING
    printf("Inside findParent()\n");
    #endif

    for (i = 0; i < 100; i++)
    { // go through FAT, if filename column is "." then that entry is empty

        char *temp = strdup(FAT[i][0]);     // makes handling the string name easier to manipulate
        char *tempType = strdup(FAT[i][1]); // makes handling the string name easier to manipulate

        #ifdef TESTING
        printf("TEMP IS: %s\n", temp);
        printf("PARENT IS: %s\n", parent);
        #endif
        // parent[strlen(parent)] = '';
        //temp[strlen(temp)-1] = 0; // remove newline from end of string to compare if it's the same as teh file name
        //tempType[strlen(tempType) - 1] = '\0';
        #ifdef TESTING
        printf("parent was: %s and length was: %lu\n", parent, strlen(parent));
        printf("dirname was: %s and length was: %lu\n", temp, strlen(temp));
        printf("dirType was: %s and length was: %lu\n", tempType, strlen(tempType));
        #endif

        startingIndex++; // increase the index which will be used to go to the line of the file entry in the FAT
        if ((strcmp(temp, parent) == 0) && (strcmp(tempType, "dir") == 0))
        { // check if a directory with the name specified exists and also check it is actually a directory type
            #ifdef TESTING
            printf("Dir: %s was found!\n", temp);
            #endif
            return 1; // parent directory was found
            break;
        }
    }
    return 0; // parent wasn't found in FAT so return 0
}

/*
    Inserts an entry with the appropriate fields into the FAT
*/
void insertEntry(int emptyIndex, char *name, char *fileOrDir, char *indexNumber, char *parent, char *timestamp)
{
    // malloc space for the new entry
    FAT[emptyIndex][0] = (char *)malloc(sizeof(char) * 300); // clear a space in memory for assigning string into table
    FAT[emptyIndex][1] = (char *)malloc(sizeof(char) * 300); // clear a space in memory for assigning string into table
    FAT[emptyIndex][2] = (char *)malloc(sizeof(char) * 300); // clear a space in memory for assigning string into table
    FAT[emptyIndex][3] = (char *)malloc(sizeof(char) * 300); // clear a space in memory for assigning string into table
    FAT[emptyIndex][4] = (char *)malloc(sizeof(char) * 300); // clear a space in memory for assigning string into table

    // assign all values to appropriate spots
    strcpy(FAT[emptyIndex][0], name);
    strcpy(FAT[emptyIndex][1], fileOrDir);
    strcpy(FAT[emptyIndex][2], indexNumber);
    strcpy(FAT[emptyIndex][3], parent);
    strcpy(FAT[emptyIndex][4], timestamp);

    // print to screen for user to see their new entry to the FAT
    printf("New file/dir created: ");
    printf("%s | ", FAT[emptyIndex][0]);
    printf("%s | ", FAT[emptyIndex][1]);
    printf("%s | ", FAT[emptyIndex][2]);
    printf("%s | ", FAT[emptyIndex][3]);
    printf("%s\n\n", FAT[emptyIndex][4]);

    // write that info to the virtual disk file where FAT is shown with separators for each entry
    if (startingIndex == 0)
    {
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][0]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][1]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][2]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][3]);
        fprintf(disk_ptr, "%s", FAT[emptyIndex][4]);
        startingIndex++; // increment startingIndex
    }
    else
    {
        fprintf(disk_ptr, "\n%s|", FAT[emptyIndex][0]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][1]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][2]);
        fprintf(disk_ptr, "%s|", FAT[emptyIndex][3]);
        fprintf(disk_ptr, "%s", FAT[emptyIndex][4]);
    }
}