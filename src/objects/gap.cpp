
#include "gap.h"

#include "nrfLog.h"


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






void GAP::onBleEvent(const ble_evt_t * bleEvent, void* foo) {

	uint32_t                    err_code;
	static uint16_t             s_conn_handle = BLE_CONN_HANDLE_INVALID;
	//static ble_gap_sec_keyset_t s_sec_keyset;
	//ble_gap_enc_info_t        * p_enc_info;

	NRFLog::log("GAP event");
	switch (bleEvent->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		// Remember handle to connection, needed later
		s_conn_handle = bleEvent->evt.gap_evt.conn_handle;
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		// TODO, if connection successfully set characteristic,  shutdown
		// else if not timed out, advertise again.
		//advertising_start();
		break;

	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
		/*
	            No security.
	            s_sec_keyset.keys_peer.p_enc_key  = NULL;
	            s_sec_keyset.keys_peer.p_id_key   = NULL;
	            s_sec_keyset.keys_peer.p_sign_key = NULL;
	            err_code                          = sd_ble_gap_sec_params_reply(s_conn_handle,
	                                                                            BLE_GAP_SEC_STATUS_SUCCESS,
	                                                                            &m_sec_params,
	                                                                            &s_sec_keyset);
	            APP_ERROR_CHECK(err_code);
		 */
		NRFLog::log("security params request");
		break;

	case BLE_GAP_EVT_AUTH_STATUS:
		break;

	case BLE_GATTS_EVT_SYS_ATTR_MISSING:
		err_code = sd_ble_gatts_sys_attr_set(s_conn_handle, NULL, 0, 0);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GAP_EVT_SEC_INFO_REQUEST:
		/*
	            No security.
	            if (s_sec_keyset.keys_own.p_enc_key != NULL)
	            {
	                p_enc_info = &s_sec_keyset.keys_own.p_enc_key->enc_info;

	                err_code = sd_ble_gap_sec_info_reply(s_conn_handle, p_enc_info, NULL, NULL);
	                APP_ERROR_CHECK(err_code);
	            }
	            else
	            {
	                // No keys found for this device.
	                err_code = sd_ble_gap_sec_info_reply(s_conn_handle, NULL, NULL, NULL);
	                APP_ERROR_CHECK(err_code);
	            }
		 */
		break;

	case BLE_GAP_EVT_TIMEOUT:
		if (bleEvent->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
		{
			/*
	                err_code = bsp_indication_set(BSP_INDICATE_IDLE);
	                APP_ERROR_CHECK(err_code);

	                err_code = app_button_disable();
	                APP_ERROR_CHECK(err_code);

	                if (err_code == NRF_SUCCESS)
	                {
	                    // Go to system-off mode.
	                    // (this function will not return; wakeup will cause a reset)
	                    err_code = sd_power_system_off();
	                    APP_ERROR_CHECK(err_code);
	                }
			 */
		}
		break;

	default:
		// No implementation needed.
		break;
	}
}



void GAP::initParams() {

	uint32_t                err_code;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	// security mode: no protection, open link
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_device_name_set(&sec_mode,
			(const uint8_t *)DEVICE_NAME,
			DEVICE_NAME_LEN);
	//NRFLog::flush();
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
