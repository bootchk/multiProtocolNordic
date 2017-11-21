

#include <inttypes.h>

// in libraries/util
// #include "nordic_common.h"

// nrf.h depends on
// toolchain/cmsis/include
// toolchain
#include "nrf.h"	// in /drivers

// NRF_CLOCK_LFCLKSRC
//#include "nrf_clock.h"	// in /drivers_nrf/hal
#include "pca10040.h"   // in boards

// Softdevice (closed library) headers
#include "nrf_sdm.h"	// in /softdevice/s132/headers

#include "bleProtocol.h"


void BLEProtocol::start() {

	uint32_t           err_code;
	nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

	SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

	// Enable BLE stack
	ble_enable_params_t ble_enable_params;
	err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
			PERIPHERAL_LINK_COUNT,
			&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	err_code = softdevice_enable(&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
	APP_ERROR_CHECK(err_code);
	/*
	gap_params_init();
	advertising_init();
	services_init();
	//sensor_simulator_init();
	conn_params_init();
	//application_timers_start();
	sec_params_init();
	advertising_start();
	*/
}
