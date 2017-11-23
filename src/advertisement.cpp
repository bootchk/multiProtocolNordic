
#include "advertisement.h"



#include <inttypes.h>
#include "ble_advdata.h"
#include "app_error.h"	// APP_ERROR_CHECK



// TODO
#define ShortSERVICEUUID 0xFFFF



/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure, m_adv_params, to be passed to the stack
 *          when starting advertising.
 */
void Advertisement::init()
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;


    /*
     * Customize.
     * We provide only one custom service.
     * UUID generated once, and inserted here.
     */
    ble_uuid_t adv_uuids[] =
    {
        {ShortSERVICEUUID, BLE_UUID_TYPE_BLE},
    };


    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}
