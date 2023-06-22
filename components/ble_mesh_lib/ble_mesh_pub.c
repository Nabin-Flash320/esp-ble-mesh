
#include "esp_ble_mesh_defs.h"
#include "debug.h"
#include "ble_mesh_pub.h"

esp_ble_mesh_model_pub_t create_ble_mesh_publication(size_t publishing_device_buffer_size, esp_ble_mesh_dev_role_t publishing_device_role)
{
    // Creating publication buffer.
    uint8_t server_pub_buffer[publishing_device_buffer_size];
    // Creating net_buf_simple_instance for model publication.
    struct net_buf_simple server_pub_net_buf = {
        .data = server_pub_buffer,
        .len = 0,
        .size = publishing_device_buffer_size,
        .__buf = server_pub_buffer,
    };
    // Creating model publication instance of the custom LED server.
    esp_ble_mesh_model_pub_t server_pub = {
        .msg = &server_pub_net_buf,
        .update = (uint32_t)NULL,
        .dev_role = ROLE_NODE,
    };
    return server_pub;
}
