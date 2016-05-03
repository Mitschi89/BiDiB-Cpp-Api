/*
 * BiDiBAPI.h
 *
 *  Created on: Feb 23, 2016
 *      Author: misc0894
 */

#ifndef BIDIBAPI_H_
#define BIDIBAPI_H_

#include "BiDiBMessageHandler.h"
#include "Segment.h"
#include "Turnout.h"
#include "locFunction.h"
#include <vector>

#define MAXNUMBEROFSEGEMENTSWITHLOC 10 //max Number of segment a loc can be on

class BidibApi {
public:

	BidibApi();
	virtual ~BidibApi();

	BiDiBMessageHandler bidibMessageHandler;

	void powerOn(); //turns Booster Power on
	void powerOff(); //turns Booster Power off

	bool isConnected();

	bool selfTestTurnout(); // switches all turnouts to check the functionality
	bool selfTestTrack(); // tests all segments on Track, needs ONE locomotvie on track on A2, direction from A2 to TA2

	int getNumberOfLocs(); //returns number of locs on track
	std::vector<Segment::segmentID> getLocPosition(int locID);
	bool isLocOnPosition(int locID, Segment::segmentID segID);
	int getAllLocPostion(); //returns all Positions of locs, no specific ID is returned
	void setLocSpeed(int locID, int speed, bool forward); // sets speed of loc
	int getLocSpeed(int locID); //returns speed of loc
	bool getLocDirection(int locID); // returns direction of loc
	void setLocFunction(int locID, locFunction::locFunctionID ID, bool functionState); //sets a loc function
	bool getLocFunctionstate(int locID, locFunction::locFunctionID ID); //returns the state of a function of a loc

	Turnout::turnDirection getTurnoutState(Turnout::turnoutID turnID); // returns the state of turnout
	int getAllTurnoutStates(); //returns all turnout states
	void setTurnoutState(Turnout::turnoutID turnID, Turnout::turnDirection turnDir); // sets the state of a turnout
	void setAllTurnoutsState(Turnout::turnDirection turnDir);

	int getLocsOnSegment(Segment::segmentID segID); //returns number of locs on a segment

};


#endif /* BIDIBAPI_H_ */
