//============================================================================
// Name        	: BiDiBMessageHandler.cpp
// Author      	: Michael Scharfenberg
// Version  	: 1.0
// Date			: 06.05.2016
// Description 	: processes all incoming data and saves them, sends data via serial connection
//============================================================================

#include "BiDiBMessageHandler.h"

BiDiBMessageHandler::BiDiBMessageHandler() {
	serialPort = NULL;
	if(!this->initSerialPort("/dev/ttyUSB0")){
		connected = -1;
		return;
	}

	receiverThread = std::thread(&BiDiBMessageHandler::getMessage, this);
	if(!initBidib()){
		connected = -1;
		return;
	}

	for(int i = 0; i < MAXNUMBEROFTURNOUTS; i++){
		turnouts[i].ID = (Turnout::turnoutID) i;
	}
	for(int i = 0; i < MAXNUMBEROFSEGEMENTSWITHLOC; i++){
		for(int k = 0; k < MAXNUMBEROFTURNOUTS; k++){
			locs[i].stateMaschine.turnouts[k] = turnouts[k];
		}
	}

	connected = 1;
}

BiDiBMessageHandler::~BiDiBMessageHandler() {
	serialPort = NULL;
	receiverThread.join();
	delete serialPort;
}


bool BiDiBMessageHandler::initSerialPort(const char* portName) {
	serialPort = new Serial (portName);
	if (!serialPort->IsConnected()){
		printf("Could NOT establish a serial connection. Check COM-Port! See BiDiBMessageHandler Constructor\n");
		return false;
	}else{
		printf("Serial Connection established\n");
		return true;
	}
	fflush(stdout);
}

bool BiDiBMessageHandler::initSerialPort(int comPortNumber) {
	serialPort = new Serial (convertComPortNumber(comPortNumber));
	if (!serialPort->IsConnected()){
		printf("Could NOT establish a serial connection. Check COM-Port! See BiDiBMessageHandler Constructor\n");
		return false;
	}else{
		printf("Serial Connection established\n");
		return true;
	}
	fflush(stdout);
}

bool BiDiBMessageHandler::initBidib() {
	printf("Initializing BiDiB-Protocol. "); fflush(stdout);

	sendSystemMessage(gbmMasterID, MSG_SYS_GET_MAGIC);
	printf(". "); fflush(stdout);
//		printf("Magic gesendet... num: %X\n", msgNum);
	usleep(1000*100);

	sendSystemMessage(gbmMasterID, MSG_SYS_RESET);
	printf(". "); fflush(stdout);
	usleep(1000*1100);
//		printf("Reset gesendet... num: %X\n");

	sendNodeTabMessage();
	printf(". "); fflush(stdout);
//		printf("Get_NodeTab gesendet... num: %X\n", msgNum);
	usleep(1000*100);

	if(nodeCount != NODECOUNT){
		return false;
	}else{
		sendSystemMessage(oneControlID, MSG_SYS_RESET);
		sendSystemMessage(oneOcID, MSG_SYS_RESET);
		usleep(1000*1100);
	}

	sendSystemMessage(gbmMasterID, MSG_SYS_ENABLE);
	printf(". "); fflush(stdout);
//		printf("Sys_enable gesendet... num: %X\n", msgNum);
	usleep(1000*100);

	sendGetLocsMessage();
	printf(". \n"); fflush(stdout);
//		printf("getLocs gesendet... num: %X\n", msgNum);

	sendGetSwitchesMessage();
	printf(". \n"); fflush(stdout);
//		printf("getSwitches gesendet... num: %X\n", msgNum);

	fflush(stdout);

	return true;
}

int BiDiBMessageHandler::isConnected() {
	return connected;
}

const char* BiDiBMessageHandler::convertComPortNumber(int comPortNumber) {
	switch (comPortNumber){
		case 0: return "/dev/ttyS0";
		case 1: return "/dev/ttyS1";
		case 2: return "/dev/ttyS2";
		case 3: return "/dev/ttyS3";
		case 4: return "/dev/ttyS4";
		case 5: return "/dev/ttyS5";
		case 6: return "/dev/ttyS6";
		case 7: return "/dev/ttyS7";
		case 8: return "/dev/ttyS8";
		case 9: return "/dev/ttyS9";
		case 10: return "/dev/ttyS10";
		case 11: return "/dev/ttyS11";
		case 12: return "/dev/ttyS12";
		case 13: return "/dev/ttyS13";
		case 14: return "/dev/ttyS14";
		case 15: return "/dev/ttyS15";
		case 16: return "/dev/ttyS16";
		case 17: return "/dev/ttyS17";
		case 18: return "/dev/ttyS18";
		case 19: return "/dev/ttyS19";

		default: return "/dev/ttyS10";
	}
}

void BiDiBMessageHandler::getMessage() {
	unsigned char incomingData [256];
	unsigned char messageData[256];
	int messageLength = 0;
	unsigned int maxReadingBytes = 255;
	int bytesRead = 0;
	int escapeHot = 0;
	int rxCRC = 0;

	while(serialPort->IsConnected() && serialPort != NULL){
		bytesRead = serialPort->ReadData(incomingData, maxReadingBytes);

		for (int index = 0 ; index < bytesRead; index++){
			if(incomingData[index] == BIDIB_PKT_MAGIC){
				if(rxCRC){
					messageLength = 0;
					printf("INCOMING CRC ERROR !!!\n"); fflush(stdout);
				}else{
					processMessage(messageData, messageLength-1);
					messageLength = 0;
				}
			}else{
				if(incomingData[index] == BIDIB_PKT_ESCAPE){
					escapeHot = 1;
				}else{
					if (escapeHot){
						messageData[messageLength] = incomingData[index] ^ 0x20;
						escapeHot = 0;
					}else{
						messageData[messageLength] = incomingData[index];
					}
					rxCRC = crc_array[messageData[messageLength] ^ rxCRC];
					messageLength++;
				}
			}
		}
	}
	return;
}

void BiDiBMessageHandler::sendMessage(unsigned char* message) {
	unsigned char rawMessage [256];
	int index = 0;
	int txCRC = 0;
	int length = message[0];
	rawMessage[index++] = BIDIB_PKT_MAGIC;

	for(int i = 0; i <= length; i++){
		if ((message[i] == BIDIB_PKT_MAGIC) || (message[i] == BIDIB_PKT_ESCAPE) ){
			rawMessage[index++] = BIDIB_PKT_ESCAPE;       											// escape this char
		    rawMessage[index]= message[i] ^ 0x20;                           						// 'veraendern'
		}else{
			rawMessage[index]= message[i];
		}
		txCRC = crc_array[rawMessage[index] ^ txCRC];
		index++;
	}
	rawMessage[index++] = txCRC;
	rawMessage[index] = BIDIB_PKT_MAGIC;

	serialPort->WriteData(rawMessage, index + 1);

	return;
}

char* BiDiBMessageHandler::getMessageType(int type) {
	switch(type){
		case MSG_SYS_MAGIC: 		return "MSG_SYS_MAGIC";
		case MSG_BOOST_DIAGNOSTIC:	return "MSG_BOOST_DIAGNOSTIC";
		case MSG_NODETAB_COUNT: 	return "MSG_NODETAB_COUNT";
		case MSG_NODETAB:			return "MSG_NODETAB";
		case MSG_SYS_ERROR:			return "MSG_SYS_ERROR";
		case MSG_BM_MULTIPLE:		return "MSG_BM_MULTIPLE ";		//liefert StartAdresse der Gleise,
																	//Anzahl der folgenden bits für belegte Gleise,
																	//bitweise kodierte belegte/frei Gleise
		case MSG_BM_OCC:			return "MSG_BM_OCC"; 			//liefert Adresse des Gleises, dass belegt ist
		case MSG_BM_FREE:			return "MSG_BM_FREE ";			//liefert Adresse des Gleises, dass wieder frei ist
		case MSG_BM_ADDRESS:		return "MSG_BM_ADDRESS ";		//liefert Adresse des Gleises, kurze Adresse, lange Adress der loc
		case MSG_CS_DRIVE_ACK: 		return "MSG_CS_DRIVE_ACK ";
		case MSG_FEATURE:			return "MSG_FEATURE";
		case MSG_FEATURE_COUNT:		return "MSG_FEATURE_COUNT";
		case MSG_NODE_NA:			return "MSG_NODE_NA";
		case MSG_LC_NA:				return "MSG_LC_NA";
		case MSG_LC_STAT:			return "MSG_LC_STAT";
		case MSG_BM_DYN_STATE:		return "MSG_BM_DYN_STATE";
		case MSG_BM_CONFIDENCE:		return "MSG_BM_CONFIDENCE";

		default: 					return "unknown Message Type";
	}
}

int BiDiBMessageHandler::processMessage(unsigned char *message, int length){
	int maxLength = 0;

	do{
		int msgLen = message[0 + maxLength] + 1;

		unsigned char msg [msgLen];

		for (int i = 0; i < msgLen; i++){
			msg[i] = message[i + maxLength];
		}

		int messageOffset = 0;
		if (msg[1] != 0x00){
			messageOffset = 1;
		}

		switch(msg[3 + messageOffset]){
			case MSG_BM_ADDRESS:			if(SHOWBMMessages){
												printMessage(msg);
											}
											processBM(msg);
											break;

			case MSG_BM_OCC:
			case MSG_BM_FREE:				if(SHOWALLBMMESSAGES){
												printMessage(msg);
											}
											processBM(msg);
											break;

			case MSG_BM_MULTIPLE:			if(SHOWBMMULTI){
												printMessage(msg);
											}
											processBM(msg);
											break;
			case MSG_FEATURE_COUNT:
			case MSG_FEATURE: 				if(SHOWFEATURE){
												printMessage(msg);
											}
											processFeature(msg);
											break;
			case MSG_SYS_MAGIC:
			case MSG_BOOST_DIAGNOSTIC:		if(SHOWDIAGNOSTIC & SHOWSYSMESSAGES){
												printMessage(msg);
											}
											processOther(msg);
											break;

			case MSG_LC_NA:
			case MSG_LC_STAT:				if(SHOWTURNOUTSTATE){
												printMessage(msg);
											}
											processTurnoutStateMessage(msg);
											break;
			case MSG_NODE_NA:
			case MSG_NODETAB_COUNT:
			case MSG_NODETAB:				if(SHOWNODETAB){
												printMessage(msg);
											}
											processNodeTabMessage(msg);
											break;
			case MSG_SYS_ERROR:				if(SHOWERRORMESSAGE){
												printMessage(msg);
												processErrorMessage(msg);
											}
											break;
			case MSG_BM_DYN_STATE:
			case MSG_BM_CONFIDENCE:
			case MSG_CS_DRIVE_ACK: 			if(SHOWSYSMESSAGES){
												printMessage(msg);
											}
											processOther(msg);
											break;

			default: 						if(SHOWSYSMESSAGES){
												printMessage(msg);
											}
											break;
		}
		maxLength += msgLen;
	}while(length > maxLength);
}

int BiDiBMessageHandler::processBM(unsigned char* message) {
	int messageOffset = 0;
	if (message[1] != gbmMasterID){
		if(message[1] == oneOcID){
			messageOffset = 1;
		}else{
			return -1;
		}
	}

	if(message[3 + messageOffset] == MSG_BM_FREE){
		if(!messageOffset){
//			printf("Position: %d free \n", message[4 + messageOffset]);
		}else{
			if(FAULTSON){
				processFaultMessage(message);
			}
		}
	}

	if(message[3 + messageOffset] == MSG_BM_OCC){
		if(!messageOffset){
//			printf("Position: %d occupied \n", message[4 + messageOffset]);
		}else{
			if(FAULTSON){
				processFaultMessage(message);
			}
		}
	}

	fflush(stdout);

	if(message[3 + messageOffset] == MSG_BM_ADDRESS){
		if(!messageOffset){
			processlocMessage(message);
		}else{
			if(FAULTSON){
				processFaultMessage(message);
			}
		}
	}

	if(message[3 + messageOffset] == MSG_BM_MULTIPLE){
		if(!messageOffset){
			locAllPositions = 0;
			locAllPositions += message[9] << 24;
			locAllPositions += message[8] << 16;
			locAllPositions += message[7] << 8;
			locAllPositions += message[6];
		}else{
			if(FAULTSON){
				processFaultMessage(message);
			}
		}
	}

	return 0;
}

int BiDiBMessageHandler::processOther(unsigned char* message) {

	int messageOffset = 0;
	if (message[1] != gbmMasterID){
		messageOffset = 1;
	}

	return 0;
}

int BiDiBMessageHandler::processFeature(unsigned char* message) {
	int messageOffset = 0;
	if(message[1] != gbmMasterID){
		messageOffset = 1;
	}

	if(message[3 + messageOffset] == MSG_FEATURE_COUNT){
		featureCount = message[4 + messageOffset];
	}
}

int BiDiBMessageHandler::processlocMessage(unsigned char* message) {
	int locPosition = message[4];
	int locAddress = message[5];
	int locIndex = -1;

	if(locAddress == 0x00){
		for(int i = 0; i < locCount; i++){
			if((locs[i].position >> locPosition) & 1){
				setLocPosition(i, locPosition, false);
			}
		}
	}else{
		for(int i = 0; i < locCount; i++){
			if(locAddress == locs[i].id){
				locIndex = i;
			}
		}

		if(locIndex == -1){
			locIndex = locCount;
			locs[locIndex].id = locAddress;
			locCount++;
		}
		setLocPosition(locIndex, locPosition, true);
	}
}

int BiDiBMessageHandler::processFunctionMessage(unsigned char* message) {
}

int BiDiBMessageHandler::processFaultMessage(unsigned char* message) {

	int messageOffset = 0;
	if (message[1] == oneOcID){
		messageOffset = 1;
	}else{
		return -1;
	}

	if(message[3 + messageOffset] == MSG_BM_OCC){
		int switchID = message[4 + messageOffset];
		bool active = false;
		setFault(switchID, active);
	}

	if(message[3 + messageOffset] == MSG_BM_FREE){
		int switchID = message[4 + messageOffset];
		bool active = true;
		setFault(switchID, active);
	}

	if(message[3 + messageOffset] == MSG_BM_MULTIPLE){
		for(int byteNumber = 0; byteNumber < 3; byteNumber++){
			int switches = message[6 + messageOffset + byteNumber];
			for(int i = 0; i < 8; i++){
				bool active = (switches & (1 << i)) >> i;
				int switchID = i + 8*byteNumber;

				setFault(switchID, !active);
			}
		}
	}
}

int BiDiBMessageHandler::setFault(int switchID, bool active) {
	//switch: 8, 10, 11, 12, 14, 16
	//button: 7, 9, 13, 15, 17, 18
	int position = -1;

	switch (switchID){
			case 8: 	fault[0] = active; break;
			case 10:	fault[1] = active; break;
			case 11:	fault[2] = active; break;
			case 12:	fault[3] = active; break;
			case 14:	fault[4] = active; break;
			case 16:	fault[5] = active; break;

			case 7:		position = 8; break;
			case 9:		position = 13; break;
			case 13:	position = 30; break;
			case 15:	position = 21; break;
			case 17:	position = 23; break;
			case 18: 	position = 12; break;
		}
		if(position != -1){
			for(int id = 0; id < locCount; id++){
				setLocPosition(id, position, active);
			}
		}
}

int BiDiBMessageHandler::setLocPosition(int id, int position, bool occupied) {
	bool activeFault = false;
	if(FAULTSON){
		switch(position){
			case 8:  	activeFault = fault[0]; break;
			case 13:  	activeFault = fault[1]; break;
			case 12:  	activeFault = fault[2]; break;
			case 30:  	activeFault = fault[3]; break;
			case 21:  	activeFault = fault[4]; break;
			case 23:  	activeFault = fault[5]; break;

		}
	}

	if(!activeFault){
		if(occupied){
			locs[id].stateMaschine.updateState(position);
			locs[id].position |= (0x01 << position);
		}else{
			locs[id].position &= ~(0x01 << position);
		}
	}
}

int BiDiBMessageHandler::processTurnoutStateMessage(unsigned char* message) {
	int messageOffset = 0;
	if (message[1] == oneControlID){
		messageOffset = 1;
	}else{
		return -1;
	}

	if(message[3 + messageOffset] == MSG_LC_NA){
		int port = message[5 + messageOffset];
		printf("Turnout port: %d not available. Check cable connection", port);
	}

	if(message[3 + messageOffset] == MSG_LC_STAT){
		int address = message[5 + messageOffset];
		int direction = address % 2; // 1 - straight, 0 - turn

		int turnID = (address - direction - 8) / 2;

		turnouts[turnID].turnDir = (Turnout::turnDirection) direction;

		for(int i = 0; i < MAXNUMBEROFSEGEMENTSWITHLOC; i++){
			locs[i].stateMaschine.turnouts[turnID].turnDir = turnouts[turnID].turnDir;
		}

	}
}

int BiDiBMessageHandler::processNodeTabMessage(unsigned char* message) {

	if(message[3] == MSG_NODETAB_COUNT){
		nodeCount = message[4];
		if(nodeCount == 0){
			sendNodeTabMessage(); printf("another try to get NodeTab...\n"); fflush(stdout);
		}
		if(nodeCount != NODECOUNT){
			printf("NOT ALL NODES AVAILABLE. CHECK CABLE CONNECTION OR PRESS RESET!\n");
		}
	}

	if(message[3] == MSG_NODETAB){
		int entryNumber = message[5];
		int ID = message[11];

		switch(ID){
			case 0xDE:		gbmMasterID = entryNumber;
							break;
			case 0x76:		oneControlID = entryNumber;
							break;
			case 0x73:		oneOcID = entryNumber;
							break;
			default:		printf("Node not recognized. Make sure Node is specified in code. See processNodeTabMessage().\n");
		}
	}

	int messageOffset = 0;
		if (message[1] != gbmMasterID){
			messageOffset = 1;
		}

	if(message[3 + messageOffset] == MSG_NODE_NA){
		int entryNumber = message[4 + messageOffset];

		if(entryNumber == gbmMasterID){
			printf("GBM Master not available.\n");
		}
		if(entryNumber == oneControlID){
			printf("OneControl not available.\n");
		}
		if(entryNumber == oneOcID){
			printf("OneOC not available.\n");
		}
		if((entryNumber != gbmMasterID) && (entryNumber !=  oneControlID) && (entryNumber !=  oneOcID)){
			printf("Unknown Node not available.\n");
		}
	}
}

int BiDiBMessageHandler::processErrorMessage(unsigned char* message) {
	int messageOffset = 0;
	if (message[1] != gbmMasterID){
		messageOffset = 1;
	}

	int errorType = message[4 + messageOffset];

	switch(errorType){
		case BIDIB_ERR_NONE:		printf("NO ERROR\n");
									break;
		case BIDIB_ERR_TXT:			printf("Text ERROR\n");
									break;
		case BIDIB_ERR_CRC:			printf("CRC ERROR\n");
									break;
		case BIDIB_ERR_SIZE:		printf("Size ERROR\n");
									break;
		case BIDIB_ERR_SEQUENCE:	printf("Sequence ERROR\n");
									break;
		case BIDIB_ERR_BUS:			printf("Bus ERROR\n");
									break;
		case BIDIB_ERR_OVERRUN:		printf("Overrun ERROR\n");
									break;
		case BIDIB_ERR_HW:			printf("Hardware ERROR\n");
									break;
		case BIDIB_ERR_SUBTIME:		printf("SubNode Message Timeout ERROR\n");
									break;
		default:					printf("Unknown ERROR\n");

	}
}

void BiDiBMessageHandler::printMessage(unsigned char* message) {
	int messageOffset = 0;
	if(message[1] != 0x00){
		messageOffset = 1;
	}

	int msgLen = message[0];

	printf("Receive: ");
	for (int i =0 ; i < msgLen + 1; i++){
		printf("%02X ",message[i]);
	}
	printf(" %s", getMessageType(message[3 + messageOffset]));
	printf("\n");
	fflush(stdout);
}

void BiDiBMessageHandler::sendSystemMessage(int node, char bidibMessageID) {

	unsigned char message[5];

	switch(bidibMessageID){
		case MSG_SYS_RESET:

		case MSG_SYS_GET_MAGIC:		if(node == gbmMasterID){
										message[0] = 3;
										message[1] = gbmMasterID;
										message[2] = msgNum [node] = 0;
										message[3] = bidibMessageID;
										break;
									}else{
										message[0] = 4;
										message[1] = node;
										message[2] = gbmMasterID;
										message[3] = msgNum[node] = 0;
										message[4] = bidibMessageID;
										break;
									}


		case MSG_NODETAB_GETALL:

		case MSG_DBM:

		case MSG_SYS_DISABLE:

		case MSG_SYS_ENABLE:		message[0] = 3;
									message[1] = gbmMasterID;
									message[2] = ++msgNum[node];
									message[3] = bidibMessageID;
									break;

		default:					return;
	}

	sendMessage(message);
}

void BiDiBMessageHandler::sendDriveMessage(int locID) {
	int speed = locs[locID].speed;
	int direction = locs[locID].direction;
	speed = speed | (direction << 7); //MSB = direction

	int functions = 0;
	for (int i = 0; i < 4 ; i++){
		functions |= locs[locID].functionState[i] << i;
	}
	functions |= locs[locID].lightState << 4;

	unsigned char message[] = {
				12,
				gbmMasterID,
				++msgNum[gbmMasterID],
				MSG_CS_DRIVE,
				locs[locID].id, //address low of loc
				0x00, //address high of loc
				BIDIB_CS_DRIVE_FORMAT_DCC128, //128 speed steps 0
				3, //output active ...first bit -> speed output is active, second bit -> F1-F4, FL active, the other function outputs are not active
				speed, // 0 = halt, 1= Nothalt, 2-127
				functions,
				0,
				0,
				0 //functions active
	};

	sendMessage(message);
}

void BiDiBMessageHandler::sendDriveMessage(int locID, int speed, bool direction) {

	if(speed > 127){
		speed = 127;
	}

	locs[locID].speed = speed;
	locs[locID].direction = direction;

	if(locs[locID].stateMaschine.direction == locs[locID].stateMaschine.logicalDirection){
		locs[locID].stateMaschine.direction = direction;
		locs[locID].stateMaschine.logicalDirection = direction;
	}else{
		locs[locID].stateMaschine.direction = direction;
		locs[locID].stateMaschine.logicalDirection = !direction;
	}

	if((speed == 0) || (speed == 1)){
		locs[locID].stateMaschine.driveStatus = false;
		locs[locID].stateMaschine.predictedNextState();
	}else{
		locs[locID].stateMaschine.driveStatus = true;
	}

	sendDriveMessage(locID);
}

void BiDiBMessageHandler::sendFunctionStateMessage(int locID, int functionNumber, bool functionState) {

	if(functionNumber == locFunction::FLight){ //light
		locs[locID].lightState = functionState;
	}else{ //F1 - F4
		locs[locID].functionState[functionNumber - 1] = functionState;
	}

	sendDriveMessage(locID);
}

void BiDiBMessageHandler::sendStateMessage(char bidibStateID) {

	unsigned char message[5];

	switch(bidibStateID){
		case BIDIB_CS_STATE_GO:
		case BIDIB_CS_STATE_STOP:
		case BIDIB_CS_STATE_OFF: 	message[0] = 4;
									message[1] = gbmMasterID;
									message[2] = ++msgNum[gbmMasterID];
									message[3] = MSG_CS_SET_STATE;
									message[4] = bidibStateID;
									break;

		default:					return;

	}

	sendMessage(message);
}

void BiDiBMessageHandler::sendFeatureMessage(int node) {
	if(node == gbmMasterID){
		unsigned char message[] = {
					3,
					gbmMasterID,
					++msgNum[node],
					MSG_FEATURE_GETALL
		};
		sendMessage(message);

		usleep(1000*1000);
		for (int i = 0; i < featureCount; i++){
			unsigned char message[] = {
						3,
						node,
						++msgNum[node],
						MSG_FEATURE_GETNEXT
			};
			sendMessage(message);
			usleep(50*1000);
		}
	}else{
		unsigned char message[] = {
					4,
					node,
					gbmMasterID,
					++msgNum[node],
					MSG_FEATURE_GETALL
		};
		sendMessage(message);

		usleep(1000*1000);
		for (int i = 0; i < featureCount; i++){
			unsigned char message[] = {
						4,
						node,
						gbmMasterID,
						++msgNum[node],
						MSG_FEATURE_GETNEXT
			};
			sendMessage(message);
			usleep(50*1000);
		}
	}
	featureCount = 0;
}

void BiDiBMessageHandler::sendNodeTabMessage() {
	unsigned char message[] = {
				3,
				gbmMasterID,
				++msgNum[gbmMasterID],
				MSG_NODETAB_GETALL
	};
	sendMessage(message);
	usleep(2000*1000);

	for (int i = 0; i < nodeCount; i++){
		unsigned char message[] = {
					3,
					gbmMasterID,
					++msgNum[gbmMasterID],
					MSG_NODETAB_GETNEXT
		};
		sendMessage(message);
		usleep(50*1000);
	}
}

void BiDiBMessageHandler::sendTurnMessage(int turnID, bool direction) {

	int address = turnID*2 + 8 + direction;

	unsigned char onMessage[] = {
				8,
				oneControlID,
				gbmMasterID,
				++msgNum[oneControlID],
				MSG_LC_OUTPUT,
				0x00,
				address,
				1
	};

	unsigned char offMessage[] = {
				8,
				oneControlID,
				gbmMasterID,
				++msgNum[oneControlID],
				MSG_LC_OUTPUT,
				0x00,
				address,
				0
	};

	sendMessage(onMessage);
	usleep(100*1000);
	sendMessage(offMessage);
}

void BiDiBMessageHandler::sendBoostOnMessage() {
	unsigned char message[] = {
						4,
						gbmMasterID,
						++msgNum[gbmMasterID],
						MSG_BOOST_ON,
						0x00
						};
	sendMessage(message);
}

void BiDiBMessageHandler::sendBoostOffMessage() {
	unsigned char message[] = {
						4,
						gbmMasterID,
						++msgNum[gbmMasterID],
						MSG_BOOST_OFF,
						0x00
						};
	sendMessage(message);
}

void BiDiBMessageHandler::sendGetLocsMessage() {
	unsigned char message[] = {
						5,
						gbmMasterID,
						++msgNum[gbmMasterID],
						MSG_BM_ADDR_GET_RANGE,
						0x00,
						31
						};
	sendMessage(message);
}

void BiDiBMessageHandler::sendGetSwitchesMessage() {
	unsigned char message[] = {
						6,
						oneOcID,
						MSG_BM_GET_RANGE,
						++msgNum[oneOcID],
						MSG_BM_ADDR_GET_RANGE,
						0x00,
						24
						};
	sendMessage(message);
}
