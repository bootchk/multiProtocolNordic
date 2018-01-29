
#include "appHandler.h"



#include "provisioner.h"

// use logger from radioSoC library
#include "services/logger.h"
//#include "nrfLog.h"




void AppHandler::onWrite(const ble_gatts_evt_write_t * aWrite) {

	RTTLogger::log("Write characteristic.");

	/*
	 * validate: is my characteristic and len is proper
	 */

	/*
	 * Filter out BT SIG defined (standard) characteristics
	 * and unknowns
	 * Since app is not interested.
	 */
	if (aWrite->uuid.type == 1 or aWrite->uuid.type == 1) {
		RTTLogger::log("Ignoring write to std BT or unknown characteristic");
	}

	/*
	 * If is my characteristic and length is sane.
	 *
	 * My characteristics are type 3,...
	 * Type 2 is UUID of service?
	 */
	else if (aWrite->uuid.type == 3 and aWrite->len ==1 ) {
		RTTLogger::log("Value written to my characteristic");
		RTTLogger::log((uint8_t)aWrite->data[0]);

		RTTLogger::log("Value written to my characteristic");

		// TODO, copy and pass value
		/*
		 * Propagate up to app.  Here app is "provisioner."
		 * Here, Provisioner will shutdown BLE.
		 */
		Provisioner::onProvisioned();
	}
	else {
		/*
		 * Unexpected:
		 * Not a standard or unknown characteristic,
		 * but not one that my app has defined.
		 */
		RTTLogger::log("Write to unrecognized characteristic.");

	}
}


void AppHandler::onConnect() {
	// App doesn't care about connections
	RTTLogger::log("Connected.\n");
}


void AppHandler::onDisconnect(){
	// App doesn't care about disconnections
	RTTLogger::log("Disconnected.\n");
}

// TODO app should care about advertisement timeouts.
// e.g. it indicates there is no client e.g. provisioner existing in RF range.


