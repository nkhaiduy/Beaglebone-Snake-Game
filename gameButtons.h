#ifndef GAMEBUTTONS_H
#define GAMEBUTTONS_H

#define GREEN_BUTTON_DIRECTION  "/sys/class/gpio/gpio65/direction"
#define GREEN_BUTTON_VALUE  "/sys/class/gpio/gpio65/value"
#define START_BUTTON "start_button"

#define STOP_BUTTON "stop_button"
#define RED_BUTTON_DIRECTION "/sys/class/gpio/gpio46/direction"
#define RED_BUTTON_VALUE "/sys/class/gpio/gpio46/value"

#define SPEED_BUTTON "speed_button"
#define BLUE_BUTTON_DIRECTION "/sys/class/gpio/gpio27/direction"
#define BLUE_BUTTON_VALUE "/sys/class/gpio/gpio27/value"

//Configures the GPIO buttons for the game
static void configureGameButtons(char* button);

//Checks if the specified button is pressed
int checkBUttonPress(char* buttonValue);



#endif