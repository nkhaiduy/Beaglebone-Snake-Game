#ifndef SNAKELOGIC_H
#define SNAKELOGIC_H
#define I2C_DEVICE_ADDRESS 0x70
#define REG_SYSTEM_SETUP 0x21 
#define REG_DISPLAY_SETUP 0x81
#define REG_ROW_BASE 0x0E

#define SNAKE_LENGTH 3 // Number of digits in the snake

typedef struct {
    int x;
    int y;
} SnakeSegment;

typedef struct{
    int x;
    int y;
} Fruit;

//Function to check the direction of the joystick
void updateDirection(float xPosition, float yPosition, int *direction);

//Function to generate a fruit at a random position on the LED matrix but not overlaping with snake.
void generateFruitPosition(SnakeSegment *snake, int snakeLength);

//Function to move the snake based on the jostick direction
void moveSnake(SnakeSegment *snake, int snakeLength, int direction);

//Function to check if the snake has eaten itself. If yes, sets gameOver to True to indicate round end.
void isEatingItself(SnakeSegment snake[], int snakeLength, bool *gameOver);

//Function to right shift row
static unsigned char RightShift(unsigned char row, int shiftBy);

//Function to right shift digits
static void RightShiftDigits(unsigned char *row, int shiftBy);

//Function to display snake onto 16x16 LED matrix
void displaySnake(int snakeLength, int i2cFileDesc[], SnakeSegment snake[]);

#endif