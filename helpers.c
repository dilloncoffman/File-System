#include "fs.h"

/* Function helpers */

/*
    Function to get the current date and time for FAT entries
    Accepts no argument and returns a string that will contain the date and time
*/
char* getTime() {
    time_t newTime = time(NULL); // get the time
    return ctime(&newTime); // use ctime() to get formatted date and time as a string
}