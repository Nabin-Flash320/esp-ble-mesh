
#ifndef __BLE_MESH_STRUCTS_H__
#define __BLE_MESH_STRUCTS_H__

#include "esp_ble_mesh_defs.h"

typedef void (*custom_model_user_callback)(esp_ble_mesh_model_cb_param_t *param, void *user_data);

typedef struct
{
    custom_model_user_callback cb;
    void *args;
} custom_model_user_data_struct_t;

#endif // __BLE_MESH_STRUCTS_H__
