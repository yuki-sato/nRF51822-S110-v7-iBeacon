// nRF51822 beacon sample
#include <stdbool.h>
#include <stdint.h>
#include "softdevice_handler.h"
#include "ble_advdata.h"

//===================================================================//
// BLE Configrations
//===================================================================//
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                 /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define APP_CFG_NON_CONN_ADV_TIMEOUT  0                                 /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */
#define NON_CONNECTABLE_ADV_INTERVAL  MSEC_TO_UNITS(100, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */

#define APP_BEACON_INFO_LENGTH        0x17                              /**< Total length of information advertised by the Beacon. */
#define APP_ADV_DATA_LENGTH           0x15                              /**< Length of manufacturer specific data in the advertisement. */
#define APP_DEVICE_TYPE               0x02                              /**< 0x02 refers to Beacon. */
#define APP_COMPANY_IDENTIFIER        0x004C                            /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */

#define APP_BEACON_UUID               0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01
#define APP_MAJOR_VALUE               0x00, 0x01                        /**< Major value used to identify Beacons. */ 
#define APP_MINOR_VALUE               0x00, 0x02                        /**< Minor value used to identify Beacons. */ 
#define APP_MEASURED_RSSI             0xC8                              /**< The Beacon's measured RSSI at 1 meter distance in dBm. */

#define DEAD_BEEF                     0xDEADBEEF                        /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

//===================================================================//
// GlobalValues
//===================================================================//
static ble_gap_adv_params_t m_adv_params;

static uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] = {
    APP_DEVICE_TYPE,
    APP_ADV_DATA_LENGTH,
    APP_BEACON_UUID,
    APP_MAJOR_VALUE,
    APP_MINOR_VALUE,
    APP_MEASURED_RSSI
};

//===================================================================//
// BLE Protocol
//===================================================================//
static void ble_stack_init(void) {
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_250MS_CALIBRATION, false);
	
    // Enable BLE stack 
    uint32_t err_code;
    ble_enable_params_t ble_enable_params;
	
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
}

static void advertising_init(void) {	
    memset(&m_adv_params, 0, sizeof(m_adv_params));
	m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_adv_params.p_peer_addr = NULL;                             // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = NON_CONNECTABLE_ADV_INTERVAL;
    m_adv_params.timeout     = APP_CFG_NON_CONN_ADV_TIMEOUT;
	
	uint32_t        err_code;
    ble_advdata_t   advdata;
    uint8_t         flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
	
    ble_advdata_manuf_data_t manuf_specific_data;
    manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;
    manuf_specific_data.data.p_data        = (uint8_t *) m_beacon_info;
    manuf_specific_data.data.size          = APP_BEACON_INFO_LENGTH;

    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.flags.size              = sizeof(flags);
    advdata.flags.p_data            = &flags;
    advdata.p_manuf_specific_data   = &manuf_specific_data;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}

static void advertising_start(void) {
    uint32_t err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}

//===================================================================//
// Main
//===================================================================//
int main(void) {
    ble_stack_init();
	sd_ble_gap_tx_power_set(4);          // accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm
    advertising_init();
    advertising_start();
	
    while(1){
    }
}

//===================================================================//
// SoftDeviceCallbackHandler
//===================================================================//
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    //ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

// nrf_assert.hに定義されている
//また、softdevice_handler.cでsoftdevice_assertion_handlerをレジストしていてそいつがこれを呼ぶ
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}
