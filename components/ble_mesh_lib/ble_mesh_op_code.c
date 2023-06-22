

#include "esp_ble_mesh_defs.h"
#include "debug.h"
#include "ble_mesh_op_code.h"


esp_ble_mesh_model_op_t ble_mesh_create_opcode(const uint32_t opcode, size_t opcode_message_len)
{
    esp_ble_mesh_model_op_t model_opcode = {
        .opcode = opcode,
        .min_len = opcode_message_len,
        .param_cb = (uint32_t)NULL,
    };
    return model_opcode;
}

