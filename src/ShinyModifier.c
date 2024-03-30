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

// If debug macro is 1, program will print more details
#define DEBUG 0
// Offset of the shiny value in Soothing Silver v1.3.2
#define OFFSET 0x558EB

bool hasExtension(const char *filename, const char *ext)
{
    if (!filename || !ext)
        return false;

    const char *actualExt = strrchr(filename, '.');
    if (!actualExt)
        return false;

    if (DEBUG)
    {
        printf("\nDEBUG - filename = %s\n", filename);
        printf("DEBUG - ext = %s\n", ext);
        printf("DEBUG - actualExt = %s\n", actualExt);
    }

    return !strncmp(actualExt, ext, strlen(ext));
}

bool fileExists(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return false;

    fclose(fp);
    return true;
}

int printCloseWindow()
{
    printf("\nPress ENTER to close this window.\n");
    getchar();
    return 0;
}

bool promptToContinue(const char *msg)
{
    printf("\n%s ", msg);
    while (true)
    {
        printf("Continue anyway? (Y/N)\n> ");

        char input[256];
        fgets(input, 256, stdin);

        if (!strncasecmp(input, "Y", 1))
            return true;
        else if (!strncasecmp(input, "N", 1))
            return false;
    }
}

int main(int argc, char **argv)
{
    // If there is no command line input, print a usage message and exit
    if (argc <= 1)
    {
        printf("Drag and drop your ROM onto this .exe file and try again.\n");
        printCloseWindow();
        return 0;
    }

    // Try to open the ROM for read only
    FILE *rom = fopen(argv[1], "rb");

    // If opening the file somehow doesn't work, exit
    if (rom == NULL)
    {
        printf("The inputted location is invalid. I'm honestly not sure how you got this error.\n");
        return printCloseWindow();
    }

    // Ask the user for the output filename
    char outFilename[256];
    while (strlen(outFilename) == 0)
    {
        printf("Enter the name of the output file.\n> ");
        fgets(outFilename, 256, stdin);
        outFilename[strcspn(outFilename, "\n")] = 0; // remove end of string newline
    }

    // Check that file already exists
    if (fileExists(outFilename) && !promptToContinue("Output file exists already."))
        return printCloseWindow();

    // Check that file extension is correct
    if (!hasExtension(outFilename, ".nds") && !promptToContinue("Output filename does not end in \".nds\"."))
        return printCloseWindow();

    if (DEBUG)
    {
        printf("\nDEBUG - Input ROM location: %s\n", argv[1]);
        printf("DEBUG - Output ROM location: %s\n", outFilename);
    }

    // Explain how the shiny rates work
    printf("\nThe shiny rate is XX/65536. In base HGSS, XX is 8. In Soothing Silver, XX is 32.\n");
    printf("The highest possible value for XX is 255, resulting in a shiny rate of about 1/257.\n\n");

    // Ask the user for a new shiny value
    int newShinyValue = -1;
    while (newShinyValue < 0 || newShinyValue > 255)
    {
        printf("What would you like XX to be? (must be at least 0 and at most 255)\n> ");
        char input[256];
        fgets(input, 256, stdin);
        sscanf(input, "%d", &newShinyValue);

        if (DEBUG)
            printf("\nDEBUG - Current newShinyValue: %d\n", newShinyValue);
    }

    // Calculate the size of the ROM file
    fseek(rom, 0, SEEK_END);
    long filesize = ftell(rom);

    // Copy the contents of the ROM file to a new file
    unsigned char *filecopy = malloc(filesize);
    fseek(rom, 0, SEEK_SET);
    fread(filecopy, 1, filesize, rom);

    if (DEBUG)
        printf("\nDEBUG - Old shiny value: 0x%02X\n", filecopy[OFFSET]);

    // Modify the shiny value in the new file
    filecopy[OFFSET] = newShinyValue;

    if (DEBUG)
        printf("DEBUG - New shiny value: 0x%02X\n", filecopy[OFFSET]);

    // Save the contents of the new file to the actual file
    FILE *newRom = fopen(outFilename, "wb");
    long result = fwrite(filecopy, 1, filesize, newRom);

    if (DEBUG)
    {
        printf("\nDEBUG - Bytes read: %ld\n", filesize);
        printf("DEBUG - Bytes wrote: %ld\n", result);
    }

    // Print whether the write was successful or not
    if (result == filesize)
    {
        printf("\nFile saved!\n");
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

    return printCloseWindow();
}
