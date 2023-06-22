

idf_component_register(SRCS "ble_mesh_callbacks.c" "ble_mesh_op_code.c" "ble_mesh_model.c" "ble_mesh_pub.c"
                        INCLUDE_DIRS "."
                        REQUIRES custom_led_onoff)
