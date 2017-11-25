
#include "gap.h"

#include <inttypes.h>
#include "ble_conn_params.h"
#include "app_error.h"	// APP_ERROR_CHECK
#include <string.h>	// memset




#define DEVICE_NAME  "Firefly"
#define DEVICE_NAME_LEN  7


// Desired by peripheral.  Negotiated and set by central, so not very important.
#define MIN_CONN_INTERVAL   MSEC_TO_UNITS(500, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL   MSEC_TO_UNITS(1000, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY       0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT    MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */


void GAP::initParams() {

	uint32_t                err_code;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	// security mode: no protection, open link
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_device_name_set(&sec_mode,
			(const uint8_t *)DEVICE_NAME,
			DEVICE_NAME_LEN);
	APP_ERROR_CHECK(err_code);

	err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT);
	APP_ERROR_CHECK(err_code);

	memset(&gap_conn_params, 0, sizeof(gap_conn_params));

	gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
	gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
	gap_conn_params.slave_latency     = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

	err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
	APP_ERROR_CHECK(err_code);

}
