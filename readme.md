
Sequential (not concurrent) multiprotocol using Nordic NRF radio SoC.

Switches sequentially between BLE and proprietary protocols.
Radio and other HW devices used sequentially by protocols.

Use BLE to create a BT peripheral that briefly advertises a characteristic (property)
that a control app (a BT central) can write.
The control app has plenty of power to scan for the peripheral, which sleeps mostly.

The goal is a standalone app that advertises a characteristic, periodically,
shutting down the SoftDevice in between.

Then the goal is to make it a library
and integrate with an app that uses a proprietary protocol most of the time.

Status
-

 work in progress, not working.



Initially derived from NRF SDK examples/multiprotocol/ble_app_gzll
Modified to be object oriented, hopefully more understandable and modifiable.
Then derived from examples/ble_peripheral/ble_app_template, which uses more abstractions from the SDK.

Project is C++
Some hacks necessary to get C code to compile with C++
Project uses Eclipse managed make (no makefile, uses Project>Properties)


Dependencies
-

SDK 14.2

NRF52K

SD132

Burn it to NRF52DK along with SD 132 or 112?


Test it with NRF app for iPhone "NRF Connect."



Configuration
-

Preprocessor symbols in Project Properties

NRF_SD_BLE_API_VERSION=3
NRF52
BLE_STACK_SUPPORT_REQD

No BOARD symbol defined (buttons and leds not used.)



in sdk_config.h

NRF_SDH_BLE_VS_UUID_COUNT = 2
since using two vendor specifig UUIDS

Used many settings from ble_app_template's sdk_configh.h
such as NRF_FSTORAGE_ENABLED (when using Advertising module)


Linker script

Modified to allow plenty or room for SD ram.
Did not use the process where you use the debugger to determine the optimal start address of application RAM.


Other Notes
-

I started with SDK 12.3, there may be improper vestiges of that.

At first, I copied source from SDK into project and change their suffix to .cpp.
They should be linked files, and C flags created properly.
