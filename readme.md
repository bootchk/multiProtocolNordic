
Sequential (not concurrent) multiprotocol using Nordic NRF radio SoC.

Switches sequentially between BLE and proprietary protocols.
Radio and other HW devices used sequentially by protocols.

Use BLE to create a BT peripheral that briefly advertises a characteristic (property)
that a control app (a BT central) can write.
The control app has plenty of power to scan for the peripheral, which sleeps mostly.

Experimental.

Derived from NRF SDK examples/multiprotocol/ble_app_gzll

Project is C++
Some hacks necessary to get C code to compile with C++
Project uses Eclipse managed make (no makefile, uses Project>Properties)

The goal is a standalone app that advertises a characteristic, periodically,
shutting down the SoftDevice in between.

Then the goal is to make it a library
and integrate with an app that uses a proprietary protocol most of the time.

Burn it to NRF52DK along with SD 132 or 112?
Test it with NRF app for iPhone "NRF Connect."


