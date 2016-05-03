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
	setTurnoutState(Turnout::TNW, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TNW, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TA1, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TA1, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TNO, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TNO, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TSO, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TSO, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TA2, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TA2, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TSW, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TSW, Turnout::straightOn);
	usleep(1000*1000);

	setTurnoutState(Turnout::TC, Turnout::bendOff);
	usleep(1000*1000);
	setTurnoutState(Turnout::TC, Turnout::straightOn);
	usleep(1000*1000);

	printf("All Turnouts should be straight!\n");

	return true;
}

bool BidibApi::selfTestTrack() {
	setAllTurnoutsState(Turnout::straightOn);
	printf("You can place the loc now on segment A2 in direction TA2 and start ... \n");
	while(getNumberOfLocs() == 0){
		usleep(1000*1000);
	}

	if(getNumberOfLocs() > 1){
		printf("To much locs on track, please place only ONE loc on segment A2 in direction TA2 and start again.\n");
		return false;
	}

	int locID = getNumberOfLocs() - 1;

	while(!isLocOnPosition(locID , Segment::A2)){
		usleep(1000*1000);
	}

	for(int i = 10; i > 0 ; i--){
		printf("Loc starts in %d \n" , i);
		usleep(1000 * 1000);
	}
	printf("Loc starts!\n");
	setLocSpeed(locID, 25, true);

	return true;
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

std::vector<Segment::segmentID> BidibApi::getLocPosition(int locID) {
	std::vector<Segment::segmentID> segID (MAXNUMBEROFSEGEMENTSWITHLOC);

	for(int i = 0; i < MAXNUMBEROFSEGEMENTSWITHLOC; i++){
		segID[i] = Segment::DEFAULT;
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

void BidibApi::setAllTurnoutsState(Turnout::turnDirection turnDir) {
	setTurnoutState(Turnout::TNW, turnDir);
	setTurnoutState(Turnout::TA1, turnDir);
	setTurnoutState(Turnout::TNO, turnDir);
	setTurnoutState(Turnout::TSO, turnDir);
	setTurnoutState(Turnout::TA2, turnDir);
	setTurnoutState(Turnout::TSW, turnDir);
	setTurnoutState(Turnout::TC, turnDir);
}

bool BidibApi::isConnected() {
	return bidibMessageHandler.isConnected();
}

bool BidibApi::isLocOnPosition(int locID, Segment::segmentID segID) {
	std::vector<Segment::segmentID> segments (MAXNUMBEROFSEGEMENTSWITHLOC);
	segments = getLocPosition(locID);
	for(int i = 0; i < MAXNUMBEROFSEGEMENTSWITHLOC; i++){
		if(segments[i] == segID){
			return true;
		}
	}
	return false;
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
