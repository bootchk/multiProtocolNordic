
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
private:
	static void shutdown();
	static void onTimerElapsed();

public:

	/*
	 * units ticks of 30uSec
	 *
	 * Max is 24-bit i.e. 17million i.e 51 seconds
	 *
	 * Min is:
	 *    time to advertise on 3 channels (about 1mSec) and listen for client (???)
	 *    !!! plus time to perform provisioning with a connected client.
	 *
	 * This does NOT include the time to startup and shutdown the protocol stack.
	 *
	 * For a failed provisioning, the radio is only active during the advertisement
	 * and while listening for a client, until the advertisement times out a few mSec later.
	 * For a successful provisioning, the radio is active longer,
	 * while characteristics are exchanged and written.
	 * Elapse time is say a half second, but the radio is not active that entire time.
	 *
	 * !!! The session must be longer than the time for a successful provisioning,
	 * else it cuts off the client.
	 */
	// static const int ProvisioningSessionDuration = 0xfffff0;	// 50 s
	//static const int ProvisioningSessionDuration = 800000;		// 2.4s
	//static const int ProvisioningSessionDuration = 70;	// 2.1 mSec
	static const int ProvisioningSessionDuration = 400000;	// 1.2 s

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

	/*
	 * Start provisioning service.
	 * Service is separate task, this returns immediately.
	 * Service advertises, accepts connections, and writes.
	 * Callbacks when writes are accepted.
	 */
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
	 * Timeout is fixed constant.
	 * Ensures SD disabled on return.
	 */
	static bool provisionWithSleep();
};
