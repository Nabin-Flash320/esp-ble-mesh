
#include "debug.h"

#include "esp_ble_mesh_defs.h"
#include "LED.h"

void LED_model_callback(esp_ble_mesh_model_cb_param_t *params, void *user_data)
{
    LED_server_struct_struct_t *led_struct = (LED_server_struct_struct_t *)user_data;
    switch (params->model_operation.opcode)
    {
    case 0xC002E5:
    {
        uint32_t value = params->model_operation.msg[0];
        if (0x01 == value)
        {
            configure_LED(led_struct);
        }
        else if (0x00 == value)
        {
            deconfigure_LED(led_struct);
        }
        else
        {
            TRACE_E("Unknown command received.");
        }
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

size_t  LED_publication_callback(void *user_data, uint8_t *data)
{
    LED_server_struct_struct_t *led_struct = (LED_server_struct_struct_t *)user_data;
    size_t len = 2;
    data = (uint8_t *)malloc(2);
    if (data)
    {
        bool state = get_LED_current_state(led_struct);
        TRACE_I("Led state is %d", state);
        *data = state;
    }
    else
    {
        free(data);
    }
    return len;
}
