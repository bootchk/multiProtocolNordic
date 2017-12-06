
#include "advertiser.h"

#include "advertisement.h"
#include "nrfLog.h"


#include <inttypes.h>
#include <cassert>

#include "ble_advdata.h"
#include "app_error.h"	// APP_ERROR_CHECK

namespace {
static ble_gap_adv_params_t advertisingParams;	// Created by init(), used by start()
}

#define APP_BLE_CONN_CFG_TAG                1      /**< A tag for a Bluetooth stack configuration. */

#define APP_ADV_INTERVAL                 40        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       180       /**< The advertising timeout in units of seconds. */


namespace {

/*
 * This is the only "mode" of advertising.
 * If using the Nordic advertising module, it will sequence a set of modes e.g. fast, slow, etc.
 */
void initParams() {
	memset(&advertisingParams, 0, sizeof(advertisingParams));

	advertisingParams.type        = BLE_GAP_ADV_TYPE_ADV_IND;
	advertisingParams.p_peer_addr = NULL; // Undirected advertisement.
	advertisingParams.fp          = BLE_GAP_ADV_FP_ANY;
	advertisingParams.interval    = APP_ADV_INTERVAL;
	advertisingParams.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
}

}

void Advertiser::init() {
	initParams();
}


/*
 * Note no access to Advertisement, already set in the Softdevice.
 */
void Advertiser::startAdvertising() {
	uint32_t err_code;

	assert(Advertisement::isInit());

	NRFLog::log("started advertising\n");

	err_code = sd_ble_gap_adv_start(&advertisingParams, APP_BLE_CONN_CFG_TAG);
	APP_ERROR_CHECK(err_code);
}

void Advertiser::stopAdvertising() {
	NRFLog::log("not implemented stop advertising\n");
	uint32_t err_code;
	err_code = sd_ble_gap_adv_stop();
	APP_ERROR_CHECK(err_code);
}

