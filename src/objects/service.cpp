
#include "service.h"

#include <inttypes.h>
#include "app_error.h"	// APP_ERROR_CHECK
#include <string.h>	// memset

#include "ble_srv_common.h"

// Service has-a
#include "characteristic.h"
// Service uses
#include "uuid.h"


/*
 * Taken from SDK ble_hrs.c
 * This is the server side (see ble_hrs_c.c for client side, i.e. a Central.
 */


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
	// The central does not get notified when characteristic changes
	serviceDataInit->support_notification = false;
	//serviceDataInit.p_report_ref         = NULL;
	serviceDataInit->initialValue   = 100;
}


static void onConnect(ServiceData * serviceData, ble_evt_t const * bleEvent)
{
    serviceData->connectionHandle = bleEvent->evt.gap_evt.conn_handle;
}


static void onDisconnect(ServiceData * serviceData, ble_evt_t const * bleEvent)
{
    UNUSED_PARAMETER(bleEvent);
    serviceData->connectionHandle = BLE_CONN_HANDLE_INVALID;
}

static void on_hrm_cccd_write(ServiceData * serviceData, ble_gatts_evt_write_t const * p_evt_write)
{
    if (p_evt_write->len == 2)
    {
#ifdef TODO
        // CCCD written, update notification state
        if (serviceData->appEventHandler != nullptr)
        {
            ble_hrs_evt_t evt;

            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_HRS_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_HRS_EVT_NOTIFICATION_DISABLED;
            }

            serviceData->evt_handler(serviceData, &evt);
        }
#endif
    }
}

// TODO I don't understand the semantics:
// why does a write send enabled event to app?
// TODO read tutorial
static void onWrite(ServiceData * serviceData, ble_evt_t const * bleEvent)
{

	ble_gatts_evt_write_t const * p_evt_write = &bleEvent->evt.gatts_evt.params.write;

	if (p_evt_write->handle == serviceData->characteristicHandles.cccd_handle)
	{
		on_hrm_cccd_write(serviceData, p_evt_write);
	}

#ifdef OLD
    if (serviceData->isNotificationSupported)
    {
        ble_gatts_evt_write_t * p_evt_write = &bleEvent->evt.gatts_evt.params.write;

        if (
            (p_evt_write->handle == serviceData->characteristicHandles.cccd_handle)
            &&
            (p_evt_write->len == 2)
           )
        {
            // CCCD written, call application event handler
            if (serviceData->eventHandler != NULL)
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
                serviceData->eventHandler(serviceData, &evt);
            }
        }
    }
#endif
}


}	//namespace



// dispatch on event type
void Service::onBleEvent(
		ServiceData * serviceData,
		ble_evt_t const* bleEvent)
{
    if (serviceData == NULL || bleEvent == NULL)
    {
        return;
    }

    switch (bleEvent->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            onConnect(serviceData, bleEvent);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            onDisconnect(serviceData, bleEvent);
            break;

        case BLE_GATTS_EVT_WRITE:
            onWrite(serviceData, bleEvent);
            break;

        default:
            // No implementation needed.
            break;
    }
}



// TODO pass in appEventHandler
uint32_t Service::init() {
	uint32_t err_code;
	// ble_uuid_t ble_uuid;

	// Local scope, used in this function and then discarded.
	// What is the purpose if it is discarded?
	ServiceDataInit serviceDataInit;

	initServiceData(&serviceDataInit);

	// Initialize service structure
	// TODO pass in

	// app's eventHandler
	serviceData.appEventHandler            = nullptr;
	serviceData.connectionHandle           = BLE_CONN_HANDLE_INVALID;
	serviceData.isNotificationSupported    = false;
	//serviceData.battery_level_last         = INVALID_BATTERY_LEVEL;

	// TODO ???
	// Add service
	//BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_BATTERY_SERVICE);

	err_code = sd_ble_gatts_service_add(
			BLE_GATTS_SRVC_TYPE_PRIMARY,
			Uuid::getCustomServiceUUID(),  //&ble_uuid,
			&serviceData.serviceHandle);

	if (err_code == NRF_SUCCESS) {
		err_code = Characteristic::add(&serviceData, &serviceDataInit);
		// Service fails when characteristic fails
	}

	return err_code;
}


const ServiceData* Service::data() {
	return &serviceData;
}
