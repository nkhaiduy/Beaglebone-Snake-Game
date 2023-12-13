#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x70
#define REG_SYSTEM_SETUP 0x21 
#define REG_DISPLAY_SETUP 0x81
#define REG_ROW_BASE 0x0E

typedef struct {
    unsigned char rows[7];
} DigitMatrix;

//Logical Frame
DigitMatrix digits[10] = {
    {{0b111, 0b101, 0b101, 0b101, 0b101, 0b101, 0b111}}, //0 Right
    {{0b001, 0b011, 0b101, 0b001, 0b001, 0b001, 0b001}}, //1 Right
    {{0b011, 0b101, 0b001, 0b010, 0b100, 0b100, 0b111}}, //2 Right
    {{0b010, 0b101, 0b001, 0b010, 0b001, 0b101, 0b010}}, //3 Right
    {{0b001, 0b011, 0b101, 0b111, 0b001, 0b001, 0b001}}, //4 Right
    {{0b111, 0b100, 0b100, 0b111, 0b001, 0b001, 0b111}}, //5 Right
    {{0b011, 0b100, 0b100, 0b111, 0b101, 0b101, 0b111}}, //6 right
    {{0b111, 0b101, 0b001, 0b001, 0b001, 0b001, 0b001}}, //7 Right
    {{0b111, 0b101, 0b101, 0b111, 0b101, 0b101, 0b111}}, //8 Right
    {{0b111, 0b101, 0b101, 0b111, 0b001, 0b001, 0b111}}  //9 Right
};

//Function to run specified commands in terminal
static void runCommand(char* command);

//Function which converts logical numbers to physcial numbers to be displayed on matrix
static void Logical_to_PhysicalFrame(DigitMatrix *digit);

//Forms 8x8 bit matrix with one digit on the left and one on the right and writes the matrix
void write2Digits(int left_digit, int right_digit, int i2cFileDesc);

//Display integer to matrix
void displayInteger(int number, int i2cFileDesc);

//Main game loop for the snake game
void gamelooop(int speed);

#endif