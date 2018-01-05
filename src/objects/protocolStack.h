
#pragma once


/*
 * A stack of layers for communication.
 * Stack includes:
 * - Nordic Softdevice (low-level coordination of HW resources)
 * - Radio protocol (BLE or other)
 *
 * Simply hides the fact of two layers.
 */

class ProtocolStack {
public:
	static void startup();
	static void shutdown();

	static bool isEnabled();
};
