

#include <objects/nrfLog.h>
#include "objects/bleProtocol.h"




int main(void)
{
	//NRFLog::enable();

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	for (;;){
		// Toggle between protocols

		// TODO
	}
}
