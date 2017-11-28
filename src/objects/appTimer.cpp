
#include "appTimer.h"

/*
 * Implementation by NRF SDK components/libraries/timer/app_timer.c or app_timer_ble_gzl.c ????
 *
 * Requires path above, and to components/drivers_nrf/delay
 */
#include "app_timer.h"


#define APP_TIMER_PRESCALER     0                         /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS    6 + BSP_APP_TIMERS_NUMBER /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE 4                         /**< Size of timer operation queues. */

void AppTimer::init() {
	// Initialize timer module, making it use the scheduler.
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, nullptr);
}
