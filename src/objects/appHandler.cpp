
#include "appHandler.h"



#include <string.h>  // memcpy

void AppHandler::onWrite(const ble_gatts_evt_write_t * aWrite) {
	int value;

	// TODO validate

	// is my characteristic

	// len is proper

	// copy unaligned data into aligned variable
	memcpy(&value, aWrite->data, sizeof(int));

	//NRFLog::logInt(value);
	// alternatively
	//NRFLog::logInt(bleEvent->evt.gatts_evt.params.write.data[0]);

	//NRFLog::logInt(bleEvent->evt.gatts_evt.params.write.len);

	// uuid field is of type "const ble_uuid_t&"
	//NRFLog::logInt(bleEvent->evt.gatts_evt.params.write.uuid.type); // ->type);
}


void AppHandler::onConnect() {

}


void AppHandler::onDisconnect(){

}
