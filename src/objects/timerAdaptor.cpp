
#include "timerAdaptor.h"

#include <cassert>

#include "nrfLog.h"

// lib radioSoC
#include "timer.h"
#include "clockFacilitator.h"



/*
 * When Using AppTimer implementation
 */
//APP_TIMER_DEF(provisionElapsedTimerID);
// AppTimer::stop(provisionElapsedTimerID);
//AppTimer::createOneShot(&provisionElapsedTimerID, provisionElapsedTimerHandler);

void TimerAdaptor::startClocks() {

#ifdef OLD
	this interferes with SD
	ClockFacilitator::startLongClockWithSleepUntilRunning();
#else
	ClockFacilitator::startLongClockNoWaitUntilRunning();
#endif

	// No need to create timers, static defined
}


#ifdef NOT_USED
void TimerAdaptor::create() {
}
#endif

void TimerAdaptor::create(AlarmHandler onTimeoutCallback) {
	// Using radioSoC, no need to create
}

void TimerAdaptor::start(uint32_t timeout, AlarmHandler onTimeoutCallback) {
	assert(ClockFacilitator::isLongClockRunning());

	NRFLog::log("start timer");
	Timer::start(
			TimerIndex::First,	// First timer not in use by SleepSync
			timeout,
			onTimeoutCallback);
}


void TimerAdaptor::stop() {
	Timer::cancel(TimerIndex::First);
}
