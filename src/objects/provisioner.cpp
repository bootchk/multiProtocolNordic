


#include <softdeviceSleeper.h>
#include "provisioner.h"

// Implementation

// Implementation Nordic SDK
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
//#include "app_timer.h"

#include <cassert>

// Implementation facade classes

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"

#include "nrfLog.h"
//#include "appTimer.h"
#include "timerAdaptor.h"



namespace {

bool isProvisioningFlag = false;

ProvisioningCallback succeedCallback = nullptr;
ProvisioningCallback failCallback = nullptr;


static void shutdown() {
	// shutdown protocol and SD
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
	isProvisioningFlag = false;

	assert(! Provisioner::isProvisioning());

	assert(!Softdevice::isEnabled());

	// Ensure not affect LF clock
	// assert(AppTimer::isClockRunning());
}

static void onTimerElapsed() {
	// Time elapsed without any client provisioning us

	shutdown();

	// Ensure not provisioning at time of callback
	assert(! Provisioner::isProvisioning());
	failCallback();
	// assert oneshot timer not enabled
}

}	// namespace




void Provisioner::provisionElapsedTimerHandler(TimerInterruptReason reason) {
	assert( Provisioner::isProvisioning() );
	NRFLog::log("provisioning session timeout");

	switch(reason) {
	case OverflowOrOtherTimerCompare:
		/*
		 * Waked but Timer not expired.
		 * Sleep again
		 */
		break;
	case SleepTimerCompare:
		onTimerElapsed();
		break;
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
	TimerAdaptor::stop();

	shutdown();

	// Ensure not provisioning at time of callback
	assert(! Provisioner::isProvisioning());
	succeedCallback();
	// assert hw resources not used by SD, can be used by app
}



void Provisioner::init(ProvisioningCallback aSucceedCallback, ProvisioningCallback aFailCallback) {
	succeedCallback = aSucceedCallback;
	failCallback = aFailCallback;

	TimerAdaptor::create(provisionElapsedTimerHandler);

	assert(! Provisioner::isProvisioning());	// enabled but not started
}


void Provisioner::startClocks(){
	// delegate
	TimerAdaptor::startClocks();
}


bool Provisioner::isProvisioning() {
	return isProvisioningFlag;
}


void Provisioner::start() {
	NRFLog::log("Provisioner start");

	// provisioning sessions are one at a time
	assert(!isProvisioning());

	// assert enabled
	assert(succeedCallback != nullptr);

	// SD
	Softdevice::enable();

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	isProvisioningFlag = true;
}



void Provisioner::provisionWithSleep() {

	start();

	NRFLog::log("Provisioner sleep using Sleeper");
	SoftdeviceSleeper::sleepInSDUntilTimeout(500);
}
