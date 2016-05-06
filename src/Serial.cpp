//============================================================================
// Name        	: Serial.cpp
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: Implements basic serial communication
//============================================================================

#include "Serial.h"

Serial::Serial(const char *portName){
	fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
	struct termios term_attr;

	if(fd == -1){
		printf("Open_port: Unable to open %s \n", portName);
	}
	else{
		fcntl(fd, F_SETFL, 0);
		printf("Port is open.\n");
	}

	if (tcgetattr(fd, &term_attr) != 0)
	{
		printf("Could not get attributes.\n");
		return;
	}
	term_attr.c_cflag = BAUDRATE | CS8 | CRTSCTS | CREAD | CLOCAL;
	term_attr.c_iflag = 0;
	term_attr.c_oflag = OPOST;
	term_attr.c_lflag = 0;

	tcsetattr(fd, TCSAFLUSH, &term_attr);

}

Serial::~Serial(){
	int err = 0;

	if(fd >= 0){
		tcflush(fd, TCOFLUSH);
		err = close( fd );
		fd = -1;
	}
}

int Serial::ReadData(unsigned char *buffer, unsigned int nbChar){
	int length = read(fd, buffer, nbChar);
	if((length < 0) && (errno == EAGAIN)) return 0;
	return length;
}


int Serial::WriteData(unsigned char *buffer, unsigned int nbChar){
	int n = write(fd, buffer, nbChar);
	if((n < 0) && (errno == EAGAIN)) return 0;

//	printf("Send: ");
//	for (int i =0 ; i < nbChar ; i++){
//		printf("%02X ",buffer[i]);
//	}
//	printf("\n");
//	fflush(stdout);

	return n;
}


bool Serial::IsConnected(){
	return (fd != -1);
}


