#pragma once

#include <inttypes.h>


/*
 * Knows how to sleep for Provisioner.
 * i.e. knows SD is enabled and coordinates with it
 */
class Sleeper {
private:
	static void sleepInSD();
public:
	static void sleepInSDUntilTimeout(uint32_t);
};
