

#include "LED.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "debug.h"

esp_err_t configure_LED(LED_server_struct_struct_t *LED_struct)
{
    esp_err_t ret = ESP_OK;
    if (!LED_struct->LED_configured)
    {
        gpio_config_t LED_config = {
            .pin_bit_mask = (1ULL << LED_struct->LED_gpio),
            .mode = GPIO_MODE_OUTPUT,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ret = gpio_config(&LED_config);
        if (ESP_OK == ret)
        {
            LED_struct->LED_configured = true;
        }
        else
        {
            LED_struct->LED_configured = false;
        }
    }
    else
    {
        TRACE_E("LED GPIO has already been configured.");
        ret = ESP_FAIL;
    }
    return ret;
}

esp_err_t set_LED(LED_server_struct_struct_t *LED_struct, uint32_t val)
{
    esp_err_t ret = ESP_OK;
    if (LED_struct->LED_configured)
    {
        TRACE_E("HERE!! value is %d", val);
        ret = gpio_set_level(LED_struct->LED_gpio, val);
        if (ESP_OK == ret)
        {
            LED_struct->current_state = val;
        }
    }
    else
    {
        TRACE_E("The LED at pin %d has not been configured.", LED_struct->LED_gpio);
        ret = ESP_FAIL;
    }
    return ret;
}
