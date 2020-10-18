#include <time.h>
#include "myshell.h"
#include <stdio.h>

void getTime(void)
{
    time_t raw;
    struct tm *timeInfo;
    char formatBuffer[30];

    time(&raw);
    timeInfo = localtime(&raw);
    strftime(formatBuffer,30,"%d/%m %H:%M",timeInfo);
    printf("[%s]", formatBuffer);
}