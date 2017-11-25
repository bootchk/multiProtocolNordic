
#include "connection.h"

#include <inttypes.h>
#include <string.h>  // memset
#include "ble_conn_params.h"


#ifdef WORK_IN_PROGRESS

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)
/**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)
/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3
/**< Number of attempts before giving up the connection parameter negotiation. */

void Connection::initParams() {

	uint32_t               err_code;
	ble_conn_params_init_t connection_params_init;

	memset(&connection_params_init, 0, sizeof(connection_params_init));

	connection_params_init.p_conn_params                  = NULL;
	connection_params_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	connection_params_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	connection_params_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	connection_params_init.start_on_notify_cccd_handle    = m_hrs.hrm_handles.cccd_handle;
	connection_params_init.disconnect_on_fail             = true;
	connection_params_init.evt_handler                    = NULL;
	connection_params_init.error_handler                  = conn_params_error_handler;

	err_code = ble_conn_params_init(&connection_params_init);
	APP_ERROR_CHECK(err_code);

}


#endif
