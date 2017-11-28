
#include "advertiser.h"



#include <inttypes.h>

#include "ble_advdata.h"
#include "app_error.h"	// APP_ERROR_CHECK

namespace {
static ble_gap_adv_params_t m_adv_params;	// Created by init(), used by start()
}

#define APP_BLE_CONN_CFG_TAG                1      /**< A tag for a Bluetooth stack configuration. */

#define APP_ADV_INTERVAL                 40        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       180       /**< The advertising timeout in units of seconds. */



void Advertiser::init() {

	    memset(&m_adv_params, 0, sizeof(m_adv_params));

	    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
	    m_adv_params.p_peer_addr = NULL; // Undirected advertisement.
	    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
	    m_adv_params.interval    = APP_ADV_INTERVAL;
	    m_adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;

}

void Advertiser::startAdvertising() {
	uint32_t err_code;

	err_code = sd_ble_gap_adv_start(&m_adv_params, APP_BLE_CONN_CFG_TAG);
	APP_ERROR_CHECK(err_code);
}
