#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_FILE_VOLTAGE2 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define A2D_FILE_VOLTAGE3 "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"
#define A2D_VOLTAGE_REV_V 1.8
#define A2D_MAX_READING 4095


double joyStickReadingX()
{

    FILE *f2 = fopen(A2D_FILE_VOLTAGE2, "r");
    if (!f2)
    {
        printf("ERROR: Unable to open voltage input file. Cape loaded? \n");
        printf("    Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    sleepForMs(5);
    float a2dReadingX = 0;
    float itemsReadX = fscanf(f2, "%f", &a2dReadingX); //Read the xvalue file
    if (itemsReadX <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file. \n");
        exit(-1);
    }
    // close file
    sleepForMs(10);
    fclose(f2);

    //Retun the x-value reading
    return a2dReadingX;
}

double joyStickReadingY()
{
    FILE *f3 = fopen(A2D_FILE_VOLTAGE3, "r");
    if (!f3)
    {
        printf("ERROR: Unable to open voltage input file. Cape loaded? \n");
        printf("    Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    sleepForMs(5);
    float a2dReadingY = 0;
    float itemsReadY = fscanf(f3, "%f", &a2dReadingY); //Read the yvalue file
    if (itemsReadY <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file. \n");
        exit(-1);
    }

    //close file
    sleepForMs(10);
    fclose(f3);

    //return the yvalue reading
    return a2dReadingY;
}

int getVoltage0Reading()
{
    FILE *f = fopen(A2D_FILE_VOLTAGE0, "r");
    if (!f)
    {
        printf("ERROR: Unable to open voltage input file. Cape loaded? \n");
        printf("    Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    //get reading
    float a2dReading = 0;
    float itemsRead = fscanf(f, "%f", &a2dReading);
    if (itemsRead <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file. \n");
        exit(-1);
    }

    //close file
    fclose(f);

    //return voltage reading
    return a2dReading;
}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}