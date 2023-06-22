
#include "debug.h"

#include "esp_ble_mesh_defs.h"
#include "LED.h"

void LED_callback(esp_ble_mesh_model_cb_param_t *params, void *user_data)
{
    TRACE_I("OP code received is 0x%X", params->model_operation.opcode);
    LED_server_struct_struct_t *led_struct = (LED_server_struct_struct_t *)user_data;
    switch (params->model_operation.opcode)
    {
    case 0xC002E5:
    {
        configure_LED(led_struct);
        break;
    }
    case 0xC102E5:
    {
        uint32_t value = params->model_operation.msg[0];
        set_LED(led_struct, value);
        break;
    }
    default:
    {
        TRACE_E("Unknown OP code obtained!!");
        break;
    }
    }
}