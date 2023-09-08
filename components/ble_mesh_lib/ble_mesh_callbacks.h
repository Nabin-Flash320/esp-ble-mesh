

#ifndef __BLE_MESH_CALLBACKS_H__
#define __BLE_MESH_CALLBACKS_H__

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_config_model_api.h"

void LED_model_callback(esp_ble_mesh_model_cb_param_t *params, void *user_data);
size_t LED_publication_callback(void *user_data, uint8_t *data);
void bluetooth_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *param);
void bluetooth_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t *param);
void bluetooth_mesh_custon_model_cb(esp_ble_mesh_model_cb_event_t event, esp_ble_mesh_model_cb_param_t *param);

#endif // __BLE_MESH_CALLBACKS_H__
