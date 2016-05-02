/*
 * Loc.h
 *
 *  Created on: 17.02.2016
 *      Author: Steffi
 */

#ifndef LOC_H_
#define LOC_H_

#include <stdint.h>


class Loc {
public:
	int id = 0;
	uint32_t position = 0;
	int speed = 0;
	bool direction = 0; //true == forward
	bool functionState[5] = {false};
	bool lightState = false;

	Loc();
	~Loc();
};

#endif /* LOC_H_ */
