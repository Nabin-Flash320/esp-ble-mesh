
#ifndef __BLE_MESH_STRUCTS_H__
#define __BLE_MESH_STRUCTS_H__

#include "esp_ble_mesh_defs.h"

typedef void (*custom_model_callback)(esp_ble_mesh_model_cb_param_t *param, void *user_data);
typedef size_t (*custom_model_publishcation_callback)(uint8_t *data);

typedef struct
{
    custom_model_callback model_cb;
    custom_model_publishcation_callback publication_cb;
    void *args;
} custom_model_user_data_struct_t;

#endif // __BLE_MESH_STRUCTS_H__
