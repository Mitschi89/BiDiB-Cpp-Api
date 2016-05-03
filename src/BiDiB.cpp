//============================================================================
// Name        : BiDiB.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include "BiDiBMessageHandler.h"
#include "bidib_messages.h"
#include "BidibApi.h"
#include "Turnout.h"

using namespace std;

int main() {
	printf("Programm Start\n\n"); fflush(stdout);
	BidibApi api;
//	BiDiBMessageHandler bidibMessageHandler;

	usleep(1000*1000);
	while(api.isConnected()){
//		if((bidibMessageHandler.loks[0].position >> 9)  & 1 ) {
//			speed = 1;
//			bidibMessageHandler.sendDriveMessage(11 , speed, 1);
//			usleep(10000*1000);
//			speed = 50;
//			bidibMessageHandler.sendDriveMessage(11, speed, 1);
//			usleep(2000*1000);
//		}else{
//			speed = 100;
//			bidibMessageHandler.sendDriveMessage(11, speed, 1);
//			bidibMessageHandler.sendTurnMessage(0 , 0);
//			usleep(1000*1000);
//			bidibMessageHandler.sendTurnMessage(0 , 1);
//		}

//		bidibMessageHandler.sendTurnMessage(0 , 0);
//		api.setTurnoutState(Turnout::TC, true);
//
//		usleep(1000*1000);
////		bidibMessageHandler.sendTurnMessage(0 , 1);
//		api.setTurnoutState(Turnout::TC, false);
//		usleep(1000*1000);
//		usleep(10000*1000);

//		api.selfTestTurnout();
		api.setTurnoutState(Turnout::TC, Turnout::bendOff);

		usleep(1000);
		if(api.getTurnoutState(Turnout::TC) == Turnout::straightOn){
			printf("turn is straight\n");
		}else{
			printf("turn is not straight\n");
		}
	}

	printf("Programm Ende \n"); fflush(stdout);

	return 0;
}
