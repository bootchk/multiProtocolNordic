
#pragma once


#include "timerAdaptor.h"	// TimerInterruptReason

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

	/*
	 * Callback from IRQ for Timer.
	 */
	static void provisionElapsedTimerHandler(TimerInterruptReason reason);

	static void init(ProvisioningCallback, ProvisioningCallback );

	/*
	 * Caller might have clocks already started.
	 * i.e. depends on whether provisioner is stand alone using TimerAdaptor
	 * or provisioner is used by some app already started clocks.
	 */
	static void startClocks();

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

	/*
	 * Do a provision session, sleeping in low power when idle.
	 * Does not return until either provisioned or timeout expired.
	 */
	static void provisionWithSleep();
};
