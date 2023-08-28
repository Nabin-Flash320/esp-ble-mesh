

#ifndef __LED_H__
#define __LED_H__

#include "esp_ble_mesh_defs.h"
#include "esp_err.h"

typedef struct LED_state_struct
{
    bool current_state;
    bool previous_state;
    uint32_t LED_gpio;
    bool inverted_output;
    bool LED_configured;
} LED_server_struct_struct_t;

esp_err_t configure_LED(LED_server_struct_struct_t *LED_struct);
esp_err_t set_LED(LED_server_struct_struct_t *LED_struct, uint32_t val);
esp_err_t deconfigure_LED(LED_server_struct_struct_t *LED_struct);
bool get_LED_current_state(LED_server_struct_struct_t *LED_struct);


#endif // __LED_H__
