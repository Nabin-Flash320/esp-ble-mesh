#ifndef __BLE_MESH_MODEL_H__
#define __BLE_MESH_MODEL_H__

#include "esp_ble_mesh_defs.h"
// Custom libs
#include "ble_mesh_definitions.h"
#include "ble_mesh_op_code.h"


static esp_ble_mesh_cfg_srv_t config_server = {
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay = ESP_BLE_MESH_RELAY_DISABLED,
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .beacon = ESP_BLE_MESH_BEACON_ENABLED,
    .friend_state = ESP_BLE_MESH_FRIEND_NOT_SUPPORTED,
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
    .default_ttl = 4,
};

static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
};

// Publication model for custom model 1.
ESP_BLE_MESH_MODEL_PUB_DEFINE(custom_pub_1, 2 + 3, ROLE_NODE);
// Adding custom model to root_model array using structure instead ESP_BLE_MESH_VENDOR_MODEL() macro.
static esp_ble_mesh_model_t custom_LED_model[] = {
    {
        .vnd.company_id = CID_ESP,
        .vnd.model_id = ESP_BLE_MESH_LED_SERVER_MODEL,
        .op = custom_op_code_1,
        .pub = &custom_pub_1,
        .cb = NULL,
        .user_data = NULL,
    },
};

#endif // __BLE_MESH_MODEL_H__