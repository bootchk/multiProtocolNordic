
#pragma once

class BLEProtocol {
public:
	static const int ProtocolTag = 1;

	static void start();
	static void startAdvertising();
	static void stopAdvertising();
	static void stop();
};
