/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "common.h"
#include "ble_gap.h"
#include "esp_sleep.h"
#include "adc.h"

#define SAMPLE_PERIOD 10 // second
#define TAG DEVICE_NAME

void sensor_ble_publish_task(void)
{
    int Vchan2, Vchan3;

    ESP_LOGI(TAG, "Enabling timer wakeup, %ds\n", SAMPLE_PERIOD);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(SAMPLE_PERIOD * 1000000));

    ble_wait_adv_ready();
    ESP_LOGI(TAG, "Starting sensor data advertisement");

    for (;;)
    {
        read_adc(&Vchan2, &Vchan3);

        ESP_LOGI(TAG, "Vchan 2 (1kohm) = %dmV ; Vchan 3 (10kohm) = %dmV\n",
                 Vchan2, Vchan3);

        ble_advertise_data(Vchan2, Vchan3);

        // enter deep sleep
        esp_deep_sleep_start();
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

    init_adc();

    ble_start();
    ESP_LOGI(TAG, "Start BLE");

    xTaskCreate((TaskFunction_t)sensor_ble_publish_task, "Advertisement", 4 * 1024, NULL, tskIDLE_PRIORITY + 2, NULL);

    return;
}
