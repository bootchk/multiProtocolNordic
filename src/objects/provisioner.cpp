


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
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
	isProvisioningFlag = false;

	assert(! Provisioner::isProvisioning());

	assert(!Softdevice::isEnabled());
	// Ensure not affect LF clock
	assert(AppTimer::isClockRunning());
}


static void provisionElapsedTimerHandler(void* context) {
	assert( Provisioner::isProvisioning() );
	NRFLog::log("provisioning session timeout");

	// Time elapsed without any client provisioning us

	shutdown();

	// Ensure not provisioning at time of callback
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

	/*
	 * We did not timeout, cancel timer.
	 */
	AppTimer::stop(provisionElapsedTimerID);

	shutdown();

	// Ensure not provisioning at time of callback
	assert(! Provisioner::isProvisioning());
	succeedCallback();
	// assert hw resources not used by SD, can be used by app
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

	// provisioning sessions are one at a time
	assert(!isProvisioning());

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

	// Start timeout on provisioning service
	AppTimer::start(provisionElapsedTimerID, 1000);	// 200);	// 0.2 seconds

	isProvisioningFlag = true;
}


void Provisioner::sleep() {
	// Not legal to sleep via Provisioner when not provisioning
	assert(isProvisioning());

	ret_code_t err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}
