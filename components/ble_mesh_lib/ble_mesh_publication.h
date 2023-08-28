
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

#endif // __BLE_MESH_PUBLICATION_H__


