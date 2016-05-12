//============================================================================
// Name        	: BidibApi.cpp
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: BiDiB API
//============================================================================

#include "BidibApi.h"

BidibApi::BidibApi() {
	while(bidibMessageHandler.isConnected() != 1){
		if(bidibMessageHandler.isConnected() == -1){
			return;
		}
		usleep(1000*1000);
	}
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
	setAllTurnoutsState(Turnout::straightOn);

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

	std::vector<Segment::segmentID> sequence;
		sequence.push_back(Segment::A2);
		sequence.push_back(Segment::TA2);
		sequence.push_back(Segment::TSW);
		sequence.push_back(Segment::W3);
		sequence.push_back(Segment::W2);
		sequence.push_back(Segment::W1);
		sequence.push_back(Segment::TNW);
		sequence.push_back(Segment::N1);
		sequence.push_back(Segment::N2);
		sequence.push_back(Segment::N3);
		sequence.push_back(Segment::TNO);
		sequence.push_back(Segment::O1);
		sequence.push_back(Segment::O2);
		sequence.push_back(Segment::O3);
		sequence.push_back(Segment::TSO);
		sequence.push_back(Segment::S3);
		sequence.push_back(Segment::S2);
		sequence.push_back(Segment::S1);
		sequence.push_back(Segment::TSW);
		sequence.push_back(Segment::W3);
		sequence.push_back(Segment::W2);
		sequence.push_back(Segment::W1);
		sequence.push_back(Segment::TNW);
		sequence.push_back(Segment::NW1);
		sequence.push_back(Segment::NW2);
		sequence.push_back(Segment::NW3);
		sequence.push_back(Segment::TC);
		sequence.push_back(Segment::SO1);
		sequence.push_back(Segment::SO2);
		sequence.push_back(Segment::SO3);
		sequence.push_back(Segment::TSO);
		sequence.push_back(Segment::O3);
		sequence.push_back(Segment::O2);
		sequence.push_back(Segment::O1);
		sequence.push_back(Segment::TNO);
		sequence.push_back(Segment::TA1);
		sequence.push_back(Segment::NO1);
		sequence.push_back(Segment::NO2);
		sequence.push_back(Segment::TC);
		sequence.push_back(Segment::SW1);
		sequence.push_back(Segment::SW2);
		sequence.push_back(Segment::TA2);
		sequence.push_back(Segment::TSW);
		sequence.push_back(Segment::W3);
		sequence.push_back(Segment::W2);
		sequence.push_back(Segment::W1);
		sequence.push_back(Segment::TNW);
		sequence.push_back(Segment::NW1);
		sequence.push_back(Segment::NW2);
		sequence.push_back(Segment::NW3);
		sequence.push_back(Segment::TC);
		sequence.push_back(Segment::SO1);
		sequence.push_back(Segment::SO2);
		sequence.push_back(Segment::SO3);
		sequence.push_back(Segment::TSO);
		sequence.push_back(Segment::O3);
		sequence.push_back(Segment::O2);
		sequence.push_back(Segment::O1);
		sequence.push_back(Segment::TNO);
		sequence.push_back(Segment::TA1);
		sequence.push_back(Segment::A1);

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
	int locSpeed = 100 ;

	int sequenceIterator = 0;
	int o2Iterator = 0;

	Segment::segmentID nextPosition = sequence.at(sequenceIterator++);

	while(!isLocOnPosition(locID , nextPosition)){
		usleep(1000*1000);
	}

	Segment::segmentID currentPosition = nextPosition;
	nextPosition = sequence.at(sequenceIterator++);

	for(int i = 10; i > 0 ; i--){
		printf("Loc starts in %d \n" , i);
		usleep(1000 * 1000);
	}
	printf("Loc starts!\n");

	setLocSpeed(locID, locSpeed, true);

	std::vector<Segment::segmentID> positions (MAXNUMBEROFSEGEMENTSWITHLOC);
	positions = getLocPosition(locID);

	while(!isLocOnPosition(locID, sequence.back())){

		if(positions == getLocPosition(locID)){
			usleep(1000);
		}else{
			positions = getLocPosition(locID);
			if(isLocOnPosition(locID, nextPosition)){
				currentPosition = nextPosition;
				nextPosition = sequence.at(sequenceIterator++);

				if(currentPosition == Segment::S1){
					setTurnoutState(Turnout::TNW, Turnout::bendOff);
				}

				if(currentPosition == Segment::SO3){
					setTurnoutState(Turnout::TNO, Turnout::bendOff);
					setTurnoutState(Turnout::TA1, Turnout::bendOff);
				}

				if(currentPosition == Segment::O2){
					if(++o2Iterator == 3){
						setTurnoutState(Turnout::TA1, Turnout::straightOn);
					}
				}
			}else{
				if(!isLocOnPosition(locID, currentPosition)){
					printf("ERROR segment Number: %d not found\n", nextPosition);
					setLocSpeed(locID, 1, true);
					return false;
				}
			}
		}
	}

	setLocSpeed(locID, 1, true);


	return true;
}

int BidibApi::getNumberOfLocs() {
	return bidibMessageHandler.locCount;
}

uint32_t BidibApi::getAllLocPosition() {
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

std::vector<Turnout> BidibApi::getAllTurnoutStates() {
	std::vector<Turnout> turnouts (MAXNUMBEROFTURNOUTS);

	for(int i = 0; i < MAXNUMBEROFTURNOUTS; i++){
		turnouts.push_back(bidibMessageHandler.turnouts[i]);
	}

	return turnouts;
}

std::vector<Segment::segmentID> BidibApi::getLocPosition(int locID) {
	std::vector<Segment::segmentID> segID (MAXNUMBEROFSEGEMENTSWITHLOC);

	for(int i = 0; i < MAXNUMBEROFSEGEMENTSWITHLOC; i++){
		segID[i] = Segment::DEFAULT;
	}

	int index = 0;
	uint32_t position = bidibMessageHandler.locs[locID].position;

	if(locID < getNumberOfLocs()){
		for(int i = 0; i < MAXNUMBEROFSEGMENTS; i++){
			if(position & (1 << i)){
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
	for(int i = 0; i < MAXNUMBEROFTURNOUTS; i++){
		setTurnoutState((Turnout::turnoutID) i , turnDir);
	}
}

bool BidibApi::isConnected() {
	if((bidibMessageHandler.isConnected() == 0) || (bidibMessageHandler.isConnected() == -1)){
		return false;
	}
	if(bidibMessageHandler.isConnected() == 1){
		return true;
	}

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

Segment::segmentID BidibApi::getPredictedNextState(int locID) {
	return bidibMessageHandler.locs[locID].stateMaschine.nextState;
}

Segment::segmentID BidibApi::getCurrentState(int locID) {
	return bidibMessageHandler.locs[locID].stateMaschine.currentState;
}
