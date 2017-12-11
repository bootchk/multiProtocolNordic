
#include "appHandler.h"
#include "nrfLog.h"


#include <string.h>  // memcpy

void AppHandler::onWrite(const ble_gatts_evt_write_t * aWrite) {
	int value;

	// TODO validate

	// is my characteristic

	// len is proper

	/*
	 * Filter out BT SIG defined (standard) characteristics
	 * and unknowns
	 * Since app is not interested.
	 */
	if (aWrite->uuid.type == 1 or aWrite->uuid.type == 1) {
		NRFLog::log("Ignoring write to std BT or unknown characteristic");
		return;
	}

	/*
	 *
	 * If is my characteristic and length is sane.
	 *
	 * My characteristics are type 3,...
	 * Type 2 is UUID of service?
	 */
	if (aWrite->uuid.type == 3 and aWrite->len ==1 ) {
		NRFLog::log("Value written to my characteristic");

		NRFLog::logInt(bleEvent->evt.gatts_evt.params.write.data[0]);
	}
	else {
		/*
		 * Unexpected:
		 * Not a standard or unknown characteristic,
		 * but not one that my app has defined.
		 */
		NRFLog::log("Write to unrecognized characteristic.");
	}

	// TODO, copy the value and do something with it
}


void AppHandler::onConnect() {
	// App doesn't care about connections
}


void AppHandler::onDisconnect(){
	// App doesn't care about disconnections
}

// TODO app should care about advertisement timeouts.
// e.g. it indicates there is no client e.g. provisioner existing in RF range.


