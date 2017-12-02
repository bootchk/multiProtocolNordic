#pragma once

#include "ble_srv_common.h"

class GAP {
public:
	/*
	 * device name, appearance, and preferred connection parameters
	 */
	static void initParams();
	static void onBleEvent(ble_evt_t const * bleEvent, void * );
};
