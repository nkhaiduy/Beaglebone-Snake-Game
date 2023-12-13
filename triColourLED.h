#ifndef TRICOLOURLED_H
#define TRICOLOURLED_H

#define BLUE_LED_DIRECTION  "/sys/class/gpio/gpio2/direction"
#define BLUE_LED_VALUE  "/sys/class/gpio/gpio2/value"
#define RED_LED_DIRECTION "/sys/class/gpio/gpio50/direction"
#define RED_LED_VALUE "/sys/class/gpio/gpio50/value"
#define GREEN_LED_DIRECTION "/sys/class/gpio/gpio51/direction"
#define GREEN_LED_VALUE "/sys/class/gpio/gpio51/value"

//Function used to write the specified value to the files.
static void writeToFile(const char* fileName, const char* value);

//Function to turn on specified LED
void turnOnLED(char* LED);

//Function to turn off specified LED
void turnOffLED(char* LED);

#endif