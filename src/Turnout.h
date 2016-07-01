//============================================================================
// Name        	: Turnout.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: basic class for turnout IDs and directions
//============================================================================

#ifndef TURNOUT_H_
#define TURNOUT_H_

class Turnout {
public:

	enum turnDirection {
		straightOn = 1, bendOff = 0
	} turnDir = straightOn;

	enum turnoutID {
		TNW = 0, TSW = 1, TC = 2, TA2 = 3, TA1 = 4, TNO = 5, TSO = 6
	} ID = TNW;

	Turnout();
	~Turnout();
};

#endif /* TURNOUT_H_ */
