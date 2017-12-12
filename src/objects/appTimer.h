
#pragma once

#include "app_timer.h"



typedef void (*timerHandler)(void *);


/*
 * Facade to Nordic app_timer
 */
class AppTimer {
public:
	static void init();
	static void createRepeating(app_timer_t* const*, timerHandler);
	static void createOneShot(app_timer_t* const*, timerHandler);
	static void start(app_timer_t *, int);
};
