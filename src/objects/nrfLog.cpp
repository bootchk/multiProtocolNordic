
#include "nrfLog.h"

#include <inttypes.h>


// Don't know what this is for, give link error if use it
//#define NRF_LOG_MODULE_NAME    APP

// components/libraries/experimental_log
// also includes from components/libraries/atomic
// also includes from external/fprintf
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


void NRFLog::enable() {
	uint32_t           err_code;

	// also define NRF_LOG_ENABLED=1 in sdk_config.h
	err_code = NRF_LOG_INIT(nullptr);	// no timestamp func
	APP_ERROR_CHECK(err_code);

	NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void NRFLog::log(char const * string) {
	// Use this if string is dynamic (on the stack)
	//NRF_LOG_INFO("%s",nrf_log_push(string));
	NRF_LOG_INFO("%s",string);
	NRF_LOG_FLUSH();
}

void NRFLog::flush() {
	NRF_LOG_FLUSH();
}
