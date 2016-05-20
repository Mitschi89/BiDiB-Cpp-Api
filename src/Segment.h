//============================================================================
// Name        	: Segment.h
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: creates a segment class with segment IDs
//============================================================================

#ifndef SEGMENT_H_
#define SEGMENT_H_

class Segment {
public:

	enum segmentID {
		A1 = 0,
		N2 = 1,
		N3 = 2,
		NO1 = 3,
		TA1 = 4,
		TNO = 5,
		O1 = 6,
		O2 = 7,
		O3 = 8,
		NO2 = 9,
		SO1 = 10,
		SO2 = 11,
		S3 = 12,
		SO3 = 13,
		TSO = 14,
		DEFAULT = 15,
		NW3 = 16,
		TC = 17,
		SW1 = 18,
		A2 = 19,
		S2 = 20,
		SW2 = 21,
		TA2 = 22,
		S1 = 23,
		NW2 = 24,
		N1 = 25,
		NW1 = 26,
		TNW = 27,
		W1 = 28,
		W2 = 29,
		W3 = 30,
		TSW = 31
	} ID = DEFAULT;

	Segment();
	virtual ~Segment();

	static char* convertSegmentName(Segment::segmentID);
};

#endif /* SEGMENT_H_ */
