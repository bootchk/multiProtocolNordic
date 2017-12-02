
#include "sleeper.h"

#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
#include "app_error.h"


void Sleeper::sleepInBt() {
	uint32_t err_code;

	// Use SoftDevice API for power_management when in Bluetooth Mode.
	err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}
