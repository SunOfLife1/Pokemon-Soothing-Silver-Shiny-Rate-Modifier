/*
 * AUTHOR      : SunOfLife1
 * USAGE       : Drag and drop a ROM onto the .exe
 * DESCRIPTION : This program changes the shiny rate in an inputted Pokemon Soothing Silver ROM by simply modifying
 *                    the hex value at the correct offset.
 */

#include <stdio.h>
#include <stdlib.h>

// If debug macro is 1, program will print more details
#define DEBUG 1

FILE *openROM(char *filename)
{
    // Open file for read and write
    FILE *fp = fopen(filename, "r+");

    if (DEBUG)
    {
        printf("DEBUG - Inputted location: %s\n", filename);
        printf("DEBUG - File pointer: %p\n\n", fp);
    }

    return fp;
}

int main(int argc, char **argv)
{
    // If there is no command line input, print a usage message and exit
    if (argc <= 2)
    {
        printf("Please drag and drop your ROM onto this .exe file and try again.\n\n");
        printf("Press ENTER to close this window.\n");
        getchar();
        return 0;
    }

    // If there is command line input, try to open the ROM
    FILE *rom = openROM(argv[1]);

    // Close the ROM file
    fclose(rom);

    return 0;
}
