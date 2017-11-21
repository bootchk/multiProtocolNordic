
Sequential (not concurrent) multiprotocol using Nordic NRF radio SoC.

Switches sequentially between BLE and proprietary protocols.
Radio and other HW devices used sequentially by protocols.

Use BLE to create a BT peripheral that briefly advertises a characteristic (property)
that a control app (a BT central) can write.
The control app has plenty of power to scan for the peripheral, which sleeps mostly.

Experimental.

