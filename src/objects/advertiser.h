
#pragma once

/*
 * Hides implementation of advertising.
 *
 * Two alternatives, using either:
 * - Nordic Advertising module
 * - direct advertising to SD
 *
 * The former:
 * - also requires app_timer?
 * - can sequence through modes (fast, slow, etc.)
 * - can handle creating bonds
 *
 * The latter:
 * - requires handling more events in GAP?
 *
 * Deciding between alternatives:
 *
 */
class Advertiser {
public:
	static void init();
	static void startAdvertising(bool);
	static void stopAdvertising(bool);
};
