
#include "softdevice.h"

#include <inttypes.h>
#include <objects/nrfLog.h>
#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
//#include "bleProtocol.h"
#include "app_error.h"	// APP_ERROR_CHECK

//#include "ble.h"
//#include "ble_srv_common.h"
//SDK13 #include "softdevice_handler.h"  // in softdevice/common/softdevice_handler // SOFTDEVICE_HANDLER_INIT
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"


#include "service.h"
#include "nrfLog.h"


// When changing this number remember to adjust the RAM settings
#define CENTRAL_LINK_COUNT      0      // < Number of central links used by the application.
#define PERIPHERAL_LINK_COUNT   1      // < Number of peripheral links used by the application.

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0
/*
 * Whether or not to include the service_changed characteristic.
 * If not enabled, the server's database cannot be changed for the lifetime of the device
 */

#define APP_BLE_OBSERVER_PRIO   3  /**< Application's BLE observer priority. You shouldn't need to modify this value. */



namespace {

ServiceData serviceData;

/* Dispatch/handle App's BLE events.
 *
 * Specific to the app.
 */
void on_ble_evt(const ble_evt_t * p_ble_evt, void* foo)
{
    uint32_t                    err_code;
    static uint16_t             s_conn_handle = BLE_CONN_HANDLE_INVALID;
    //static ble_gap_sec_keyset_t s_sec_keyset;
    //ble_gap_enc_info_t        * p_enc_info;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
        	// Remember handle to connection, needed later
            s_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
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
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
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

/*
 * Dispatch BLE stack event to all modules with a BLE stack event handler.
 *
 * Registered with and called from the BLE Stack event interrupt handler.
 */
void dispatchBleEvent(ble_evt_t * bleEvent)
{
	// Service events
	Service::onBleEvent(&serviceData, bleEvent);

    //lkk ble_conn_params_on_ble_evt(bleEvent);

	// GAP events
    on_ble_evt(bleEvent, nullptr);		// TODO what should second argument be
}

void dispatchSysEvent(ble_evt_t * bleEvent) {

}

}




void Softdevice::enable(int tag) {
	uint32_t           err_code;

	// NRFLog::log("Enable SD\n");

	#ifdef SDK13
	// Struct instance that describes lf clock, copied from board/pca10040.h
	nrf_clock_lf_cfg_t clock_lf_cfg =  {
			.source        = NRF_CLOCK_LF_SRC_XTAL,            \
			.rc_ctiv       = 0,                                \
			.rc_temp_ctiv  = 0,                                \
			.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM};


	SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, nullptr);

	// Enable BLE stack
	ble_enable_params_t ble_enable_params;
	err_code = softdevice_enable_get_default_config(
			CENTRAL_LINK_COUNT,
			PERIPHERAL_LINK_COUNT,
			&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	// ??? Might be necessary for later SD versions?
	ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

	// Nordic deprecated to check the ram settings against the used number of links

	err_code = softdevice_enable(&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	uint8_t sd_is_enabled = 0;
	sd_softdevice_is_enabled(&sd_is_enabled);
	//sd_softdevice_is_ble_enabled(&sd_is_enabled);
	if ( !sd_is_enabled ) {
		NRFLog::log("SOFT DEVICE IS NOT ENABLED\r\n");
	}

	err_code = softdevice_ble_evt_handler_set(dispatchBleEvent);
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for SYS events.
	// See NRF_SOC_EVTS enum.  Events like hfclkstarted and radio events
	err_code = softdevice_sys_evt_handler_set(dispatchSysEvent);
	APP_ERROR_CHECK(err_code);

	NRFLog::flush();
#else
	err_code = nrf_sdh_enable_request();
	APP_ERROR_CHECK(err_code);

	ASSERT(nrf_sdh_is_enabled());

	// Configure the BLE stack using the default settings.
	// Fetch the start address of the application RAM.
	uint32_t ram_start = 0;
	//
	err_code = nrf_sdh_ble_default_cfg_set(tag, &ram_start);
	APP_ERROR_CHECK(err_code);

	// Enable BLE stack.
	err_code = nrf_sdh_ble_enable(&ram_start);
	APP_ERROR_CHECK(err_code);

	// Register a handler for BLE events.
	//NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
	NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, dispatchBleEvent, nullptr);
#endif
}
