

#ifndef __BLE_MESH_MODEL_PUB_H__
#define __BLE_MESH_MODEL_PUB_H__

#include "esp_ble_mesh_defs.h"
#include "mesh_buf.h"

#if 0
/** ================================================================== **/
/** Custom model publication instance creation for the custom server. **/
// Creating publication buffer.
static uint8_t custom_server_pub_buffer[20];
// Creating net_buf_simple_instance for model publication.
static struct net_buf_simple custom_server_pub_net_buf = {
    .data = custom_server_pub_buffer,
    .len = 0,
    .size = 20,
    .__buf = custom_server_pub_buffer
};
// Creating model publication instance of the custom server.
static esp_ble_mesh_model_pub_t custom_srv_pub = {
    .msg = &custom_server_pub_net_buf,
    .update = (uint32_t)NULL,
    .dev_role = ROLE_NODE
};
/** ================================================================== **/


/** ================================================================== **/
/** Custom LED model publication instance creation for the custom LED server. **/
// Creating publication buffer.
static uint8_t custom_led_server_pub_buffer[20];
// Creating net_buf_simple_instance for model publication.
static struct net_buf_simple custom_led_server_pub_net_buf = {
    .data = custom_led_server_pub_buffer,
    .len = 0,
    .size = 20,
    .__buf = custom_led_server_pub_buffer
};
// Creating model publication instance of the custom LED server.
static esp_ble_mesh_model_pub_t custom_led_server_pub = {
    .msg = &custom_led_server_pub_net_buf,
    .update = (uint32_t)NULL,
    .dev_role = ROLE_NODE
};
/** ================================================================== **/
#endif

esp_ble_mesh_model_pub_t create_ble_mesh_publication(size_t publishing_device_buffer_size, esp_ble_mesh_dev_role_t publishing_device_role);

#endif // __BLE_MESH_MODEL_PUB_H__


 