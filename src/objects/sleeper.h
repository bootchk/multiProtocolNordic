#pragma once

#include <inttypes.h>



class Sleeper {
private:
	static void sleepInBt();
public:
	static void sleepInBtUntilTimeout(uint32_t);


};
