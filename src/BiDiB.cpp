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
#include <cstdlib>
#include "BiDiBMessageHandler.h"
#include "bidib_messages.h"
#include "BidibApi.h"
#include "Turnout.h"
#include "Definitions.h"

using namespace std;

int locCount = 0;
int speed = 50;
Segment::segmentID criticalState[MAXNUMBEROFSEGEMENTSWITHLOC] = {Segment::DEFAULT};

void handlePredictions(BidibApi *api){

	if(locCount != api->getNumberOfLocs()){
		locCount = api->getNumberOfLocs();
		for(int locID = 0; locID < locCount; locID++){
			api->setLocSpeed(locID, speed, true);
		}
	}

	for(int locID = 0; locID < locCount; locID++){
		if(api->getLocSpeed(locID) != speed){
			api->setLocSpeed(locID, speed, true);
		}
	}

	for(int locID = 0; locID < locCount; locID++){

		if(api->getCurrentState(locID) != criticalState[locID]){
			criticalState[locID] = Segment::DEFAULT;
		}

		if((api->getCurrentState(locID) == Segment::A1) || (api->getCurrentState(locID) == Segment::A2)){
			if(!(api->isLocOnPosition(locID, Segment::TA1)) && !(api->isLocOnPosition(locID, Segment::TA2))){
				if((criticalState[locID] != Segment::A1) && (criticalState[locID] != Segment::A2)){
					if(api->getLocLogicalDirection(locID) == false){
						api->setLocSpeed(locID, speed, !api->getLocDirection(locID));
						criticalState[locID] = api->getCurrentState(locID);
					}
				}
			}
		}

		for(int nextLocID = locID+1; nextLocID < locCount; nextLocID++){

			if(api->getCurrentState(nextLocID) != criticalState[nextLocID]){
				criticalState[nextLocID] = Segment::DEFAULT;
			}

			if(api->getPredictedNextState(locID) == api->getPredictedNextState(nextLocID)){
				if((api->getCurrentState(locID) != criticalState[locID]) &&
						(api->getCurrentState(nextLocID) != criticalState[nextLocID])){

					api->setLocSpeed(locID, speed, !api->getLocDirection(locID));
					api->setLocSpeed(nextLocID, speed, !api->getLocDirection(nextLocID));
					criticalState[locID] = api->getCurrentState(locID);
					criticalState[nextLocID] = api->getCurrentState(nextLocID);
					printf("Equal Predicted State: loc: %d and loc: %d at position: %s\n" ,
							locID, nextLocID, Segment::convertSegmentName(api->getPredictedNextState(locID)));fflush(stdout);
				}
			}
			if(api->getPredictedNextState(locID) == api->getCurrentState(nextLocID)){
				if(api->getLocLogicalDirection(locID) == api->getLocLogicalDirection(nextLocID)){
					api->setLocSpeed(locID, speed / 2, api->getLocDirection(locID));
				}else{
					if((api->getCurrentState(locID) != criticalState[locID]) &&
							(api->getCurrentState(nextLocID) != criticalState[nextLocID])){

						api->setLocSpeed(locID, speed, !api->getLocDirection(locID));
						api->setLocSpeed(nextLocID, speed, !api->getLocDirection(nextLocID));
						criticalState[locID] = api->getCurrentState(locID);
						criticalState[nextLocID] = api->getCurrentState(nextLocID);
						printf("Predicted State is current State: loc: %d and loc: %d at position: %s\n" ,
								locID, nextLocID, Segment::convertSegmentName(api->getPredictedNextState(locID)));fflush(stdout);
					}
				}
			}
			if(api->getCurrentState(locID) == api->getPredictedNextState(nextLocID)){
				if(api->getLocLogicalDirection(locID) == api->getLocLogicalDirection(nextLocID)){
					api->setLocSpeed(nextLocID, speed / 2, api->getLocDirection(nextLocID));
				}else{
					if((api->getCurrentState(locID) != criticalState[locID]) &&
							(api->getCurrentState(nextLocID) != criticalState[nextLocID])){

						api->setLocSpeed(locID, speed, !api->getLocDirection(locID));
						api->setLocSpeed(nextLocID, speed, !api->getLocDirection(nextLocID));
						criticalState[locID] = api->getCurrentState(locID);
						criticalState[nextLocID] = api->getCurrentState(nextLocID);
						printf("current State is Predicted State: loc: %d and loc: %d at position: %s\n" ,
								locID, nextLocID, Segment::convertSegmentName(api->getCurrentState(locID)));fflush(stdout);
					}
				}
			}
			if(api->getCurrentState(locID) == api->getCurrentState(nextLocID)){
				if(api->getLocLogicalDirection(locID) != api->getLocLogicalDirection(nextLocID)){
					if((api->getCurrentState(locID) != criticalState[locID]) &&
							(api->getCurrentState(nextLocID) != criticalState[nextLocID])){

						api->setLocSpeed(locID, speed, !api->getLocDirection(locID));
						api->setLocSpeed(nextLocID, speed, !api->getLocDirection(nextLocID));
						criticalState[locID] = api->getCurrentState(locID);
						criticalState[nextLocID] = api->getCurrentState(nextLocID);
						printf("current state is current state: loc: %d and loc: %d at position: %s\n" ,
								locID, nextLocID, Segment::convertSegmentName(api->getPredictedNextState(locID)));fflush(stdout);
					}
				}
			}
		}
	}
}

bool startSelfTest(BidibApi *api){

	if(api->selfTestTrack()){
		printf("selftest OK\n");
		return true;
	}else{
		printf("ERROR while selftest.\n");
		return false;
	}
}

void switchRandomTurn(BidibApi *api){
	bool turnOk = true;

	Turnout::turnoutID turnID;
	do{
		turnOk = true;
		turnID = (Turnout::turnoutID)(rand() % 7);
		Segment::segmentID segID;
		switch(turnID){
		case Turnout::TA1:		segID = Segment::TA1; break;
		case Turnout::TA2:		segID = Segment::TA2; break;
		case Turnout::TC:		segID = Segment::TC; break;
		case Turnout::TNO:		segID = Segment::TNO; break;
		case Turnout::TNW:		segID = Segment::TNW; break;
		case Turnout::TSO:		segID = Segment::TSO; break;
		case Turnout::TSW:		segID = Segment::TSW; break;
		}
		for(int locID = 0; locID < locCount; locID++){
			if((api->isLocOnPosition(locID, segID)) || (api->getPredictedNextState(locID)) || (api->getCurrentState(locID))){
				turnOk = false;
			}
		}
	}while(!turnOk);
	api->toggleTurnoutState(turnID);
}

int main() {
	printf("Program Start...\n\n"); fflush(stdout);
	BidibApi api;

	api.setAllTurnoutsState(Turnout::straightOn);
	api.setTurnoutState(Turnout::TNW, Turnout::bendOff);
	api.setTurnoutState(Turnout::TNO, Turnout::bendOff);
//	api.setTurnoutState(Turnout::TA1, Turnout::bendOff);

	bool directionChanged = false;
	while(api.isConnected()){

		handlePredictions(&api);

//		switchRandomTurn(&api);

//		usleep(500*1000);
	}

	printf("\nProgram End... \n"); fflush(stdout);

	return 0;
}


