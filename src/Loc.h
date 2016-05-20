//============================================================================
// Name        	: Loc.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: creates a Loc class for saving informations about a loc
//============================================================================

#ifndef LOC_H_
#define LOC_H_

#include <stdint.h>
#include "StateMaschine.h"

class Loc {
public:
	int id = 0;
	uint32_t position = 0;
	int speed = 0;
	bool direction = 0; //true == forward
	bool functionState[4] = { false };
	bool lightState = false;

	StateMaschine stateMaschine;

	Loc();
	~Loc();
};

#endif /* LOC_H_ */
