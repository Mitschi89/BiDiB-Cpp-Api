/*
 * Weiche.h
 *
 *  Created on: 17.02.2016
 *      Author: Steffi
 */

#ifndef TURNOUT_H_
#define TURNOUT_H_


class Turnout {
public:

	enum turnDirection{
		straightOn = 1,
		bendOff = 0
	} turnDir = straightOn;

	enum turnoutID{	TNW = 0,
			TSW = 1,
			TC = 2,
			TA2 = 3,
			TA1 = 4,
			TNO = 5,
			TSO = 6
			} ID;


	Turnout();
	~Turnout();
};

#endif /* TURNOUT_H_ */
