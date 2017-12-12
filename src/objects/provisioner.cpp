


#include "provisioner.h"

// Implementation

// Implementation Nordic SDK
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "app_timer.h"

#include <cassert>

// Implementation facade classes
// SD supports many protocols
#include "softdevice.h"
// One of protocols is BLE
#include "bleProtocol.h"

#include "nrfLog.h"
#include "appTimer.h"

/*
#include "service.h"
#include "adModule.h"
#include "uuid.h"
#include "gap.h"
#include "gatt.h"
#include "connection.h"
*/


namespace {

bool isProvisioningFlag = false;

ProvisioningCallback succeedCallback = nullptr;
ProvisioningCallback failCallback = nullptr;

APP_TIMER_DEF(provisionElapsedTimerID);


static void shutdown() {
	// shutdown protocol and SD
	BLEProtocol::stop();
	Softdevice::disable();
}


static void provisionElapsedTimerHandler(void* context) {
	assert( Provisioner::isProvisioning() );

	// Time elapsed without any client provisioning us

	shutdown();
	assert(! Provisioner::isProvisioning());
	failCallback();
	// assert oneshot timer not enabled
}

}



void Provisioner::onProvisioned() {
	assert(isProvisioning());

	/*
	 * Semantics are one-shot:
	 * Any provisioning ends session
	 */
	// TODO cancel timer

	shutdown();
	succeedCallback();
}



void Provisioner::enable(ProvisioningCallback aSucceedCallback, ProvisioningCallback aFailCallback) {
	succeedCallback = aSucceedCallback;
	failCallback = aFailCallback;

	AppTimer::createOneShot(&provisionElapsedTimerID, provisionElapsedTimerHandler);

	assert(! Provisioner::isProvisioning());	// enabled but not started
}


bool Provisioner::isProvisioning() {
	return isProvisioningFlag;
}


void Provisioner::start() {
	// assert enabled
	assert(succeedCallback != nullptr);

	// SD
	Softdevice::enable();

	// TODO bleProtocol
	BLEProtocol::start();

	/*
	These modules:

	GAP::initParams();
	Gatt::init();
	Uuid::init();
	AdModule::init();
	// Creating service also creates its characteristics
	Service::init();
	Connection::initParams();
	AdModule::startAdvertising(false);
	*/
	BLEProtocol::startAdvertising();

	// Create timeout on provisioning service
	AppTimer::start(provisionElapsedTimerID, 200);

	isProvisioningFlag = true;
}


void Provisioner::sleep() {
	// Not legal to sleep via Provisioner when not provisioning
	assert(isProvisioning());

	ret_code_t err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}
