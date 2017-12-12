
#include "appTimer.h"

/*
 * Implementation by NRF SDK components/libraries/timer/app_timer.c or app_timer_ble_gzl.c ????
 *
 * Requires path above, and to components/drivers_nrf/delay
 */
#include "app_timer.h"
#include "app_error.h"


//#define APP_TIMER_PRESCALER     0                         /**< Value of the RTC1 PRESCALER register. */
//#define APP_TIMER_MAX_TIMERS    6 + BSP_APP_TIMERS_NUMBER /**< Maximum number of simultaneously created timers. */
//#define APP_TIMER_OP_QUEUE_SIZE 4                         /**< Size of timer operation queues. */

// TODO need to bump count of timers

void AppTimer::init() {
	// Initialize timer module, making it use the scheduler.
	// older SDK APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, nullptr);

	// SDK 14:
	app_timer_init();
}

void AppTimer::createRepeating(app_timer_t* const* timerID, timerHandler aHandler ) {
	ret_code_t err_code;

	err_code = app_timer_create(timerID, APP_TIMER_MODE_REPEATED, aHandler);
	APP_ERROR_CHECK(err_code);
}

void AppTimer::createOneShot(app_timer_t* const* timerID, timerHandler aHandler ) {
	ret_code_t err_code;

	err_code = app_timer_create(timerID, APP_TIMER_MODE_SINGLE_SHOT , aHandler);
	APP_ERROR_CHECK(err_code);
}

void AppTimer::start(app_timer_t* timerID, int milliseconds) {
	ret_code_t err_code;
	err_code = app_timer_start(timerID, APP_TIMER_TICKS(milliseconds), NULL);
	APP_ERROR_CHECK(err_code);
}
