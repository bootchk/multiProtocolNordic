
#pragma once

/*
 * Knows parameters of advertising.
 */
class Advertiser {
public:
	// Build advertising params.
	// Used when advertising.
	static void init();
	static void startAdvertising();
	static void stopAdvertising();
};
