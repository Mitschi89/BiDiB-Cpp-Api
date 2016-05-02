/*
 * Serial.h
 *
 *  Created on: 22.01.2016
 *      Author: Steffi
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
//#include <windows.h>

#define BAUDRATE B115200

class Serial {

protected:
		int fd;

public:
		//Initialize Serial communication with the given COM port
//      Serial();
		Serial(const char *portName);
		//Close the connection
		~Serial();
		//Read data in a buffer, if nbChar is greater than the
		//maximum number of bytes available, it will return only the
		//bytes available. The function return -1 when nothing could
		//be read, the number of bytes actually read.
		int ReadData(unsigned char *buffer, unsigned int nbChar);
		//Writes data from a buffer through the Serial connection
		//return true on success.
		int WriteData(unsigned char *buffer, unsigned int nbChar);
		//Check if we are actually connected
		bool IsConnected();
};


#endif /* SERIAL_H_ */
