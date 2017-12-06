#pragma once

#include "ble_srv_common.h"

/*
 * GAP Generic Access Profile
 *
 * A Profile is a vertical slice.
 * GAP is a control slice.
 *
 * GATT Generic Attribute Profile.
 * GATT is a data slice.
 *
 * Events
 */
class GAP {
public:
	/*
	 * device name, appearance, and preferred connection parameters
	 */
	static void initParams();
	static void onBleEvent(ble_evt_t const * bleEvent, void * );
};
