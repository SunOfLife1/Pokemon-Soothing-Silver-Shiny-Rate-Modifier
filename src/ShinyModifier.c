/*
 * AUTHOR      : SunOfLife1
 * USAGE       : Drag and drop a ROM onto the .exe
 * DESCRIPTION : This program changes the shiny rate in an inputted Pokemon Soothing Silver ROM by simply modifying
 *               the hex value at the correct offset.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "CLIHelper.h"

// If debug macro is 1, program will print more details
#define DEBUG 0
// If simplified macro is 1, program will default to max shiny rate and basic output filename
#define SIMPLIFIED 0
// Offset of the shiny value in Soothing Silver v1.3.2
#define OFFSET 0x558EB

bool hasExtension(const char* filename, const char* ext)
{
    if (!filename || !ext) return false;

    const char* actualExt = strrchr(filename, '.');
    if (!actualExt) return false;

    return !strncmp(actualExt, ext, strlen(ext));
}

bool fileExists(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) return false;

    fclose(fp);
    return true;
}

int main(int argc, char** argv)
{
    // If there is no command line input, print a usage message and exit
    if (argc <= 1)
    {
        printf("Drag and drop your ROM onto this .exe file and try again.\n");
        return enterToClose();
    }

    // Try to open the ROM for read only
    FILE* rom = fopen(argv[1], "rb");
    DEBUG_PRINT("\nDEBUG - Input ROM location: %s\n", argv[1]);

    // If opening the file somehow doesn't work, exit
    if (rom == NULL)
    {
        printf("The inputted location is invalid. I'm honestly not sure how you got this error.\n");
        return enterToClose();
    }

    char outFilename[256];
    if (!SIMPLIFIED)
    {
        // Ask the user for the output filename
        while (strlen(outFilename) == 0)
        {
            printf("Enter the name of the output file.\n> ");
            fgets(outFilename, 256, stdin);
            outFilename[strcspn(outFilename, "\n")] = 0; // remove end of string newline
        }
    }
    else
    {
        // Create the output filename
        char* ext = strrchr(argv[1], '.');
        *ext = '\0'; // separate file name and extension
        ext++;
        snprintf(outFilename, 256, "%s (Max Shiny Odds).%s", argv[1], ext);
    }

    // Check that file already exists
    if (fileExists(outFilename) && !promptToContinue("Output file exists already."))
        return enterToClose();

    // Check that file extension is correct
    if (!hasExtension(outFilename, ".nds") && !promptToContinue("Output filename does not end in \".nds\"."))
        return enterToClose();

    if (DEBUG)
        printf("\nDEBUG - Output ROM location: %s\n", outFilename);

    // Explain how the shiny rates work
    if (!SIMPLIFIED)
    {
        printf("\nThe shiny rate is XX/65536. In base HGSS, XX is 8. In Soothing Silver, XX is 32.\n");
        printf("The highest possible value for XX is 255, resulting in a shiny rate of about 1/257.\n\n");
    }

    int newShinyValue = 255;
    if (!SIMPLIFIED)
    {
        // Ask the user for a new shiny value
        do
        {
            printf("What would you like XX to be? (must be at least 0 and at most 255)\n> ");
            char input[256];
            fgets(input, 256, stdin);
            sscanf(input, "%d", &newShinyValue);

            if (DEBUG)
                printf("\nDEBUG - Current newShinyValue: %d\n", newShinyValue);
        } while (newShinyValue < 0 || newShinyValue > 255);
    }

    // Calculate the size of the ROM file
    fseek(rom, 0, SEEK_END);
    long filesize = ftell(rom);

    // Copy the contents of the ROM file to a new file
    unsigned char* filecopy = malloc(filesize);
    fseek(rom, 0, SEEK_SET);
    fread(filecopy, 1, filesize, rom);

    if (DEBUG)
        printf("\nDEBUG - Old shiny value: 0x%02X\n", filecopy[OFFSET]);

    // Modify the shiny value in the new file
    filecopy[OFFSET] = newShinyValue;

    if (DEBUG)
        printf("DEBUG - New shiny value: 0x%02X\n", filecopy[OFFSET]);

    // Save the contents of the new file to the actual file
    FILE* newRom = fopen(outFilename, "wb");
    long result = fwrite(filecopy, 1, filesize, newRom);

    if (DEBUG)
    {
        printf("\nDEBUG - Bytes read: %ld\n", filesize);
        printf("DEBUG - Bytes wrote: %ld\n", result);
    }

    // Print whether the write was successful or not
    if (result == filesize)
    {
        if (!SIMPLIFIED)
            printf("\nFile saved!\n");
        else
        {
            printf("\nNew Shiny Rate: 1/257\n");
            printf("\nFile saved to \"%s\"\n", outFilename);
        }
    }
    else
    {
        if (newRom == NULL)
            printf("\nError occured while creating \"%s\". Try again with a different filename.", outFilename);
        else
            printf("\nError occured while saving file.\n");
    }

    // Close files, free allocated memory
    fclose(rom);
    fclose(newRom);
    free(filecopy);

    return enterToClose();
}
