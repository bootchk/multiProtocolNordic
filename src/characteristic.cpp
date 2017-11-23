
#include "characteristic.h"

#include <string.h>	// memset



uint32_t Characteristic::add(ServiceData * serviceData, ServiceDataInit * serviceDataInit) {
	//static uint32_t battery_level_char_add(ble_bas_t * p_bas, const ble_bas_init_t * p_bas_init)
	//{
	uint32_t            err_code;

	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;

	ble_uuid_t          ble_uuid;

	ble_gatts_attr_md_t attr_md;

	uint8_t             initialValue;

	//uint8_t             encoded_report_ref[BLE_SRV_ENCODED_REPORT_REF_LEN];
	//uint8_t             init_len;

	if (serviceData->isNotificationSupported)
	{
		memset(&cccd_md, 0, sizeof(cccd_md));

		// According to BAS_SPEC_V10, the read operation on cccd should be possible without
		// authentication.
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
		cccd_md.write_perm = serviceDataInit->battery_level_char_attr_md.cccd_write_perm;
		cccd_md.vloc       = BLE_GATTS_VLOC_STACK;
	}

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read   = 1;
	char_md.char_props.notify = (serviceData->isNotificationSupported) ? 1 : 0;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = (serviceData->isNotificationSupported) ? &cccd_md : NULL;
	char_md.p_sccd_md         = NULL;

	// TODO
	BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_BATTERY_LEVEL_CHAR);

	memset(&attr_md, 0, sizeof(attr_md));

	attr_md.read_perm  = serviceDataInit->battery_level_char_attr_md.read_perm;
	attr_md.write_perm = serviceDataInit->battery_level_char_attr_md.write_perm;
	attr_md.vloc       = BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth    = 0;
	attr_md.wr_auth    = 0;
	attr_md.vlen       = 0;

	initialValue = serviceDataInit->initialValue;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = sizeof(uint8_t);
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = sizeof(uint8_t);
	attr_char_value.p_value   = &initialValue;

	err_code = sd_ble_gatts_characteristic_add(serviceData->serviceHandle, &char_md,
			&attr_char_value,
			&serviceData->characteristicHandles);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	/* Omit HID stuff
	if (serviceDataInit->p_report_ref != NULL)
	{
		// Add Report Reference descriptor
		BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_REPORT_REF_DESCR);

		memset(&attr_md, 0, sizeof(attr_md));

		attr_md.read_perm = serviceDataInit->battery_level_report_read_perm;
		BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

		attr_md.vloc    = BLE_GATTS_VLOC_STACK;
		attr_md.rd_auth = 0;
		attr_md.wr_auth = 0;
		attr_md.vlen    = 0;

		init_len = ble_srv_report_ref_encode(encoded_report_ref, serviceDataInit->p_report_ref);

		memset(&attr_char_value, 0, sizeof(attr_char_value));

		attr_char_value.p_uuid    = &ble_uuid;
		attr_char_value.p_attr_md = &attr_md;
		attr_char_value.init_len  = init_len;
		attr_char_value.init_offs = 0;
		attr_char_value.max_len   = attr_char_value.init_len;
		attr_char_value.p_value   = encoded_report_ref;

		err_code = sd_ble_gatts_descriptor_add(serviceData->characteristicHandles.value_handle,
				&attr_char_value,
				&serviceData->report_ref_handle);
		if (err_code != NRF_SUCCESS)
		{
			return err_code;
		}
	}
	else
	{
		serviceData->report_ref_handle = BLE_GATT_HANDLE_INVALID;
	}
	*/

	return NRF_SUCCESS;

}
