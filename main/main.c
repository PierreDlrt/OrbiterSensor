/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "common.h"
#include "ble_gap.h"

/* Library function declarations */
void ble_store_config_init(void);

void sensor_ble_publish_task(void)
{
    unsigned int d = 0xdac;

    ble_wait_adv_ready();
    ESP_LOGI(TAG, "Starting sensor data advertisement");

    for (;;)
    {
        ble_advertise_data(d);

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main(void)
{
    /* Local variables */
    esp_err_t ret = ESP_OK;

    /* NVS flash initialization */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "failed to initialize nvs flash, error code: %d ", ret);
    }

    ble_start();
    ESP_LOGI(TAG, "Start BLE");

    xTaskCreate((TaskFunction_t)sensor_ble_publish_task, "Advertisement", 4 * 1024, NULL, tskIDLE_PRIORITY + 2, NULL);

    return;
}
