

#ifndef __BLE_MESH_CALLBACKS_H__
#define __BLE_MESH_CALLBACKS_H__

#include "esp_ble_mesh_defs.h"

void LED_model_callback(esp_ble_mesh_model_cb_param_t *params, void *user_data);
size_t LED_publication_callback(void *user_data, uint8_t *data);

#endif // __BLE_MESH_CALLBACKS_H__
