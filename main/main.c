
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

esp_err_t ble_mesh_initialize()
{
    esp_err_t error = ESP_OK;
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

    esp_err_t error = nvs_flash_init();
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