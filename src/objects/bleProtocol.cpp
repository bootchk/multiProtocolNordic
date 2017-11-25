

#include <inttypes.h>


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
#include "nrf.h"	// in /drivers



// Softdevice (closed library) headers
// s110 is BLE peripheral only but deprecated?

// s132 is for BLE central/peripheral without ANT
#include "nrf_sdm.h"	// in /softdevice/s132/headers

#include "bleProtocol.h"

#include "ble.h"
#include "ble_srv_common.h"	// in ble/common
//#include "ble_advdata.h"

// ble profiles ?
//#include "ble_bas.h"	// in ble/ble_services/ble_bas
//#include "ble_dis.h"




#include "softdevice_handler.h"  // in softdevice/common/softdevice_handler




// classes interface to softdevice
#include "softdevice.h"
#include "gap.h"
#include "advertisement.h"
#include "advertiser.h"
#include "service.h"
#include "connection.h"

#include "nrfLogging.h"
















void BLEProtocol::start() {

	Softdevice::enable();

	GAP::initParams();

	// Prepare for advertising
	Advertisement::init();
	Advertiser::init();

	Service::init();

	/*
	 No connection negotiation.
	 No security.

	Connection::initParams();
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

