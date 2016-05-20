//============================================================================
// Name        	: Loc.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: creates a Loc functions
//============================================================================

#ifndef LOCFUNCTION_H_
#define LOCFUNCTION_H_

class locFunction {
public:

	enum locFunctionID {
		F1 = 1, F2 = 2, F3 = 3, F4 = 4, FLight = 0
	} ID;

	locFunction();
	virtual ~locFunction();
};

#endif /* LOCFUNCTION_H_ */
