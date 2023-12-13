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
#include "snakeGame.h"
#include "joysticktest.h"
#include "joysticktest.c"
#include "triColourLED.h"
#include "triColourLED.c"
#include "gameButtons.h"
#include "gameButtons.c"
#include "snakeLogic.h"
#include "snakeLogic.c"
#include <stdbool.h>

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS0 0x70
#define I2C_DEVICE_ADDRESS1 0x71
#define I2C_DEVICE_ADDRESS2 0x72
#define I2C_DEVICE_ADDRESS3 0x73

char pin17Config_i2c[] = "config-pin p9_17 i2c";
char pin18Config_i2c[] = "config-pin p9_18 i2c";
int bestScore = 0;
int score = 0;


static void runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        }
        // Get the exit code from the pipe; non-zero is an error:
        int exitCode = WEXITSTATUS(pclose(pipe));
        if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

static void Logical_to_PhysicalFrame(DigitMatrix *digit) {
    for(int i = 0; i < 8; i++) {
        RightShiftDigits(&digit->rows[i], 1);
    }
}


//Forms 8x8 bit matrix with one digit on the left and one on the right and writes the matrix
void write2Digits(int left_digit, int right_digit, int i2cFileDesc) {
   
    DigitMatrix integerNum;

    //Puts together both digit bits in the same 8x8 bit matrix
    for(int i = 0; i < 8; i++) {
        integerNum.rows[i] = ((digits[left_digit].rows[i] << 4) | (digits[right_digit].rows[i])) << 1;
    }

    Logical_to_PhysicalFrame(&integerNum);
    
    //Write to the whole 8x8 LED matrix, except last row since digits are 3x7 bits
    for(int i = 1; i < 8; i++) {
        //Register Address of ROW1 (TOP) is 0x0E and decreases by 2 as row goes down
        //i.e ROW2 = 0x0C
        writeI2cReg(i2cFileDesc, REG_ROW_BASE - 2*(i-1), integerNum.rows[i-1]);
    }
}


void displayInteger(int number, int i2cFileDesc) {
    int tens = number/10; //Left digit
    int ones = number%10; //Right digit

    write2Digits(tens, ones, i2cFileDesc);

    //Display a space in the last row
    writeI2cReg(i2cFileDesc, REG_ROW_BASE - 2*(8-1), 0b00000000);
}

void gamelooop(int speed){
    int i2cFileDesc[4];

	i2cFileDesc[0] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS0);
    initI2cDisplay(i2cFileDesc[0]);

    i2cFileDesc[1] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS1);
    initI2cDisplay(i2cFileDesc[1]);

    i2cFileDesc[2] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS2);
    initI2cDisplay(i2cFileDesc[2]);

    i2cFileDesc[3] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS3);
    initI2cDisplay(i2cFileDesc[3]);

    float xPosition, yPosition;
       
    int snakeLength = SNAKE_LENGTH;
    int direction = 1; // Initial direction (1: Right, 2: Left, 3: Up, 4: Down)

    SnakeSegment snake[256]; //Have to make it large enough, modify it later

    bool gameOver = false;

    snake[0].x = 3;
    snake[0].y = 3;

    snake[1].x = 2;
    snake[1].y = 3;

    snake[2].x = 2;
    snake[2].y = 4;

  

    turnOffLED("green");
    turnOffLED("red");
    turnOffLED("blue");

    while (!gameOver) {

            //Stop the round/game when user presses red button
            if(checkBUttonPress(RED_BUTTON_VALUE) == 1){
                break;
            }
            else{

                xPosition = joyStickReadingX();
                yPosition = joyStickReadingY();

                updateDirection(xPosition, yPosition, &direction);
                switch (direction) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    default:
                        break;
                }

                moveSnake(snake, snakeLength, direction);

                //If snake is eating itself, game is over
                isEatingItself(snake, snakeLength, &gameOver);


                //Terminate loop here if game is over? or let it finish the loop?
                // Yash - Termination woud prolly be better
                
                if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                    //Increment score when fruit is eaten
                    runCommand("aplay eat.wav");
                    score = score + 1;

                    if (score > bestScore || bestScore == 0) {
                        bestScore = score;
                    }
                    printf("FRUIT EATEN!, current score is: %d\n", score);
                    
                    //Turn the LED green when snake eats the fruit
                    turnOnLED("green");
                    sleepForMs(200);
                    turnOffLED("green");

                    //Generate a new fruit position
                    generateFruitPosition(snake, snakeLength);

                    //Increase snake length
                    snakeLength++;
                }
                else{
                    displaySnake(snakeLength, i2cFileDesc, snake);
                    //printf("Displayed\n");

                    sleepForMs(speed); // Adjust the delay to control the speed of the snake movement
                }
            } 

        }
    runCommand("aplay gameover.wav");
    turnOnLED("red");
    close(i2cFileDesc[0]);
    close(i2cFileDesc[1]);
    close(i2cFileDesc[2]);
    close(i2cFileDesc[3]);
}

int main()
{
    int i2cFileDesc[4];

	i2cFileDesc[0] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS0);
    initI2cDisplay(i2cFileDesc[0]);

    i2cFileDesc[1] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS1);
    initI2cDisplay(i2cFileDesc[1]);

    i2cFileDesc[2] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS2);
    initI2cDisplay(i2cFileDesc[2]);

    i2cFileDesc[3] = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS3);
    initI2cDisplay(i2cFileDesc[3]);

    //snake speed
    int snakeSpeed = 300;

    //Configure pins for i2c use
    runCommand(pin17Config_i2c);
    runCommand(pin18Config_i2c);

    //configure the game buttons
    configureGameButtons(START_BUTTON);
    configureGameButtons(STOP_BUTTON);
    configureGameButtons(SPEED_BUTTON);

    //Make sure TriColour LED off
    turnOffLED("red");
    turnOffLED("blue");

    printf("*******************************************\n");
    printf("Welcome to the SNAKE GAME!\n");
    printf("To play please press the GREEN button\n");
    printf("To exit the game press the RED button\n");
    printf("*******************************************\n");

    //Ask the user if the want to play the game or exit
    while(1){
        if (checkBUttonPress(GREEN_BUTTON_VALUE) == 1){
            sleep(1);
            printf("*******************************************\n");
            printf("You have chosen to play the snake game!!!\n");
            printf("If you would like to make the snake faster, click the Blue button!\n");
            printf("Each blue button press will make the snake 50ms faster!\n");
            printf("Once you are happy with the speed, press the Green button again to proceed with the game!\n");

            while(1){
                if(checkBUttonPress(BLUE_BUTTON_VALUE) == 1){
                    turnOnLED("blue");
                    sleep(1);
                    snakeSpeed = snakeSpeed - 50;
                    switch (snakeSpeed)
                    {
                    case 300:
                        printf("LEVEL 1 CHOSEN...slowest\n");
                        break;
                    case 250:
                        printf("LEVEL 2 CHOSEN...getting there\n");
                        break;
                    case 200:
                        printf("LEVEL 3 CHOSEN...getting closer ;)\n");
                        break;
                    case 150:
                        printf("LEVEL 4 CHOSEN...lightning fast\n");
                        break;
                    case 100:
                        printf("LEVEL 5 CHOSEN...you are insane\n");
                        break;
                    case 50:
                        printf("LEVEL 6 CHOSEN...FASTEST SPEED CHOSEN!!!!\n");
                        break;
                    case 0:
                        printf("Not allowed...snake speed set back to level 1\n");
                        snakeSpeed = 300;
                        break;
                    default:
                        break;
                    }
                    turnOffLED("blue");
                }
                if(checkBUttonPress(GREEN_BUTTON_VALUE) == 1){
                    break;
                }
            }
            break;
        }
        if (checkBUttonPress(RED_BUTTON_VALUE) == 1){
            return 0;
        }
        else{
            sleepForMs(0.1);
        }
    }
    while(1){
        gamelooop(snakeSpeed);
        sleepForMs(1000);
        printf("****************************************\n");
        printf("ROUND OVER!\n Your score was: %d\n", score);
        displayInteger(score, i2cFileDesc[2]);
        displayInteger(bestScore, i2cFileDesc[3]);

        turnOnLED("green");
        
        //turnOnLED("red");
        printf("Current best score is: %d\n", bestScore);

        score = 0;
        printf("Press GREEN BUtton if you would like to play another round\n");
        printf("Press RED button if you would like to exit the game\n");
        printf("****************************************\n");
        while(1){
            if (checkBUttonPress(GREEN_BUTTON_VALUE) == 1){
                break;
            }
            else if (checkBUttonPress(RED_BUTTON_VALUE) == 1){
                turnOnLED("blue");
                turnOffLED("green");

                sleep(2);
                turnOffLED("blue");
                turnOffLED("red");
                clearDisplay(i2cFileDesc[0]);
                clearDisplay(i2cFileDesc[1]);
                clearDisplay(i2cFileDesc[2]);
                clearDisplay(i2cFileDesc[3]);

                return 0;
            }
            else{
                sleepForMs(0.01);
            }
        }
    }

	return 0;
}