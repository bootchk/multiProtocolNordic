

#include "objects/bleProtocol.h"

#include "nrfLogging.h"



int main(void)
{
	enableNrfLogging();

	BLEProtocol::start();
	BLEProtocol::startAdvertising();

	for (;;){
		// Toggle between protocols

		// TODO
	}
}
