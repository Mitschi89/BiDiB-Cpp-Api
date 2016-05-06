//============================================================================
// Name        	: BidibApi.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: BiDiB API
//============================================================================

#ifndef BIDIBAPI_H_
#define BIDIBAPI_H_

#include "BiDiBMessageHandler.h"
#include "Segment.h"
#include "Turnout.h"
#include "locFunction.h"
#include <vector>

class BidibApi {
public:

	BidibApi();
	virtual ~BidibApi();

	//turns Booster Power on
	void powerOn();
	//turns Booster Power off
	void powerOff();

	//returns connection status
	bool isConnected();

	// switches all turnouts to check the functionality
	bool selfTestTurnout();
	// tests all segments on Track, needs ONE locomotvie on track on A2, direction from A2 to TA2
	bool selfTestTrack();

	//returns number of locs on track
	int getNumberOfLocs();
	//returns an array with a non defined number of SegmentIDs, last is always DEFAULT
	std::vector<Segment::segmentID> getLocPosition(int locID);
	//returns true if loc is on that segment
	bool isLocOnPosition(int locID, Segment::segmentID segID);
	//returns all Positions of locs, no specific ID is returned
	int getAllLocPostion();
	// sets speed of loc
	void setLocSpeed(int locID, int speed, bool forward);
	//returns speed of loc
	int getLocSpeed(int locID);
	// returns direction of loc
	bool getLocDirection(int locID);
	//sets a loc function
	void setLocFunction(int locID, locFunction::locFunctionID ID, bool functionState);
	//returns the state of a function of a loc
	bool getLocFunctionstate(int locID, locFunction::locFunctionID ID);

	// returns the state of turnout
	Turnout::turnDirection getTurnoutState(Turnout::turnoutID turnID);
	//returns all turnout states
	std::vector<Turnout> getAllTurnoutStates();
	// sets the state of a turnout
	void setTurnoutState(Turnout::turnoutID turnID, Turnout::turnDirection turnDir);
	// sets all turnouts in one state
	void setAllTurnoutsState(Turnout::turnDirection turnDir);

	//returns number of locs on a segment
	int getLocsOnSegment(Segment::segmentID segID);

private:
	//basic variable for all actions
	BiDiBMessageHandler bidibMessageHandler;
};


#endif /* BIDIBAPI_H_ */
