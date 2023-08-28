
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gap_bt_api.h"
#include "esp_gatts_api.h"
#include "string.h"
#include "esp_gatt_common_api.h"
#include "debug.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#include "ble_mesh_example_init.h"

// My libs
#include "ble_mesh_definitions.h"
#include "ble_mesh_op_code.h"
#include "ble_mesh_model.h"
#include "ble_mesh_structs.h"
#include "LED.h"

static uint8_t dev_uuid[16] = {0xdd, 0xdd};

static esp_ble_mesh_prov_t provision = {
    .uuid = dev_uuid,
    .output_size = 0,
    .output_actions = ESP_BLE_MESH_DISPLAY_NUMBER,
    .input_size = 0,
    .input_actions = ESP_BLE_MESH_PUSH,
};

static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, custom_LED_model),
};

esp_ble_mesh_comp_t composition = {
    .cid = CID_ESP,
    .element_count = ARRAY_SIZE(elements),
    .elements = elements,
};

static void bluetooth_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event,
                                           esp_ble_mesh_prov_cb_param_t *param)
{
    switch (event)
    {
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_PROV_REGISTER_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
    {
        TRACE_I("ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
    {
        TRACE_I("ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
    {
        TRACE_I("ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_PROV_RESET_EVT:
    {
        TRACE_E("ESP_BLE_MESH_NODE_PROV_RESET_EVT");
        break;
    }
    case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT:
    {
        TRACE_I("ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT");
        break;
    }
    case ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT:
    {
        TRACE_E("ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT");
        break;
    }
    default:
    {
        TRACE_E("Default bluetooth mesh provisioning callback event.(code: %d)", event);
        break;
    }
    }
}

static void bluetooth_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event,
                                            esp_ble_mesh_cfg_server_cb_param_t *param)
{
    if (event == ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT)
    {
        switch (param->ctx.recv_op)
        {
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD:
        {
            TRACE_I("ESP ble mesh model operation app key add event.%d", param->ctx.recv_op);
            TRACE_I("net_idx: 0x%04x, app_idx: 0x%04x", param->value.state_change.netkey_add.net_idx,
                    param->value.state_change.appkey_add.app_idx);
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND");
            TRACE_I("elem_addr 0x%04x, app_idx 0x%04x, cid 0x%04x, mod_id 0x%04x",
                    param->value.state_change.mod_app_bind.element_addr,
                    param->value.state_change.mod_app_bind.app_idx,
                    param->value.state_change.mod_app_bind.company_id,
                    param->value.state_change.mod_app_bind.model_id);
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD");
            TRACE_I("elem_addr 0x%04x, sub_addr 0x%04x, cid 0x%04x, mod_id 0x%04x",
                    param->value.state_change.mod_sub_add.element_addr,
                    param->value.state_change.mod_sub_add.sub_addr,
                    param->value.state_change.mod_sub_add.company_id,
                    param->value.state_change.mod_sub_add.model_id);
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_RELAY_SET:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_OP_RELAY_SET");
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET");
            TRACE_I("elem_addr 0x%04x, pub_addr 0x%04x, publish_app_id 0x%04x, cred_flag %d, ttl %d, period %d, retransmit %d, \
                        model id 0x%04x, company id 0x%04x", param->value.state_change.mod_pub_set.element_addr, param->value.state_change.mod_pub_set.pub_addr,
                        param->value.state_change.mod_pub_set.app_idx, param->value.state_change.mod_pub_set.cred_flag, param->value.state_change.mod_pub_set.pub_ttl,
                        param->value.state_change.mod_pub_set.pub_period, param->value.state_change.mod_pub_set.pub_retransmit, 
                        param->value.state_change.mod_pub_set.model_id, param->value.state_change.mod_pub_set.company_id);
            break;
        }
        default:
        {
            TRACE_I("Default recv_op. %d", param->ctx.recv_op);
            break;
        }
        }
    }
}

static void bluetooth_mesh_custon_model_cb(esp_ble_mesh_model_cb_event_t event,
                                           esp_ble_mesh_model_cb_param_t *param)
{
    switch (event)
    {
    case ESP_BLE_MESH_MODEL_OPERATION_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_OPERATION_EVT");
        TRACE_E("Model id is 0x%X", param->model_operation.model->vnd.model_id);
        custom_model_user_data_struct_t *model_struct = (custom_model_user_data_struct_t *)param->model_operation.model->user_data;
        if (NULL != model_struct)
        {
            model_struct->cb(param, model_struct->args);
        }
        break;
    }
    case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_SEND_COMP_EVT");
        break;
    }
    case ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT");
#pragma message("ESP_BLE_MESH_MODEL_PUBLISH_UPDATE_EVT perform operation here, loadprohibited is encountered.");
        break;
    }
    case ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT");
        TRACE_I("The error code is %s", esp_err_to_name(param->model_publish_comp.err_code));
        break;
    }
    case ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT:
    {
        TRACE_I("ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT");
        break;
    }
    case ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT:
    {
        TRACE_E("ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT");
        break;
    }
    case ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_SERVER_MODEL_UPDATE_STATE_COMP_EVT");
        break;
    }
    default:
    {
        TRACE_E("Default event!");
        break;
    }
    }
}

ERROR ble_mesh_initialize()
{
    ERROR error = ESP_OK;
    esp_ble_mesh_register_prov_callback(bluetooth_mesh_provisioning_cb);
    esp_ble_mesh_register_config_server_callback(bluetooth_mesh_config_server_cb);
    esp_ble_mesh_register_custom_model_callback(bluetooth_mesh_custon_model_cb);

    error = esp_ble_mesh_init(&provision, &composition);
    if (error != ESP_OK)
    {
        TRACE_E("Error initializing BLE mesh.");
    }

    error = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    if (error != ESP_OK)
    {
        TRACE_E("Failed to enable mesh node (error %d)", error);
        return error;
    }

    TRACE_I("BLE Mesh Node initialized");

    error = esp_ble_mesh_set_unprovisioned_device_name(DEVICE_NAME);
    if (error != ESP_OK)
    {
        TRACE_E("Failed to set unprovisioned node's name");
        return error;
    }

    return error;
}

void app_main()
{

    ERROR error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    else
    {
        TRACE_I("NVS flash initialized.");
    }

    // Releasing memory previously occupied.
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    TRACE_I("Controller memory released successfully");

    // Configuring the controller and initializing the controller.
    esp_bt_controller_config_t controller_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&controller_cfg));
    TRACE_I("Controller initialization completed successfully.");

    // enabing the bluetooth controller.
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    TRACE_I("Bluetooth controller enabled successfully.");

    // Initializing bluedroid
    ESP_ERROR_CHECK(esp_bluedroid_init());
    TRACE_I("Bluedroid initialized successfully.");

    // Enabling bluedroid
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    TRACE_I("Bluedroid enabled successfully.");

    TRACE_I("Initializing BLE mesh...");
    error = ble_mesh_initialize();
    if (error != ESP_OK)
    {
        TRACE_E("Error initializing BLE mesh.");
    }
    else
    {
        TRACE_I("Successfully initialized BLE mesh.");
    }
}