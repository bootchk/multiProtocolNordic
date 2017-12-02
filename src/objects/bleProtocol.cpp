

#include <inttypes.h>
#include "../objects/nrfLog.h"


/*
 * External hacks
 */
// define BLE_STACK_SUPPORT_REQD in project properties>C++Compiler/Settings/Preprocessor

// hack softdevice/s132/headers/nrf_svc.h to "bx r14" : : "I" ((uint16_t)number) : "r0"

// paths to include files in project properties>C++Compiler/Settings/Includes

// Add linker file to Project Properties>C++ Build>Settings>Tool Settings>Cross ARM C Linker>General>Script Files>Add...
// Add path to included linker file
// Project Properties>C++ Build>Settings>Tool Settings>Cross ARM C Linker>Libraries-> Library search path>Add...





// in libraries/util
// #include "nordic_common.h"

// nrf.h depends on
// toolchain/cmsis/include
// toolchain
//#include "nrf.h"	// in /drivers



// Softdevice (closed library) headers
// s110 is BLE peripheral only but deprecated?

// s132 is for BLE central/peripheral without ANT
//#include "nrf_sdm.h"	// in /softdevice/s132/headers

#include "bleProtocol.h"

//#include "ble.h"
//#include "ble_srv_common.h"	// in ble/common
//#include "ble_advdata.h"







// classes interface to softdevice
#include "softdevice.h"
#include "gap.h"
#include "advertisement.h"
#include "advertiser.h"
#include "service.h"
#include "connection.h"
#include "appTimer.h"




void BLEProtocol::start() {

	// OLD now done by caller
	// Softdevice::enable(BLEProtocol::ProtocolTag);

	// Softdevice requires??
	// AppTimer::init();

	GAP::initParams();

	// Prepare for advertising
	Advertisement::init();
	Advertiser::init();
	// This will advance testing each function
	return;

	Service::init();

	//Connection::initParams(Service::data());
	/*
	 No connection negotiation.
	 No security.


	sec_params_init();
	*/


}

void BLEProtocol::stop() {

}


void BLEProtocol::startAdvertising() {
	Advertiser::startAdvertising();
}
void BLEProtocol::stopAdvertising() {
	Advertiser::stopAdvertising();
}

