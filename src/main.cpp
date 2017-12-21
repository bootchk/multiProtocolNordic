

#include <objects/nrfLog.h>
#include "objects/softdevice.h"
#include "objects/bleProtocol.h"
#include "objects/sleeper.h"




int main(void)
{
	NRFLog::enable();

	Softdevice::enable();
	BLEProtocol::start();

	NRFLog::log("start advertising");
	BLEProtocol::startAdvertising();

	for (;;){
		// Toggle between protocols
        // TODO

		NRFLog::log("sleep");
		SoftdeviceSleeper::sleepInBt();
		NRFLog::log("loop");
	}
}
