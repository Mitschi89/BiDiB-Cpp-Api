/*
 * StateMaschine.cpp
 *
 *  Created on: May 11, 2016
 *      Author: misc0894
 */

#include "StateMaschine.h"

StateMaschine::StateMaschine() {
}

StateMaschine::~StateMaschine() {
	// TODO Auto-generated destructor stub
}

void StateMaschine::updateState(int currentPosition) {
//	printf("real Position: %d\n", currentPosition); fflush(stdout);

	switch((Segment::segmentID) currentPosition){
	case Segment::N1:
	case Segment::N2:
	case Segment::N3:		previousState = currentState;
							currentState = Segment::N1;
							break;
	case Segment::S1:
	case Segment::S2:
	case Segment::S3:		previousState = currentState;
							currentState = Segment::S1;
							break;
	case Segment::W1:
	case Segment::W2:
	case Segment::W3:		previousState = currentState;
							currentState = Segment::W1;
							break;
	case Segment::O1:
	case Segment::O2:
	case Segment::O3:		previousState = currentState;
							currentState = Segment::O1;
							break;
	case Segment::NW1:
	case Segment::NW2:
	case Segment::NW3:		previousState = currentState;
							currentState = Segment::NW1;
							break;
	case Segment::SO1:
	case Segment::SO2:
	case Segment::SO3:		previousState = currentState;
							currentState = Segment::SO1;
							break;
	case Segment::NO1:
	case Segment::NO2:		previousState = currentState;
							currentState = Segment::NO1;
							break;
	case Segment::SW1:
	case Segment::SW2:		previousState = currentState;
							currentState = Segment::SW1;
							break;
	default:				previousState = currentState;
							currentState = (Segment::segmentID) currentPosition;
							break;
	}

	if(previousState != currentState){
		if(currentState != nextState){
			logicalDirection = !logicalDirection;
		}
		nextState = predictedNextState();
	}
}

Segment::segmentID StateMaschine::predictedNextState() {
	Segment::segmentID nextState = Segment::DEFAULT;

	if(!driveStatus){
		nextState = currentState;
	}else{
		switch(currentState){
		case Segment::N1:		if(logicalDirection){
									nextState = Segment::TNO;
								}else{
									nextState = Segment::TNW;
								}
								break;
		case Segment::S1:		if(logicalDirection){
									nextState = Segment::TSW;
								}else{
									nextState = Segment::TSO;
								}
								break;
		case Segment::W1:		if(logicalDirection){
									nextState = Segment::TNW;
								}else{
									nextState = Segment::TSW;
								}
								break;
		case Segment::O1:		if(logicalDirection){
									nextState = Segment::TSO;
								}else{
									nextState = Segment::TNO;
								}
								break;
		case Segment::NW1:		if(logicalDirection){
									nextState = Segment::TC;
								}else{
									nextState = Segment::TNW;
								}
								break;
		case Segment::NO1:		if(logicalDirection){
									nextState = Segment::TA1;
								}else{
									nextState = Segment::TC;
								}
								break;
		case Segment::SW1:		if(logicalDirection){
									nextState = Segment::TA2;
								}else{
									nextState = Segment::TC;
								}
								break;
		case Segment::SO1:		if(logicalDirection){
									nextState = Segment::TC;
								}else{
									nextState = Segment::TSO;
								}
								break;
		case Segment::A1:		if(logicalDirection){
									nextState = Segment::TA1;
								}else{
									nextState = Segment::A1;
								}
								break;
		case Segment::A2:		if(logicalDirection){
									nextState = Segment::TA2;
								}else{
									nextState = Segment::A2;
								}
								break;
		case Segment::TNW:		if(logicalDirection){
									if(turnouts[Turnout::TNW].turnDir == Turnout::straightOn){
										nextState = Segment::N1;
									}else{
										nextState = Segment::NW1;
									}
								}else{
									nextState = Segment::W1;
								}
								break;
		case Segment::TNO:		if(!logicalDirection){
									if(turnouts[Turnout::TNO].turnDir == Turnout::straightOn){
										nextState = Segment::N1;
									}else{
										nextState = Segment::TA1;
									}
								}else{
									nextState = Segment::O1;
								}
								break;
		case Segment::TA1:		if(!logicalDirection){
									if(turnouts[Turnout::TA1].turnDir == Turnout::straightOn){
										nextState = Segment::A1;
									}else{
										nextState = Segment::NO1;
									}
								}else{
									nextState = Segment::TNO;
								}
								break;
		case Segment::TSW:		if(!logicalDirection){
									if(turnouts[Turnout::TSW].turnDir == Turnout::straightOn){
										nextState = Segment::S1;
									}else{
										nextState = Segment::TA2;
									}
								}else{
									nextState = Segment::W1;
								}
								break;
		case Segment::TSO:		if(logicalDirection){
									if(turnouts[Turnout::TSO].turnDir == Turnout::straightOn){
										nextState = Segment::S1;
									}else{
										nextState = Segment::SO1;
									}
								}else{
									nextState = Segment::O1;
								}
								break;
		case Segment::TA2:		if(!logicalDirection){
									if(turnouts[Turnout::TA2].turnDir == Turnout::straightOn){
										nextState = Segment::A2;
									}else{
										nextState = Segment::SW1;
									}
								}else{
									nextState = Segment::TSW;
								}
								break;
		case Segment::TC:		switch(previousState){
								case Segment::NW1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::SO1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::SW1;
														}
														break;
								case Segment::SW1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::NO1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::NW1;
														}
														break;
								case Segment::NO1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::SW1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::SO1;
														}
														break;
								case Segment::SO1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::NW1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::NO1;
														}
														break;
								default:				nextState = Segment::TC;
														break;
								}
								break;

		default:				break;
		}
	}

	printf("CurrentState: \t %s\n", Segment::convertSegmentName(currentState)); fflush(stdout);
	printf("nextState: \t %s\n", Segment::convertSegmentName(nextState)); fflush(stdout);
	printf("driveStatus: \t %d\n", driveStatus);fflush(stdout);
	printf("logDirection: \t %d\n", logicalDirection);fflush(stdout);
	printf("direction: \t %d\n\n", direction);fflush(stdout);

	return nextState;
}
