

#include "esp_ble_mesh_defs.h"
#include "debug.h"
#include "ble_mesh_model.h"


esp_ble_mesh_model_t ble_mesh_create_mesh_model(const uint16_t mesh_model_id, esp_ble_mesh_model_op_t *model_opcode, esp_ble_mesh_model_pub_t *const model_publication, void *user_data)
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

