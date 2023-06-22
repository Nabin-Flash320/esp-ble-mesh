#ifndef __BLE_MESH_MODEL_H__
#define __BLE_MESH_MODEL_H__

#include "esp_ble_mesh_defs.h"
// Custom libs
#include "ble_mesh_definitions.h"
#include "ble_mesh_op_code.h"
#include "ble_mesh_structs.h"
#include "ble_mesh_callbacks.h"

// LED controller.
#include "LED.h"


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
// Creating user data.
static LED_server_struct_struct_t LED_struct = {
    .current_state = false,
    .inverted_output = false,
    .LED_gpio = 3,
    .previous_state = false,
    .LED_configured = false,
};

static custom_model_user_data_struct_t LED_model_struct = {
    .cb = LED_callback,
    .args = (void*)&LED_struct,
};

// Adding custom model to root_model array using structure instead ESP_BLE_MESH_VENDOR_MODEL() macro.
static esp_ble_mesh_model_t custom_LED_model[] = {
    {
        .vnd.company_id = CID_ESP,
        .vnd.model_id = ESP_BLE_MESH_LED_SERVER_MODEL,
        .op = custom_op_code_1,
        .pub = &custom_pub_1,
        .cb = NULL,
        .user_data = (void*)&LED_model_struct,
    },
};

esp_ble_mesh_model_t ble_mesh_create_mesh_model(uint16_t mesh_model_id, esp_ble_mesh_model_op_t *model_opcode, esp_ble_mesh_model_pub_t *const model_publication, void *user_data)
{
    esp_ble_mesh_model_t mesh_model = {
        .vnd.company_id = CID_ESP,
        .vnd.model_id = mesh_model_id,
        .op = model_opcode,
        .pub = model_publication,
        .cb = NULL,
        .user_data = user_data,
    };
    return mesh_model;
}

#endif // __BLE_MESH_MODEL_H__