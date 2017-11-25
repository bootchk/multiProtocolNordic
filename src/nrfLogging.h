#include <inttypes.h>

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"


void enableNrfLogging() {
	uint32_t           err_code;

	// If not done, crashes?
	// also define NRF_LOG_ENABLED=1 in sdk_config.h
	err_code = NRF_LOG_INIT(nullptr);	// no timestamp func
	APP_ERROR_CHECK(err_code);
}
