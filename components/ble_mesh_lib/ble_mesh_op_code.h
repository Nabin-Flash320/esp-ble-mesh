
#ifndef _BLE_MESH_OP_CODE_H_
#define _BLE_MESH_OP_CODE_H_

#include "esp_ble_mesh_defs.h"

#include "ble_mesh_definitions.h"
// LED controller.
#include "LED.h"

// Creating Op-Codes for LED server model.
#define ESP_BLE_MESH_LED_SERVER_MODEL_OP_CONFIG ESP_BLE_MESH_MODEL_OP_3(0x00, CID_ESP)
#define ESP_BLE_MESH_LED_SERVER_MODEL_OP_SET ESP_BLE_MESH_MODEL_OP_3(0x01, CID_ESP)
#define ESP_BLE_MESH_LED_SERVER_MODEL_OP_STATUS ESP_BLE_MESH_MODEL_OP_3(0x02, CID_ESP)


// Op-Code for custom model 1.
static esp_ble_mesh_model_op_t custom_op_code_1[] = {
    {
        .opcode = ESP_BLE_MESH_LED_SERVER_MODEL_OP_CONFIG,
        .min_len = 1,
        .param_cb = (uint32_t)NULL,
    },
    {
        .opcode = ESP_BLE_MESH_LED_SERVER_MODEL_OP_SET,
        .min_len = 1,
        .param_cb = (uint32_t)NULL,
    },
    ESP_BLE_MESH_MODEL_OP_END,
};


#endif // _BLE_MESH_OP_CODE_H_
