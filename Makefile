all:

	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -pthread -D _POSIX_C_SOURCE=200809L -Werror snakeGame.c -o snakeGame
	cp snakeGame $(HOME)/cmpt433/public/myApps/
