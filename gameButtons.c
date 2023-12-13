#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "gameButtons.h"

char value_reading[1024];

static void configureGameButtons(char* button){
    //Configue start (green button)
    if (strcmp(button, START_BUTTON) == 0){
        writeToFile(GREEN_BUTTON_DIRECTION, "in");
    }

    //Configure stop (red button)
    if (strcmp(button, STOP_BUTTON) == 0){
        writeToFile(RED_BUTTON_DIRECTION, "in");
    }

    //Configure blue (speed button)
    if(strcmp(button, SPEED_BUTTON) == 0){
        writeToFile(BLUE_BUTTON_DIRECTION, "in");
    }
}

int checkBUttonPress(char* buttonValue){
    //Opens specified button value file for reading
    FILE *pFile = fopen(buttonValue, "r");
                if (pFile == NULL) {
                    printf("ERROR: Unable to open file (%s) for read\n", buttonValue);
                    exit(-1);
                }
        fgets(value_reading, 1024, pFile);

    //Checks if the button has been pressed, if it has return 1.
    if (strstr(value_reading, "1")){
        fclose(pFile);
        return 1;
    }
    //If the button has not been pressed return 0.
    else{
        fclose(pFile);
        return 0;
    }
}