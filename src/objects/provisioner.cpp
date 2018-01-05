
#include "provisioner.h"
#include <cassert>

// Implementation facade classes
#include <softdeviceSleeper.h>
#include "protocolStack.h"
#include "nrfLog.h"
#include "timerAdaptor.h"



namespace {

bool isProvisioningFlag = false;

ProvisioningCallback succeedCallback = nullptr;
ProvisioningCallback failCallback = nullptr;

}	// namespace





void Provisioner::init(ProvisioningCallback aSucceedCallback, ProvisioningCallback aFailCallback) {
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

	ProtocolStack::shutdown();

	isProvisioningFlag = false;

	assert(! Provisioner::isProvisioning());

	assert(!ProtocolStack::isEnabled());

	// Ensure not affect LF clock
	// assert(AppTimer::isClockRunning());
}




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




/*
 * onProvisioned() and onTimerElapsed() are two reasons to end a session
 * Should be similar implementations
 */

/*
 * Called from SD, from a handler.
 * You can't shutdown SD at such a time?
 * Because it returns to the SD's chain of handlers.
 */
void Provisioner::onProvisioned() {
	assert(isProvisioning());

	// We did not timeout, cancel timer.
	TimerAdaptor::stop();

	/*
	 * Semantics are one-shot: any provisioning ends sleep and session.
	 */
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Canceled);

	// Tell app
	succeedCallback();
}

void Provisioner::onTimerElapsed() {
	// Time elapsed without any client provisioning us
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::TimedOut);

	failCallback();
	// assert oneshot timer not enabled
}






void Provisioner::startClocks(){
	// delegate
	TimerAdaptor::startClocks();
}


bool Provisioner::isProvisioning() {
	return isProvisioningFlag;
}





void Provisioner::provisionWithSleep() {
	// Clear flag before starting session, it may succeed before we get to sleep
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Cleared);

	start();
	NRFLog::log("Provisioner sleeps");
	SoftdeviceSleeper::sleepInSDUntilTimeoutOrCanceled(Provisioner::ProvisioningSessionDuration);

	assert(SoftdeviceSleeper::getReasonForSDWake() != ReasonForSDWake::Cleared);

	shutdown();
	// assert hw resources not used by SD, can be used by app
	assert(! Provisioner::isProvisioning());
}
