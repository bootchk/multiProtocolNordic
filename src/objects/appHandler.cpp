
#include "appHandler.h"



#include "provisioner.h"
#include "characteristic.h"

// use logger from radioSoC library
#include "services/logger.h"
//#include "nrfLog.h"




void AppHandler::onWriteCharacteristic(const ble_gatts_evt_write_t * aWrite) {

	RTTLogger::log("Write characteristic.");

	/*
	 * The write is "wild": OTA and might be flawed
	 * or not one that app recognizes.
	 */
	if ( Characteristic::isValidWrite(aWrite)) {
		uint8_t value = aWrite->data[0];

		RTTLogger::log("Value written to my characteristic");
		RTTLogger::log(value);

		RTTLogger::log("Value written to my characteristic");

		/*
		 * Propagate up to app.  Here app is "provisioner."
		 * Here, Provisioner will shutdown BLE.
		 */
		Provisioner::onProvisioned(value);
	}
	else {
		/*
		 * Unexpected:
		 * Not a standard or unknown characteristic,
		 * and not one that my app has defined.
		 */
		RTTLogger::log("Write to unexpected characteristic.");

	}
}


void AppHandler::onConnect() {
	// App doesn't care about connections
	RTTLogger::log("Connected.\n");
}

/*
 * This doesn' seem to come, when we shutdown without calling disconnect()
 */
void AppHandler::onDisconnect(){
	// App doesn't care about disconnections
	RTTLogger::log("Disconnected.\n");
}

// TODO app should care about advertisement timeouts.
// e.g. it indicates there is no client e.g. provisioner existing in RF range.


