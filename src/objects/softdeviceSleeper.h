#pragma once

#include <inttypes.h>


/*
 * Knows how to sleep for Provisioner i.e. knows SD is enabled and coordinates with it
 */
class SoftdeviceSleeper {
public:
	/*
	 * Sleep until any one event.
	 */
	static void sleepInSDUntilAnyEvent();

	/*
	 * Sleep until SD is disabled OR duration.
	 * May return before duration has elapsed.
	 * Ensures SD disabled on return.
	 * (When duration elapses, a timer's event handler disables SD.)
	 *
	 * If SD is not enabled when called, returns immediately.
	 */
	static void sleepInSDUntilTimeout(uint32_t duration);
};
