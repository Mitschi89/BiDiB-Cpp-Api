/*
 * BiDiBAPI.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: misc0894
 */


#include "BidibApi.h"

BidibApi::BidibApi() {

}

BidibApi::~BidibApi() {
	// TODO Auto-generated destructor stub
}

void BidibApi::powerOn() {
	bidibMessageHandler.sendBoostOnMessage();
}

void BidibApi::powerOff() {
	bidibMessageHandler.sendBoostOffMessage();
}

bool BidibApi::selfTestTurnout() {
	setTurnoutState(Turnout::TNW, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TNW, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TA1, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TA1, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TNO, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TNO, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TSO, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TSO, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TA2, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TA2, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TSW, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TSW, Turnout::turn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TC, Turnout::straight);
	usleep(1000*1000);
	setTurnoutState(Turnout::TC, Turnout::turn);
	usleep(1000*1000);
}

int BidibApi::getNumberOfLocs() {
	return bidibMessageHandler.locCount;
}

int BidibApi::getAllLocPostion() {
	return bidibMessageHandler.locAllPositions;
}

void BidibApi::setLocSpeed(int locID, int speed, bool forward) {
	bidibMessageHandler.sendDriveMessage(locID, speed , forward);
}

int BidibApi::getLocSpeed(int locID) {
	return bidibMessageHandler.locs[locID].speed;
}

bool BidibApi::getLocDirection(int locID) {
	return bidibMessageHandler.locs[locID].direction;
}

void BidibApi::setLocFunction(int locID, locFunction::locFunctionID ID, bool functionState) {
	bidibMessageHandler.sendFunctionStateMessage(locID, ID, functionState);
}

bool BidibApi::getLocFunctionstate(int locID, locFunction::locFunctionID ID) {
	if(ID == locFunction::FLight){
		return bidibMessageHandler.locs[locID].lightState;
	}else{
		return bidibMessageHandler.locs[locID].functionState[ID];
	}
}

Turnout::turnDirection BidibApi::getTurnoutState(Turnout::turnoutID turnID) {
	return bidibMessageHandler.turnouts[turnID].turnDir;
}

int BidibApi::getAllTurnoutStates() {
	int states = 0;
	for(int i=0; i<7; i++){
		states |= (bidibMessageHandler.turnouts[i].turnDir << i);
	}
	return states;
}

Segment::segmentID* BidibApi::geLocPosition(int locID) {
	Segment::segmentID segID[10];
	for(int i = 0; i < 10; i++){
		segID[i] = (Segment::DEFAULT);
	}

	int index = 0;
	int postion = bidibMessageHandler.locs[locID].position;

	if(locID < bidibMessageHandler.locCount){
		for(int i = 0; i <= 31; i++){
			if(postion & (1 << i)){
				segID[index] = (Segment::segmentID) i;
				index++;
			}
		}
	}
	return segID;
}

void BidibApi::setTurnoutState(Turnout::turnoutID turnID, Turnout::turnDirection turnDir) {
	bidibMessageHandler.sendTurnMessage(turnID, turnDir);
}

int BidibApi::getLocsOnSegment(Segment::segmentID segID) {
	int locsOnSegment = 0;
	for (int i=0; i<bidibMessageHandler.locCount; i++){
		if(bidibMessageHandler.locs[i].position == segID){
			locsOnSegment |= (1 << i);
		}
	}
	return locsOnSegment;
}
