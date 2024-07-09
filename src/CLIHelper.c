/*
 * AUTHOR      : SunOfLife1
 * DESCRIPTION : This file contains functions related to the command-line interface
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "CLIHelper.h"

int enterToClose()
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
        if (!strncasecmp(input, "N", 1))
            return false;
    }
}
