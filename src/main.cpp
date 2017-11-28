

#include <objects/nrfLog.h>
#include "objects/bleProtocol.h"




int main(void)
{
	NRFLog::enable();

	BLEProtocol::start();

	NRFLog::log("start advertising");
	BLEProtocol::startAdvertising();

	for (;;){
		// Toggle between protocols

		// TODO
		NRFLog::log("looping");
	}
}
