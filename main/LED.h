

#ifndef __LED_H__
#define __LED_H__

#include "esp_ble_mesh_defs.h"

typedef struct LED_state_struct{
    bool state;
}LED_state_struct_t;

typedef struct LED_server_state_data_struct
{
    esp_ble_mesh_model_t* model;
    LED_state_struct_t state;
}LED_server_state_data_struct_t;

#endif // __LED_H__
