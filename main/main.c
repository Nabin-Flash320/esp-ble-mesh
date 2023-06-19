
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
#include "driver/gpio.h"
#include "LED.h"

#define ERROR esp_err_t
#define TAG __FILE__
#define DEVICE_NAME "ESP32 BLE Mesh Example"
#define CID_ESP 0x02E5
#define PIN 14

#define ESP_BLE_MESH_SERVER_MODEL_NUM_1 0x0001
#define ESP_BLE_MESH_SERVER_MODEL_NUM_2 0x0002

#define ESP_BLE_MESH_SERVER_MODEL_1_OP_SET ESP_BLE_MESH_MODEL_OP_3(0x00, CID_ESP)
#define ESP_BLE_MESH_SERVER_MODEL_1_OP_STATUS ESP_BLE_MESH_MODEL_OP_3(0x01, CID_ESP)

#define ESP_BLE_MESH_SERVER_MODEL_2_SET ESP_BLE_MESH_MODEL_OP_3(0x02, CID_ESP)
#define ESP_BLE_MESH_SERVER_MODEL_2_STATUS ESP_BLE_MESH_MODEL_OP_3(0x03, CID_ESP)


static const gpio_config_t config = {
    .pin_bit_mask = (1ULL << PIN),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

int isOn = 0;

static uint8_t dev_uuid[16] = {0xdd, 0xdd};

static esp_ble_mesh_cfg_srv_t config_server = {
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay = ESP_BLE_MESH_RELAY_DISABLED,
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .beacon = ESP_BLE_MESH_BEACON_ENABLED,
    .friend_state = ESP_BLE_MESH_FRIEND_NOT_SUPPORTED,
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
    .default_ttl = 4,
};

ESP_BLE_MESH_MODEL_PUB_DEFINE(onoff_pub_0, 2+3, ROLE_NODE);
static esp_ble_mesh_gen_onoff_srv_t onoff_server_0 = {
    .rsp_ctrl.get_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
    .rsp_ctrl.set_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
};

// Publication model for custom model 1;
ESP_BLE_MESH_MODEL_PUB_DEFINE(custom_pub_1, 2+3, ROLE_NODE);
// Op-Code for custom model 1.
static esp_ble_mesh_model_op_t custom_op_code_1[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_SERVER_MODEL_1_OP_SET, 2),
    ESP_BLE_MESH_MODEL_OP_END
};

// Publication model for cusotm model 2;
ESP_BLE_MESH_MODEL_PUB_DEFINE(custom_pub_2, 2+3, ROLE_NODE);
static esp_ble_mesh_model_op_t custom_op_code_2[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_SERVER_MODEL_2_SET, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};

static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
    ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV(&onoff_pub_0, &onoff_server_0), 
};

// Adding custom model to root_model array.
static esp_ble_mesh_model_t custom_LED_model[] = {
    ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_SERVER_MODEL_NUM_1, custom_op_code_1, &custom_pub_1, NULL), 
    ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_SERVER_MODEL_NUM_2, custom_op_code_2, &custom_pub_2, NULL),
};

static esp_ble_mesh_prov_t provision =  {
        .uuid                   =           dev_uuid,
        .output_size            =           0,
        .output_actions         =           ESP_BLE_MESH_DISPLAY_NUMBER,
        .input_size             =           0,
        .input_actions          =           ESP_BLE_MESH_PUSH,
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
    switch(event){
        case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:{
            TRACE_I("ESP_BLE_MESH_PROV_REGISTER_COMP_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:{
            TRACE_I("ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:{
            TRACE_I("ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:{
            TRACE_I("ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:{
            TRACE_I("ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_RESET_EVT:{
            TRACE_E("ESP_BLE_MESH_NODE_PROV_RESET_EVT");
            break;
        }
        case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:{
            TRACE_I("ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT");
            break;
        }
        case ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT:{
            TRACE_I("ESP_BLE_MESH_PROXY_SERVER_CONNECTED_EVT");
            break;
        }
        case ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT:{
            TRACE_E("ESP_BLE_MESH_PROXY_SERVER_DISCONNECTED_EVT");
            break;
        }
        default:{
            TRACE_E("Default bluetooth mesh provisioning callback event.(code: %d)", event);
            break;
        }
    }
}

static void bluetooth_mesh_generic_server_cb(esp_ble_mesh_generic_server_cb_event_t event,
                                             esp_ble_mesh_generic_server_cb_param_t *param)
{
    esp_ble_mesh_gen_onoff_srv_t *srv;
    TRACE_I("event 0x%02x, opcode 0x%04x, src 0x%04x, dst 0x%04x",
            event, param->ctx.recv_op, param->ctx.addr, param->ctx.recv_dst);

    switch (event)
    {
    case ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT:
        TRACE_I("ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT");
        if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET ||
            param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK)
        {
            TRACE_I("onoff 0x%02x", param->value.state_change.onoff_set.onoff);
            isOn = !isOn;
            gpio_set_level(PIN, isOn);
            TRACE_I("%d", isOn);
        }
        break;
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT:
        TRACE_I("ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT");
        if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET)
        {
            srv = param->model->user_data;
            TRACE_I("onoff 0x%02x", srv->state.onoff);
        }
        break;
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT:
        TRACE_I("ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT");
        if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET ||
            param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK)
        {
            TRACE_I("onoff 0x%02x, tid 0x%02x", param->value.set.onoff.onoff, param->value.set.onoff.tid);
            if (param->value.set.onoff.op_en)
            {
                TRACE_I("trans_time 0x%02x, delay 0x%02x",
                        param->value.set.onoff.trans_time, param->value.set.onoff.delay);
            }
        }
        break;
    default:
        ESP_LOGE(TAG, "Unknown Generic Server event 0x%02x", event);
        break;
    }
}

static void bluetooth_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event, 
                                    esp_ble_mesh_cfg_server_cb_param_t *param)
{
    if(event == ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT)
    {
        switch(param->ctx.recv_op){
            case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD:{
                TRACE_I("ESP ble mesh model operation app key add event.%d", param->ctx.recv_op);
                TRACE_I("net_idx: 0x%04x, app_idx: 0x%04x", param->value.state_change.netkey_add.net_idx, 
                                                            param->value.state_change.appkey_add.app_idx);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND:{
                TRACE_I("ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND");
                TRACE_I("elem_addr 0x%04x, app_idx 0x%04x, cid 0x%04x, mod_id 0x%04x",
                         param->value.state_change.mod_app_bind.element_addr,
                         param->value.state_change.mod_app_bind.app_idx,
                         param->value.state_change.mod_app_bind.company_id,
                         param->value.state_change.mod_app_bind.model_id);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD:{
                TRACE_I("ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD");
                TRACE_I("elem_addr 0x%04x, sub_addr 0x%04x, cid 0x%04x, mod_id 0x%04x",
                         param->value.state_change.mod_sub_add.element_addr,
                         param->value.state_change.mod_sub_add.sub_addr,
                         param->value.state_change.mod_sub_add.company_id,
                         param->value.state_change.mod_sub_add.model_id);
                break;
            }
            case ESP_BLE_MESH_MODEL_OP_RELAY_SET:{
                TRACE_I("ESP_BLE_MESH_MODEL_OP_RELAY_SET");
                break;
            }
            default:{
                TRACE_I("Default recv_op. %d", param->ctx.recv_op);
                break;
            }
        }
    }
}


static void bluetooth_mesh_custon_model_cb(esp_ble_mesh_model_cb_event_t event,
                                             esp_ble_mesh_model_cb_param_t *param)
{
    switch(event)
    {
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_OPERATION_EVT");
            TRACE_I("Received OP code is 0x%X; Length of received message is %d", param->model_operation.opcode, param->model_operation.length);
            for(int i = 0; i < param->model_operation.length; i++)
            {
                TRACE_E("Message is 0x%2X", param->model_operation.msg[i]);
            }
            TRACE_E("Model id is 0x%X", param->model_operation.model->vnd.model_id);
            TRACE_E("Model callback is %s", ((param->model_operation.model->cb == NULL) ? "NULL" : "!NULL"));
            break;
        }
        case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
        {
            TRACE_I("ESP_BLE_MESH_MODEL_SEND_COMP_EVT");
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
    esp_ble_mesh_register_generic_server_callback(bluetooth_mesh_generic_server_cb);
    esp_ble_mesh_register_custom_model_callback(bluetooth_mesh_custon_model_cb);

    error = esp_ble_mesh_init(&provision, &composition);
    if(error != ESP_OK)
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

    return error;
}

 
void app_main()
{



    ERROR error = nvs_flash_init();
    if(error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    else
    {
        TRACE_I("NVS flash initialized.");
    }
    gpio_config(&config);
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
    if(error != ESP_OK)
    {
        TRACE_E("Error initializing BLE mesh.");
    }
    else
    {
        TRACE_I("Successfully initialized BLE mesh.");
    }

}