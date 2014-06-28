#include <termios.h>
#include <stdio.h>

void main()
{
	struct termios config;		
	
	printf("Term restore file called\n");
	tcgetattr(0,&config);
	config.c_lflag |= ICANON;  	/*Enable buffered i/o */
	config.c_lflag |= ECHO;		/* Disable echo mode */
	tcsetattr(0, TCSANOW, &config);
}
