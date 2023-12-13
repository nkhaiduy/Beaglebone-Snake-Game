#ifndef JOYSTICKTEST_H
#define JOYSTICKTEST_H

//Function to retrieve the X-axis readings from the joystick
double joyStickReadingX();

//Function to retrieve the Y-axis readings from the joystick
double joyStickReadingY();

//Function to return the voltage readings
int getVoltage0Reading();

//Function to add delay
static void sleepForMs(long long delayInMs);
#endif 