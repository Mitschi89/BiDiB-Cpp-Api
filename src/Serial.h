//============================================================================
// Name        	: Serial.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: Implements basic serial communication
//============================================================================

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BAUDRATE B115200

class Serial {

protected:
	int fd;

public:
	//Initialize Serial communication with the given COM port
	Serial(const char *portName);
	//Close the connection
	~Serial();
	//Read data in a buffer, if nbChar is greater than the
	//maximum number of bytes available, it will return only the
	//bytes available. The function return -1 when nothing could
	//be read, the number of bytes actually read.
	int readData(unsigned char *buffer, unsigned int nbChar);
	//Writes data from a buffer through the Serial connection
	//return true on success.
	int writeData(unsigned char *buffer, unsigned int nbChar);
	//Check if we are actually connected
	bool isConnected();
};

#endif /* SERIAL_H_ */
