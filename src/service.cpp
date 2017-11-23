
#include "service.h"

#include <inttypes.h>
#include "app_error.h"	// APP_ERROR_CHECK
#include <string.h>	// memset

#include "ble_srv_common.h"

// Service has-a
#include "characteristic.h"





namespace {

ServiceData serviceData;

void initServiceData(ServiceDataInit* serviceDataInit) {
	memset(&serviceDataInit, 0, sizeof(serviceDataInit));

	// Here the sec level for the Battery Service can be increased.
	//BLE_GAP_CONN_SEC_MODE_SET_OPEN(&serviceDataInit.battery_level_char_attr_md.cccd_write_perm);
	//BLE_GAP_CONN_SEC_MODE_SET_OPEN(&serviceDataInit.battery_level_char_attr_md.read_perm);
	//BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&serviceDataInit.battery_level_char_attr_md.write_perm);

	//BLE_GAP_CONN_SEC_MODE_SET_OPEN(&serviceDataInit.battery_level_report_read_perm);

	serviceDataInit->evt_handler          = NULL;
	serviceDataInit->support_notification = true;
	//serviceDataInit.p_report_ref         = NULL;
	serviceDataInit->initialValue   = 100;
}


static void onConnect(ServiceData * p_bas, ble_evt_t * p_ble_evt)
{
    p_bas->connectionHandle = p_ble_evt->evt.gap_evt.conn_handle;
}


static void onDisconnect(ServiceData * p_bas, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_bas->connectionHandle = BLE_CONN_HANDLE_INVALID;
}


static void onWrite(ServiceData * p_bas, ble_evt_t * p_ble_evt)
{
    if (p_bas->isNotificationSupported)
    {
        ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

        if (
            (p_evt_write->handle == p_bas->characteristicHandles.cccd_handle)
            &&
            (p_evt_write->len == 2)
           )
        {
            // CCCD written, call application event handler
            if (p_bas->eventHandler != NULL)
            {
            	BleServiceEvent evt;

                if (ble_srv_is_notification_enabled(p_evt_write->data))
                {
                    evt.evt_type = BLE_EVT_NOTIFICATION_ENABLED;
                }
                else
                {
                    evt.evt_type = BLE_EVT_NOTIFICATION_DISABLED;
                }

                // call app's handler
                p_bas->eventHandler(p_bas, &evt);
            }
        }
    }
}


// dispatch on event type
void onBleEvent(ServiceData * p_bas,
		ble_evt_t* p_ble_evt)
{
    if (p_bas == NULL || p_ble_evt == NULL)
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            onConnect(p_bas, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            onDisconnect(p_bas, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            onWrite(p_bas, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


}


// TODO pass in appEventHandler
uint32_t Service::init() {
	uint32_t err_code;
	ble_uuid_t ble_uuid;

	// Local scope, used in this function and then discarded
	ServiceDataInit serviceDataInit;

	initServiceData(&serviceDataInit);

	// Initialize service structure
	// TODO pass in
	serviceData.eventHandler               = nullptr;	//p_bas_init->evt_handler;
	serviceData.connectionHandle           = BLE_CONN_HANDLE_INVALID;
	serviceData.isNotificationSupported    = true;	// p_bas_init->support_notification;
	//serviceData.battery_level_last         = INVALID_BATTERY_LEVEL;

	// TODO ???
	// Add service
	BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_BATTERY_SERVICE);

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &serviceData.serviceHandle);

	if (err_code == NRF_SUCCESS) {
		err_code = Characteristic::add(&serviceData, &serviceDataInit);
		// Service fails when characteristic fails
	}

	return err_code;
}

