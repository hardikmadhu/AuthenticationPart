#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

short passwrd,passcode;
struct termios config;

void init_terminos() 
{
	tcgetattr(0,&config);
	config.c_lflag &= ~ICANON; /* disable buffered i/o */
  	config.c_lflag &= ~ECHO; /* Disable echo mode */
  	tcsetattr(0, TCSANOW, &config);
}

void reset_terminos()
{	
	config.c_lflag |= ICANON;  	/*Enable buffered i/o */
	config.c_lflag |= ECHO;		/* Disable echo mode */
	tcsetattr(0, TCSANOW, &config);
}

void get_passcode()
{	
	static short count =3;

	init_terminos();
	printf("Enter Passcode: ");
	scanf("%hd",&passwrd);
	if(passcode == passwrd)	{
		printf("\nAccess Granted\n");
		count = 3;
	}
	
	else {
		count--;
		printf("\nAccess Denied\n");
		if( count >= 0) {
			printf("You have %d tries remainig\n", count);
			get_passcode();
		}
		else {
			printf("Your tries are over\n");
			goto ret1;
		}
	}

ret1:
	printf("Kill.sh will be called\n");
	reset_terminos();
	system("/home/hardik/Documents/project/Complete/kill.sh");
}

int send_passcode(char *psd)
{
	
	int fd,bytes;
	char *buf;

	buf = malloc(50*sizeof(char));
	
	fd = open("/dev/ttyUSB0",O_RDWR);
	if(fd < 0)
	{
		printf("Unable to open device\n");
		return fd;
	}
	printf("Device open with fd = %d\n",fd);
	
	buf = "AT+CMGF=1\r";
	//printf("size of buf: %d\n",sizeof(buf));
	bytes = write(fd, buf,10);
	bytes = read(fd, buf, 18);
	printf("read sequenc:%s\n", buf);
	printf("Text mode selected\n");

	buf = "AT+CMGS=\"+919408896696\"\r";
	bytes = write(fd, buf, 24);
	
	bytes = read(fd, buf, 31);
	printf("Read sequence %s\n", buf);
	
	bytes = write(fd, psd,6);
	bytes = read(fd, buf, 21);
	printf("Last response %s\n", buf);
	close(fd);
	
	return 0;
}

int conv_passcode()
{
	char *buf;
	int ret;

	buf = malloc(10*sizeof(char));
	
	sprintf(buf,"%d%c\n", passcode, '\x1A');
	
	ret = send_passcode(buf);
	if(ret != 0)
	{
		printf("Something wrong happened.. Try again\n");	
		return ret;
	}
	return 0;
}

int generate_passcode()
{
	time_t seed;
	int ret;
	
	time(&seed);

	srand(seed);
	
	passcode = rand() % 10000;
	
	ret = conv_passcode();
	if(ret != 0)
	{
		return ret;
	}
	
	printf("Passcode generated and sent to mobile\n");
	return 0;
}

int start_timer(void)
{
	int ret;
	
	sleep(5);
	
	ret = system("sudo insmod up_call_module.ko");
	if(ret != 0)
	{
		printf("Problem in calling the module\nExiting...\n");
		return ret;
	}
	printf("Timer Started, Passcode will expire in 5 mins\n");
	return 0;
}

int gsm_config()
{
	int ret = 0;
	
	ret = system("sudo stty -F /dev/ttyUSB0 115200");
	if (ret != 0)
	{
		printf("Couldn't configure the GSM modem\n");
		return ret;
	}	
	printf("GSM modem successfully configured\n");
	return 0;
}

void main()
{
	int err;
	
	err = gsm_config();
	if(err != 0)
		return;
		
	err = generate_passcode();
	if(err != 0)
		return;
	
	err = start_timer();
	if(err != 0)
		return;

	get_passcode();
}
