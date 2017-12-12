
#pragma once

/*
 * Provides provisioning over BLE, for a short duration.
 *
 *
 * Algebra:
 * enable() start() sleep() (callback occurs) start() sleep() (callback occurs) ....
 */

typedef void (*ProvisioningCallback)();


class Provisioner {
public:
	static void enable(ProvisioningCallback, ProvisioningCallback );

	static void start();

	/*
	 * Has start been called and callback not occurred yet.
	 */
	static bool isProvisioning();

	/*
	 * Sleep system.
	 * Illegal to sleep any other way when isProvisioning()
	 */
	static void sleep();

	/*
	 * Called by BLE app when client provisions.
	 *
	 * Called from appHandler.cpp
	 * i.e. events propagate from BLE resulting in ProvisioningCallback() call back to app.
	 */
	static void onProvisioned();
};
