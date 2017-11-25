#pragma once

// Enable NRF modules

#define BLE_ADVERTISING_ENABLED 1
#define NRF_BLE_GATT_ENABLED 1
// TODO what size
#define NRF_BLE_GATT_MAX_MTU_SIZE 23


// Logging

// local config file.  nrf log source includes this sdk_config.h, so this is convenient place to insure seggerConfig.h is found
#include "seggerConfig.h"	//

// enable module
#define NRF_LOG_ENABLED 1

// configure options of module
#define NRF_LOG_DEFAULT_LEVEL 4		// 4 is Debug level, verbose
#define NRF_LOG_USES_TIMESTAMP 0
#define NRF_LOG_TIMESTAMP_DIGITS 0
#define NRF_LOG_DEFERRED 0
#define NRF_LOG_USES_COLORS 0
// backend rtt requires additional source files
#define NRF_LOG_BACKEND_SERIAL_USES_RTT 1
#define NRF_LOG_BACKEND_MAX_STRING_LENGTH 255

