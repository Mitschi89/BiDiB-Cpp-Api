//============================================================================
// Name        	: BiDiB.cpp
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: Example program
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
	printf("Program Start...\n\n"); fflush(stdout);
	BidibApi api;

	while(api.isConnected()){

		if(api.selfTestTrack()){
			printf("selftest OK\n");
			break;
		}else{
			printf("ERROR while selftest.\n");
			break;
		}
	}

	printf("\nProgram End... \n"); fflush(stdout);

	return 0;
}
