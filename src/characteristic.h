
#pragma once

#include <inttypes.h>
#include "service.h"

class Characteristic {
public:
	static uint32_t add(ServiceData*, ServiceDataInit*);
};
