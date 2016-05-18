/*
 * StateMaschine.h
 *
 *  Created on: May 11, 2016
 *      Author: misc0894
 */

#ifndef STATEMASCHINE_H_
#define STATEMASCHINE_H_

#include "Segment.h"
#include "Turnout.h"
#include "Definitions.h"
#include <stdio.h>
#include <vector>

class StateMaschine {
public:
	StateMaschine();
	virtual ~StateMaschine();

	void updateState(int currentPosition);
	bool updateDirection();
	Segment::segmentID predictedNextState();

	void setCircles();

	Segment::segmentID currentState = Segment::DEFAULT;
	Segment::segmentID previousState = Segment::DEFAULT;
	Segment::segmentID nextState = Segment::DEFAULT;
	bool direction = true;
	bool logicalDirection = true;
	bool driveStatus = false;
	Turnout turnouts[MAXNUMBEROFTURNOUTS];

	struct Node{
		Segment::segmentID currentNode;
		Segment::segmentID nextNode;
		Segment::segmentID previousNode;
	};
	std::vector<struct Node> bigCircle;
	std::vector<struct Node> leftCircle;
	std::vector<struct Node> rightCircle;

};

#endif /* STATEMASCHINE_H_ */
