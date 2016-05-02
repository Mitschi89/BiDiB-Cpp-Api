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

class BidibApi {
public:

	BidibApi();
	virtual ~BidibApi();

//	struct SEGMENTID{
//		enum {	A1 = 0,
//				N2 = 1,
//				N3 = 2,
//				NO1 = 3,
//				TA1 = 4,
//				TN0 = 5,
//				O1 = 6,
//				O2 = 7,
//				O3 = 8,
//				NO2 = 9,
//				SO1 = 10,
//				SO2 = 11,
//				S3 = 12,
//				SO3 = 13,
//				TSO = 14,
//				DEFAULT = 15,
//				NW3 = 16,
//				TC = 17,
//				SW1 = 18,
//				A2 = 19,
//				S2 = 20,
//				SW2 = 21,
//				TA2 = 22,
//				S1 = 23,
//				NW2 = 24,
//				N1 = 25,
//				NW1 = 26,
//				TNW = 27,
//				W1 = 28,
//				W2 = 29,
//				W3 = 30,
//				TSW = 31
//				};
//	};

//	struct TURNOUTID{
//		enum {	TNW = 0,
//				TSW = 1,
//				TC = 2,
//				TA2 = 3,
//				TA1 = 4,
//				TNO = 5,
//				TSO = 6
//				};
//	};

	BiDiBMessageHandler bidibMessageHandler;

	void powerOn(); //turns Booster Power on
	void powerOff(); //turns Booster Power off

	bool selfTestTurnout(); // switches all turnouts to check the functionality

	int getNumberOfLocs(); //returns number of locs on track
	Segment::segmentID* geLocPosition(int locID);
	int getAllLocPostion(); //returns all Positions of locs, no specific ID is returned
	void setLocSpeed(int locID, int speed, bool forward); // sets speed of loc
	int getLocSpeed(int locID); //returns speed of loc
	bool getLocDirection(int locID); // returns direction of loc
	void setLocFunction(int locID, locFunction locFunctionID, bool functionState); //sets a loc function
	bool getLocFunctionstate(int locID, int functionNumber); //returns the state of a function of a loc

	Turnout::turnDirection getTurnoutState(Turnout::turnoutID turnID); // returns the state of turnout
	int getAllTurnoutStates(); //returns all turnout states
	void setTurnoutState(Turnout::turnoutID turnID, Turnout::turnDirection turnDir); // sets the state of a turnout

	int getLocsOnSegment(Segment::segmentID segID); //returns number of locs on a segment

};


#endif /* BIDIBAPI_H_ */
