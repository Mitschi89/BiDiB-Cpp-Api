/*
 * locFunction.h
 *
 *  Created on: May 2, 2016
 *      Author: misc0894
 */

#ifndef LOCFUNCTION_H_
#define LOCFUNCTION_H_

class locFunction {
public:

	enum locFunctionID{
		F1 = 1,
		F2 = 2,
		F3 = 3,
		F4 = 4,
		FLight = 0
	} ID;

	locFunction();
	virtual ~locFunction();
};

#endif /* LOCFUNCTION_H_ */
