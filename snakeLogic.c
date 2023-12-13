#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "snakeLogic.h"

Fruit fruit;

//Initializes I2C device
int initI2cBus(char* bus, int address) {
    int i2cFileDesc = open(bus, O_RDWR);
    if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if(result < 0) {
        perror("Unable to set I2C device to slave address.");
		exit(-1);
    }

    return i2cFileDesc;
}

//Writes to an I2C device's register
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;

    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("Unable to write i2c register");
		exit(-1);
    }
}

//Initialize the display by writing commands
void initI2cDisplay(int i2cFileDesc) {
    writeI2cReg(i2cFileDesc, REG_SYSTEM_SETUP, 0x00); //To turn on LED matrix
	writeI2cReg(i2cFileDesc, REG_DISPLAY_SETUP, 0x00);  //To turn on display
}

//Reads from an I2C device's register
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr) {
    // To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

void clearDisplay(int i2cFileDesc) {
    for (int row = 1; row <= 8; row++) {
        writeI2cReg(i2cFileDesc, REG_ROW_BASE - 2 * (row - 1), 0b00000000);
    }
}


void updateDirection(float xPosition, float yPosition, int *direction) {

    // Deadzone, in case of minor shifts in the joystick
    int deadzone = 400;

    // Base line values for joystick in the resting state.
    int baselineXposition = 2077;
    int baselineYposition = 1900;

    if (*direction !=3 && xPosition > (baselineXposition + deadzone)) {
        *direction = 4; // Left
    } else if (*direction !=4 && xPosition < (baselineXposition - deadzone)) {
        *direction = 3; // Right
    } else if (*direction !=1 && yPosition > (baselineYposition + deadzone)) {
        *direction = 2; // Up
    } else if (*direction !=2 && yPosition < (baselineYposition - deadzone)) {
        *direction = 1; // Down
    }
}


void generateFruitPosition(SnakeSegment *snake, int snakeLength) {
    //Random genetration of fruit
    srand(time(NULL)); 
    while (1) {
        fruit.x = rand() % 16; //Generate random x-coordinate (0 to 15)
        fruit.y = rand() % 16; //Generate random y-coordinate (0 to 15)

        //To make sure the fruit does not overlap with the snake
        int overlap = 0;
        for (int i = 0; i < snakeLength; i++) {
            if (fruit.x == snake[i].x && fruit.y == snake[i].y) {
                overlap = 1;
                break;
            }
        }
        if (!overlap) {
            break;
        }
    }
}

void moveSnake(SnakeSegment *snake, int snakeLength, int direction) {
    // Move the rest of the snake
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    // Move the snake's head based on the current direction
    switch (direction) {
        case 1: // Right
            snake[0].x = (snake[0].x + 1) % 16;
            break;
        case 2: // Left
            snake[0].x = (snake[0].x - 1 + 16) % 16; // Adding 8 to handle modulo of negative numbers
            break;
        case 3: // Up
            snake[0].y = (snake[0].y - 1 + 16) % 16; // Adding 8 to handle modulo of negative numbers
            break;
        case 4: // Down
            snake[0].y = (snake[0].y + 1) % 16;
            break;
    }
}

//Checks if the snake is eating (crashes with) any part of its body
void isEatingItself(SnakeSegment snake[], int snakeLength, bool *gameOver){
    for(int i = 1; i < snakeLength; i++){
        if((snake[0].x == snake[i].x) && (snake[0].y == snake[i].y)) {
            *gameOver = true;
            printf("GAME OVER!\n");
        }
    }
}

static unsigned char RightShift(unsigned char row, int shiftBy) {
    int row_size = 8;
    row = (row >> shiftBy) | (row << (row_size - shiftBy));
    return row;
}

static void RightShiftDigits(unsigned char *row, int shiftBy) {
    int row_size = 8;
    *row = (*row >> shiftBy) | (*row << (row_size - shiftBy));
}

void displaySnake(int snakeLength, int i2cFileDesc[], SnakeSegment snake[]) {
    // Clear the display before drawing the snake
    clearDisplay(i2cFileDesc[0]);
    clearDisplay(i2cFileDesc[1]);
    clearDisplay(i2cFileDesc[2]);
    clearDisplay(i2cFileDesc[3]);

    // Display the fruit
    unsigned char fruitRow = 1 << (7 - (fruit.x % 8));
    fruitRow = RightShift(fruitRow, 1);
    if(fruit.x >= 0 && fruit.x <= 7) { //Left Half
        if(fruit.y >= 0 && fruit.y <= 7) { //Upper Half
            writeI2cReg(i2cFileDesc[0], REG_ROW_BASE - (2 * (fruit.y % 8)), fruitRow);

        } 
        if(fruit.y >= 8 && fruit.y <= 15) { //Lower Half
            writeI2cReg(i2cFileDesc[3], REG_ROW_BASE - (2 * (fruit.y % 8)), fruitRow);
        }
    }

    if(fruit.x >= 8 && fruit.x <= 15) { //Right Half
        if(fruit.y >= 0 && fruit.y <= 7) { //Upper Half
            writeI2cReg(i2cFileDesc[1], REG_ROW_BASE - (2 * (fruit.y % 8)), fruitRow);
        }
        if(fruit.y >= 8 && fruit.y <= 15) { //Lower Half
            writeI2cReg(i2cFileDesc[2], REG_ROW_BASE - (2 * (fruit.y % 8)), fruitRow);
        }
    }


    // Display each segment of the snake
    for (int i = 0; i < snakeLength; i++) {
        int displayCol = snake[i].x;
        unsigned char snakeRow = 1 << (7 - (displayCol % 8));
        snakeRow = RightShift(snakeRow, 1);

        if(snake[i].x >= 0 && snake[i].x <= 7) { //Left Half
            if(snake[i].y >= 0 && snake[i].y <= 7) { //Upper Half
            // Read the current row content before updating
            unsigned char currentRow = readI2cReg(i2cFileDesc[0], REG_ROW_BASE - 2 * (snake[i].y % 8));
            // Combine the snakeRow with the existing row content
            unsigned char updatedRow = currentRow | snakeRow;
            writeI2cReg(i2cFileDesc[0], REG_ROW_BASE - (2 * (snake[i].y % 8)), updatedRow);
            } 
            if(snake[i].y >= 8 && snake[i].y <= 15) { //Lower Half
             unsigned char currentRow = readI2cReg(i2cFileDesc[3], REG_ROW_BASE - 2 * (snake[i].y % 8));
            // Combine the snakeRow with the existing row content
            unsigned char updatedRow = currentRow | snakeRow;
            writeI2cReg(i2cFileDesc[3], REG_ROW_BASE - (2 * (snake[i].y % 8)), updatedRow);
            }
        }

        if(snake[i].x >= 8 && snake[i].x <= 15) { //Right Half
            if(snake[i].y >= 0 && snake[i].y <= 7) { //Upper Half
                unsigned char currentRow = readI2cReg(i2cFileDesc[1], REG_ROW_BASE - 2 * (snake[i].y % 8));
                unsigned char updatedRow = currentRow | snakeRow;
                writeI2cReg(i2cFileDesc[1], REG_ROW_BASE - (2 * (snake[i].y % 8)), updatedRow);
            }
            if(snake[i].y >= 8 && snake[i].y <= 15) { //Lower Half
                unsigned char currentRow = readI2cReg(i2cFileDesc[2], REG_ROW_BASE - 2 * (snake[i].y % 8));
                unsigned char updatedRow = currentRow | snakeRow;
                writeI2cReg(i2cFileDesc[2], REG_ROW_BASE - (2 * (snake[i].y % 8)), updatedRow);
            }
        }
    }
}
