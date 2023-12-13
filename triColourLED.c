#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "triColourLED.h"


static void writeToFile(const char* fileName, const char* value)
{   //open file
	FILE *pFile = fopen(fileName, "w");
    //Write specified value to the file
	fprintf(pFile, "%s", value);
    //close file
	fclose(pFile);
}

void turnOnLED(char* LED){
    //turn on blue LED
    if(strcmp(LED, "blue") == 0){
        writeToFile(BLUE_LED_DIRECTION, "out");
        writeToFile(BLUE_LED_VALUE, "0");
    }
    //turn on green LED
    if(strcmp(LED, "green") == 0){
        writeToFile(GREEN_LED_DIRECTION, "out");
        writeToFile(GREEN_LED_VALUE, "0");
    }
    //turn on red LED
    if (strcmp(LED, "red") == 0){
        writeToFile(RED_LED_DIRECTION, "out");
        writeToFile(RED_LED_VALUE, "0");
    }

}

void turnOffLED(char* LED){
    //Turn off blue LED
    if(strcmp(LED, "blue") == 0){
        writeToFile(BLUE_LED_VALUE, "1");
    }
    //turn off green LED
    if(strcmp(LED, "green") == 0){
        writeToFile(GREEN_LED_VALUE, "1");
    }
    //turn off red LED
    if (strcmp(LED, "red") == 0){
        writeToFile(RED_LED_VALUE, "1");
    }

}
