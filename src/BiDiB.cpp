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
	printf("Program Start\n\n"); fflush(stdout);
	BidibApi api;

//	usleep(1000*1000);
	while(api.isConnected()){

		if(api.selfTestTrack()){
			printf("selftest OK\n");
			break;
		}else{
			printf("ERROR while selftest.\n");
			break;
		}
	}

	printf("Program End... \n"); fflush(stdout);

	return 0;
}
