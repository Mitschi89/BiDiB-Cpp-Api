//============================================================================
// Name        	: Segment.cpp
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: creates a segment class with segment IDs
//============================================================================

#include "Segment.h"

Segment::Segment() {
	// TODO Auto-generated constructor stub

}

Segment::~Segment() {
	// TODO Auto-generated destructor stub
}

char* Segment::convertSegmentName(Segment::segmentID segmentID) {
	char * segmentName;

	switch(segmentID){
	case N1:	segmentName = "N1"; break;
	case N2:	segmentName = "N2"; break;
	case N3:	segmentName = "N3"; break;
	case W1:	segmentName = "W1"; break;
	case W2:	segmentName = "W2"; break;
	case W3:	segmentName = "W3"; break;
	case S1:	segmentName = "S1"; break;
	case S2:	segmentName = "S2"; break;
	case S3:	segmentName = "S3"; break;
	case O1:	segmentName = "O1"; break;
	case O2:	segmentName = "O2"; break;
	case O3:	segmentName = "O3"; break;
	case A1:	segmentName = "A1"; break;
	case A2:	segmentName = "A2"; break;
	case NW1:	segmentName = "NW1"; break;
	case NW2:	segmentName = "NW2"; break;
	case NW3:	segmentName = "NW3"; break;
	case SW1:	segmentName = "SW1"; break;
	case SW2:	segmentName = "SW2"; break;
	case SO1:	segmentName = "SO1"; break;
	case SO2:	segmentName = "SO2"; break;
	case SO3:	segmentName = "SO3"; break;
	case NO1:	segmentName = "NO1"; break;
	case NO2:	segmentName = "NO2"; break;
	case TNW:	segmentName = "TNW"; break;
	case TNO:	segmentName = "TNO"; break;
	case TSW:	segmentName = "TSW"; break;
	case TSO:	segmentName = "TSO"; break;
	case TA1:	segmentName = "TA1"; break;
	case TA2:	segmentName = "TA2"; break;
	case TC:	segmentName = "TC"; break;
	case DEFAULT:	segmentName = "DEFAULT"; break;
	}
	return segmentName;
}
