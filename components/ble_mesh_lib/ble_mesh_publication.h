
#ifndef __BLE_MESH_PUBLICATION_H__
#define __BLE_MESH_PUBLICATION_H__

#include "esp_ble_mesh_defs.h"
#include "mesh_buf.h"
#include "LED.h"

static uint8_t net_buf_data_LED_server_net_buf[20];
static struct net_buf_simple LED_server_net_buf = {
    .data = net_buf_data_LED_server_net_buf,
    .len = 0,
    .size = 20,
    .__buf = net_buf_data_LED_server_net_buf,
};
static esp_ble_mesh_model_pub_t LED_server_publication = {
    .update = (uint32_t)NULL,
    .msg = &LED_server_net_buf,
    .dev_role = ROLE_NODE,
};

inline static struct net_buf_simple ble_mesh_get_net_buf(size_t net_buf_size)
{
    uint8_t *mesh_server_data_buf = (uint8_t *)malloc(net_buf_size * sizeof(uint8_t));
    struct net_buf_simple mesh_server_net_buf = {
        .data = mesh_server_data_buf,
        .len = 0,
        .size = net_buf_size,
        .__buf = mesh_server_data_buf,
    };
    return mesh_server_net_buf;
}

inline static esp_ble_mesh_model_pub_t mesh_server_publication_init(uint8_t mesh_node_role, size_t net_buf_size, struct net_buf_simple mesh_server_net_buf)
{
    esp_ble_mesh_model_pub_t mesh_server_publication = {
        .update = (uint32_t)NULL,
        .msg = &mesh_server_net_buf,
        .dev_role = mesh_node_role,
    };
    return mesh_server_publication;
}

#endif // __BLE_MESH_PUBLICATION_H__
