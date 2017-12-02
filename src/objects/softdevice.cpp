
#include "softdevice.h"

#include <inttypes.h>
#include "../objects/nrfLog.h"

#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
#include "app_error.h"	// APP_ERROR_CHECK

//#include "ble.h"
//#include "ble_srv_common.h"
//SDK13 #include "softdevice_handler.h"  // in softdevice/common/softdevice_handler // SOFTDEVICE_HANDLER_INIT
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"


#include "service.h"
#include "gap.h"
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


/*
 * Dispatch BLE stack event to all modules with a BLE stack event handler.
 *
 * Registered with and called from the BLE Stack event interrupt handler.
 *
 * Sequentially call each module's event handler.  Many modules are observing same event.
 */
void dispatchBleEvent( ble_evt_t const * bleEvent, void * context)
{
	NRFLog::log("bleEvent\n");

	// !!! original code passes the context, to be cast
	// ?? Here I pass the one serviceData?
	Service::onBleEvent(&serviceData, bleEvent);

    //lkk ble_conn_params_on_ble_evt(bleEvent);

    GAP::onBleEvent(bleEvent, context);
}

#ifdef NOT_USED
void dispatchSysEvent(ble_evt_t * bleEvent) {

}
#endif

}




void Softdevice::enable() {
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
	err_code = nrf_sdh_ble_default_cfg_set(Softdevice::ProtocolTag, &ram_start);
	APP_ERROR_CHECK(err_code);

	// Enable BLE stack.
	err_code = nrf_sdh_ble_enable(&ram_start);
	APP_ERROR_CHECK(err_code);

	// Register a handler for BLE events.
	// bleObserver is just name of some object defined by macro
	// last param is context to be passed to event handler
	//NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
	NRF_SDH_BLE_OBSERVER(bleObserver, APP_BLE_OBSERVER_PRIO, dispatchBleEvent, nullptr);
#endif
}
