
#include "sleeper.h"

// Implementation: NRF_SDK
#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
#include "app_error.h"

// Implementation:
#include "timerAdaptor.h"
#include "softdevice.h"
#include "provisioner.h"
#include "nrfLog.h"

#include <cassert>


void Sleeper::sleepInSD() {
	uint32_t err_code;

	// Sleep using SoftDevice API when SD active.
	err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}

void Sleeper::sleepInSDUntilTimeout(uint32_t timeout) {

	// Start timeout on provisioning service
	TimerAdaptor::start(timeout, Provisioner::provisionElapsedTimerHandler);

	//AppTimer::start(provisionElapsedTimerID, 1000);	// 200);	// 0.2 seconds

	// Not meaningful to call when SD already disabled
	assert(Softdevice::isEnabled());

	/*
	 * This design depends on all relevant events disabling SD.
	 */
	while (Softdevice::isEnabled()) {
		sleepInSD();

		NRFLog::log("Wake from sleepInSD");
		/*
		 * Woken by event:
		 * 1) that SD handled
		 * 2) timeout on expiration timer
		 * 3) other events (long clock overflow or other timers)
		 *
		 * 1) client provisioned us.
		 * SD already handled it in interrupt context
		 * and called AppHandler::onWrite
		 * which called Provisioner::onProvisioned which shutdown SD
		 *
		 * 2) timeout
		 * ISR of timer callback handler which disabled SD
		 *
		 * 3) other events which woke cpu
		 * Sleep again.
		 *
		 * !!! This depends on NRF_SDH_DISPATCH_MODEL_INTERRUPT
		 * i.e. SDH callbacks have already occurred in ISR's before
		 * execution resumes here.
		 */
	}

}

