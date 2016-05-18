/*
 * StateMaschine.cpp
 *
 *  Created on: May 11, 2016
 *      Author: misc0894
 */

#include "StateMaschine.h"

StateMaschine::StateMaschine() {
	setCircles();
}

StateMaschine::~StateMaschine() {
	// TODO Auto-generated destructor stub
}

void StateMaschine::updateState(int currentPosition) {

	previousState = currentState;
	currentState = (Segment::segmentID) currentPosition;

	if(previousState != currentState){
//		if(currentState != nextState){
//			logicalDirection = !logicalDirection;
//		}
		logicalDirection = updateDirection();
		nextState = predictedNextState();
	}
}

bool StateMaschine::updateDirection() {

	for(int i = 0; i < bigCircle.size(); i++){
		if(bigCircle.at(i).currentNode == currentState){
			if(bigCircle.at(i).previousNode == previousState){
				return true;
			}else{
				if(bigCircle.at(i).nextNode == previousState){
					return false;
				}else{
					break;
				}
			}
		}
	}
	for(int i = 0; i < leftCircle.size(); i++){
		if(leftCircle.at(i).currentNode == currentState){
			if(leftCircle.at(i).previousNode == previousState){
				return true;
			}else{
				if(leftCircle.at(i).nextNode == previousState){
					return false;
				}else{
					break;
				}
			}
		}
	}
	for(int i = 0; i < rightCircle.size(); i++){
		if(rightCircle.at(i).currentNode == currentState){
			if(rightCircle.at(i).previousNode == previousState){
				return true;
			}else{
				if(rightCircle.at(i).nextNode == previousState){
					return false;
				}else{
					break;
				}
			}
		}
	}
}

Segment::segmentID StateMaschine::predictedNextState() {
	Segment::segmentID nextState = Segment::DEFAULT;

	if(!driveStatus){
		nextState = currentState;
	}else{
		switch(currentState){
		case Segment::N1:		if(logicalDirection){
									nextState = Segment::N2;
								}else{
									nextState = Segment::TNW;
								}
								break;
		case Segment::N2:		if(logicalDirection){
									nextState = Segment::N3;
								}else{
									nextState = Segment::N1;
								}
								break;
		case Segment::N3:		if(logicalDirection){
									nextState = Segment::TNO;
								}else{
									nextState = Segment::N2;
								}
								break;
		case Segment::S1:		if(logicalDirection){
									nextState = Segment::TSW;
								}else{
									nextState = Segment::S2;
								}
								break;
		case Segment::S2:		if(logicalDirection){
									nextState = Segment::S1;
								}else{
									nextState = Segment::S3;
								}
								break;
		case Segment::S3:		if(logicalDirection){
									nextState = Segment::S2;
								}else{
									nextState = Segment::TSO;
								}
								break;
		case Segment::W1:		if(logicalDirection){
									nextState = Segment::TNW;
								}else{
									nextState = Segment::W2;
								}
								break;
		case Segment::W2:		if(logicalDirection){
									nextState = Segment::W1;
								}else{
									nextState = Segment::W3;
								}
								break;
		case Segment::W3:		if(logicalDirection){
									nextState = Segment::W2;
								}else{
									nextState = Segment::TSW;
								}
								break;
		case Segment::O1:		if(logicalDirection){
									nextState = Segment::O2;
								}else{
									nextState = Segment::TNO;
								}
								break;
		case Segment::O2:		if(logicalDirection){
									nextState = Segment::O3;
								}else{
									nextState = Segment::O1;
								}
								break;
		case Segment::O3:		if(logicalDirection){
									nextState = Segment::TSO;
								}else{
									nextState = Segment::O2;
								}
								break;
		case Segment::NW1:		if(logicalDirection){
									nextState = Segment::NW2;
								}else{
									nextState = Segment::TNW;
								}
								break;
		case Segment::NW2:		if(logicalDirection){
									nextState = Segment::NW3;
								}else{
									nextState = Segment::NW1;
								}
								break;
		case Segment::NW3:		if(logicalDirection){
									nextState = Segment::TC;
								}else{
									nextState = Segment::NW2;
								}
								break;
		case Segment::NO1:		if(logicalDirection){
									nextState = Segment::TA1;
								}else{
									nextState = Segment::NO2;
								}
								break;
		case Segment::NO2:		if(logicalDirection){
									nextState = Segment::NO1;
								}else{
									nextState = Segment::TC;
								}
								break;
		case Segment::SW1:		if(logicalDirection){
									nextState = Segment::SW2;
								}else{
									nextState = Segment::TC;
								}
								break;
		case Segment::SW2:		if(logicalDirection){
									nextState = Segment::TA2;
								}else{
									nextState = Segment::SW1;
								}
								break;
		case Segment::SO1:		if(logicalDirection){
									nextState = Segment::TC;
								}else{
									nextState = Segment::SO2;
								}
								break;
		case Segment::SO2:		if(logicalDirection){
									nextState = Segment::SO1;
								}else{
									nextState = Segment::SO3;
								}
								break;
		case Segment::SO3:		if(logicalDirection){
									nextState = Segment::SO2;
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
										nextState = Segment::N3;
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
									nextState = Segment::W3;
								}
								break;
		case Segment::TSO:		if(logicalDirection){
									if(turnouts[Turnout::TSO].turnDir == Turnout::straightOn){
										nextState = Segment::S3;
									}else{
										nextState = Segment::SO3;
									}
								}else{
									nextState = Segment::O3;
								}
								break;
		case Segment::TA2:		if(!logicalDirection){
									if(turnouts[Turnout::TA2].turnDir == Turnout::straightOn){
										nextState = Segment::A2;
									}else{
										nextState = Segment::SW2;
									}
								}else{
									nextState = Segment::TSW;
								}
								break;
		case Segment::TC:		switch(previousState){
								case Segment::NW3:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::SO1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::SW1;
														}
														break;
								case Segment::SW1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::NO2;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::NW3;
														}
														break;
								case Segment::NO2:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::SW1;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::SO1;
														}
														break;
								case Segment::SO1:		if(turnouts[Turnout::TC].turnDir == Turnout::straightOn){
															nextState = Segment::NW3;
															logicalDirection = !logicalDirection;
														}else{
															nextState = Segment::NO2;
														}
														break;
								default:				nextState = Segment::TC;
														break;
								}
								break;

		default:				break;
		}
	}

	printf("PreviousState: \t %s\n", Segment::convertSegmentName(previousState)); fflush(stdout);
	printf("CurrentState: \t %s\n", Segment::convertSegmentName(currentState)); fflush(stdout);
	printf("nextState: \t %s\n", Segment::convertSegmentName(nextState)); fflush(stdout);
	printf("driveStatus: \t %d\n", driveStatus);fflush(stdout);
	printf("logDirection: \t %d\n", logicalDirection);fflush(stdout);
	printf("direction: \t %d\n\n", direction);fflush(stdout);

	return nextState;
}

void StateMaschine::setCircles() {
	Node node;

	node.previousNode = Segment::TSW;
	node.currentNode = Segment::W3;
	node.nextNode = Segment::W2;
	bigCircle.push_back(node);
	leftCircle.push_back(node);

	node.previousNode = Segment::W3;
	node.currentNode = Segment::W2;
	node.nextNode = Segment::W1;
	bigCircle.push_back(node);
	leftCircle.push_back(node);

	node.previousNode = Segment::W2;
	node.currentNode = Segment::W1;
	node.nextNode = Segment::TNW;
	bigCircle.push_back(node);
	leftCircle.push_back(node);

	node.previousNode = Segment::W1;
	node.currentNode = Segment::TNW;
	node.nextNode = Segment::N1;
	bigCircle.push_back(node);

	node.previousNode = Segment::TNW;
	node.currentNode = Segment::N1;
	node.nextNode = Segment::N2;
	bigCircle.push_back(node);

	node.previousNode = Segment::N1;
	node.currentNode = Segment::N2;
	node.nextNode = Segment::N3;
	bigCircle.push_back(node);

	node.previousNode = Segment::N2;
	node.currentNode = Segment::N3;
	node.nextNode = Segment::TNO;
	bigCircle.push_back(node);

	node.previousNode = Segment::N3;
	node.currentNode = Segment::TNO;
	node.nextNode = Segment::O1;
	bigCircle.push_back(node);

	node.previousNode = Segment::TNO;
	node.currentNode = Segment::O1;
	node.nextNode = Segment::O2;
	bigCircle.push_back(node);
	rightCircle.push_back(node);

	node.previousNode = Segment::O1;
	node.currentNode = Segment::O2;
	node.nextNode = Segment::O3;
	bigCircle.push_back(node);
	rightCircle.push_back(node);

	node.previousNode = Segment::O2;
	node.currentNode = Segment::O3;
	node.nextNode = Segment::TSO;
	bigCircle.push_back(node);
	rightCircle.push_back(node);

	node.previousNode = Segment::O3;
	node.currentNode = Segment::TSO;
	node.nextNode = Segment::S3;
	bigCircle.push_back(node);

	node.previousNode = Segment::TSO;
	node.currentNode = Segment::S3;
	node.nextNode = Segment::S2;
	bigCircle.push_back(node);

	node.previousNode = Segment::S3;
	node.currentNode = Segment::S2;
	node.nextNode = Segment::S1;
	bigCircle.push_back(node);

	node.previousNode = Segment::S2;
	node.currentNode = Segment::S1;
	node.nextNode = Segment::TSW;
	bigCircle.push_back(node);

	node.previousNode = Segment::S1;
	node.currentNode = Segment::TSW;
	node.nextNode = Segment::W3;
	bigCircle.push_back(node);


	node.previousNode = Segment::W1;
	node.currentNode = Segment::TNW;
	node.nextNode = Segment::NW1;
	leftCircle.push_back(node);

	node.previousNode = Segment::TNW;
	node.currentNode = Segment::NW1;
	node.nextNode = Segment::NW2;
	leftCircle.push_back(node);

	node.previousNode = Segment::NW1;
	node.currentNode = Segment::NW2;
	node.nextNode = Segment::NW3;
	leftCircle.push_back(node);

	node.previousNode = Segment::NW2;
	node.currentNode = Segment::NW3;
	node.nextNode = Segment::TC;
	leftCircle.push_back(node);

	node.previousNode = Segment::NW3;
	node.currentNode = Segment::TC;
	node.nextNode = Segment::SW1;
	leftCircle.push_back(node);

	node.previousNode = Segment::TC;
	node.currentNode = Segment::SW1;
	node.nextNode = Segment::SW2;
	leftCircle.push_back(node);

	node.previousNode = Segment::SW1;
	node.currentNode = Segment::SW2;
	node.nextNode = Segment::TA2;
	leftCircle.push_back(node);

	node.previousNode = Segment::SW2;
	node.currentNode = Segment::TA2;
	node.nextNode = Segment::TSW;
	leftCircle.push_back(node);

	node.previousNode = Segment::TA2;
	node.currentNode = Segment::TSW;
	node.nextNode = Segment::W3;
	leftCircle.push_back(node);

	node.previousNode = Segment::O3;
	node.currentNode = Segment::TSO;
	node.nextNode = Segment::SO3;
	rightCircle.push_back(node);

	node.previousNode = Segment::TSO;
	node.currentNode = Segment::SO3;
	node.nextNode = Segment::SO2;
	rightCircle.push_back(node);

	node.previousNode = Segment::SO3;
	node.currentNode = Segment::SO2;
	node.nextNode = Segment::SO1;
	rightCircle.push_back(node);

	node.previousNode = Segment::SO2;
	node.currentNode = Segment::SO1;
	node.nextNode = Segment::TC;
	rightCircle.push_back(node);

	node.previousNode = Segment::SO1;
	node.currentNode = Segment::TC;
	node.nextNode = Segment::NO2;
	rightCircle.push_back(node);

	node.previousNode = Segment::TC;
	node.currentNode = Segment::NO2;
	node.nextNode = Segment::NO1;
	rightCircle.push_back(node);

	node.previousNode = Segment::NO2;
	node.currentNode = Segment::NO1;
	node.nextNode = Segment::TA1;
	rightCircle.push_back(node);

	node.previousNode = Segment::NO1;
	node.currentNode = Segment::TA1;
	node.nextNode = Segment::TNO;
	rightCircle.push_back(node);

	node.previousNode = Segment::TA1;
	node.currentNode = Segment::TNO;
	node.nextNode = Segment::O1;
	rightCircle.push_back(node);
}
