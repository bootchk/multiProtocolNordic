#include "protocolStack.h"

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"



void ProtocolStack::startup() {
	Softdevice::enable();

	BLEProtocol::start();

	BLEProtocol::startAdvertising();
}


void ProtocolStack::shutdown() {
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
}

bool ProtocolStack::isEnabled() {
	return Softdevice::isEnabled();
}
