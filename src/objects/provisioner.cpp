
#include "provisioner.h"
#include <cassert>

// Implementation facade classes
#include <softdeviceSleeper.h>
#include "protocolStack.h"
#include "timerAdaptor.h"

#include "rssi.h"

#include "nrfLog.h"



namespace {

bool isProvisioningFlag = false;
bool provisioningSessionResult = false;
uint8_t provisionedValue = 0;

ProvisioningSucceedCallback succeedCallback = nullptr;
ProvisioningFailCallback failCallback = nullptr;


// assert not in interrupt context, app is not realtime constrained.
void callbackAppWithProvisioningResult() {
	if (provisioningSessionResult ) {
		 // assert SoftdeviceSleeper::getReasonForSDWake() == ReasonForSDWake::Canceled

		succeedCallback(provisionedValue, RSSI::getConnectionRSSI() );
	}
	else {
		failCallback();
	}
}


}	// namespace


uint8_t Provisioner::getProvisionedValue(){ return provisionedValue; }



void Provisioner::init(ProvisioningSucceedCallback aSucceedCallback, ProvisioningFailCallback aFailCallback) {
	succeedCallback = aSucceedCallback;
	failCallback = aFailCallback;

	TimerAdaptor::create(provisionElapsedTimerHandler);

	assert(! Provisioner::isProvisioning());	// enabled but not started
}



/*
 * start() and shutdown() should be mirror images
 */

void Provisioner::start() {
	NRFLog::log("Provisioner start");

	// provisioning sessions are one at a time
	assert(!isProvisioning());

	// assert self initialized
	assert(succeedCallback != nullptr);

	ProtocolStack::startup();

	isProvisioningFlag = true;
}

void Provisioner::shutdown() {
	NRFLog::log("Provisioner shutdown");

	// Might be in a connection.
	// Assume SD handles that case.

	ProtocolStack::shutdown();

	isProvisioningFlag = false;

	assert(! Provisioner::isProvisioning());

	assert(!ProtocolStack::isEnabled());

	// Ensure not affect LF clock
	// assert(AppTimer::isClockRunning());
}



/*
 * ISR for RTCx.
 *
 * Interrupt does not necessarily mean that our timer (compare register) elapsed:
 * overflow and other compare registers (although there aren't any in use)
 * will also interrupt.
 */
void Provisioner::provisionElapsedTimerHandler(TimerInterruptReason reason) {
	assert( Provisioner::isProvisioning() );
	NRFLog::log("provisioner RTC interrupt");

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




/*
 * onProvisioned() and onTimerElapsed() are two reasons to end a session
 * Should be similar implementations
 */


/*
 * Called from SD, from a handler.
 * In interrupt context.
 * You can't shutdown SD at such a time?
 * Because it returns to the SD's chain of handlers.
 */
void Provisioner::onProvisioned(uint8_t aProvisionedValue) {
	assert(isProvisioning());

	// We did not timeout, cancel timer.
	TimerAdaptor::stop();

	RSSI::captureLastRSSI();

	/*
	 * Tell SoftdeviceSleeper to quit its sleeping loop.
	 * Semantics are one-shot: any one provisioning ends sleep and session.
	 */
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Canceled);

	provisionedValue = aProvisionedValue;
	provisioningSessionResult = true;

	/*
	 * Continuation is return to Softdevice handler,
	 * then eventually to after call to SoftdeviceSleeper in provisionWithSleep().
	 * There we will callback app (when not in interrupt context)
	 */
}

void Provisioner::onTimerElapsed() {
	// Time elapsed without any client provisioning us
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::TimedOut);

	provisioningSessionResult = false;

	// assert time is oneshot and thus no longer enabled
}






void Provisioner::startClocks(){
	// delegate
	TimerAdaptor::startClocks();
}


bool Provisioner::isProvisioning() {
	return isProvisioningFlag;
}





bool Provisioner::provisionWithSleep() {
	// Clear flag before starting session, it may succeed before we get to sleep
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Cleared);

	start();
	NRFLog::log("Provisioner sleeps");

	// Blocks in low-power until timer expires or client provisions us via Softdevice
	SoftdeviceSleeper::sleepInSDUntilTimeoutOrCanceled(Provisioner::ProvisioningSessionDuration);

	// Must be a reason we are not sleeping anymore
	assert(SoftdeviceSleeper::getReasonForSDWake() != ReasonForSDWake::Cleared);

	shutdown();
	// assert hw resources not used by SD, can be used by app
	assert(! Provisioner::isProvisioning());

	callbackAppWithProvisioningResult();

	return provisioningSessionResult;
}



