
#include "debug.h"
#include "ble_mesh_callbacks.h"
#include "esp_ble_mesh_defs.h"
#include "LED.h"

void LED_model_callback(esp_ble_mesh_model_cb_param_t *params, void *user_data)
{
    LED_server_struct_struct_t *led_struct = (LED_server_struct_struct_t *)user_data;
    switch (params->model_operation.opcode)
    {
    case 0xC002E5:
    {
        uint32_t value = params->model_operation.msg[0];
        if (0x01 == value)
        {
            configure_LED(led_struct);
        }
        else if (0x00 == value)
        {
            deconfigure_LED(led_struct);
        }
        else
        {
            TRACE_E("Unknown command received.");
        }
        break;
    }
    case 0xC102E5:
    {
        uint32_t value = params->model_operation.msg[0];
        set_LED(led_struct, value);
        break;
    }
    default:
    {
        TRACE_E("Unknown OP code obtained!!");
        break;
    }
    }
}

size_t  LED_publication_callback(void *user_data, uint8_t *data)
{
    LED_server_struct_struct_t *led_struct = (LED_server_struct_struct_t *)user_data;
    size_t len = 2;
    data = (uint8_t *)malloc(2);
    if (data)
    {
        bool state = get_LED_current_state(led_struct);
        TRACE_I("Led state is %d", state);
        *data = state;
    }
    else
    {
        free(data);
    }
    return len;
}

void bluetooth_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *param)
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

void bluetooth_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t *param)
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
                        model id 0x%04x, company id 0x%04x",
                    param->value.state_change.mod_pub_set.element_addr, param->value.state_change.mod_pub_set.pub_addr,
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

void bluetooth_mesh_custon_model_cb(esp_ble_mesh_model_cb_event_t event, esp_ble_mesh_model_cb_param_t *param)
{
    switch (event)
    {
    case ESP_BLE_MESH_MODEL_OPERATION_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_OPERATION_EVT");
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
        break;
    }
    case ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT:
    {
        TRACE_I("ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT");
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