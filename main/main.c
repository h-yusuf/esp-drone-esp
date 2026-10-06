/*
 * ESP-Drone Firmware
 * 
 * Copyright 2019-2020  Espressif Systems (Shanghai) 
 * Copyright (C) 2011-2012 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "nvs_flash.h"

#include "stm32_legacy.h"
#include "platform.h"
#include "system.h"
#include "log.h"
#define DEBUG_MODULE "APP_MAIN"
#include "debug_cf.h"

#ifdef CONFIG_DEBUG_PRINT_ATTITUDE
static void attitudePrintTask(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(8000)); // wait for logInit() in the system task
    logVarId_t r = logGetVarId("stabilizer", "roll"), p = logGetVarId("stabilizer", "pitch");
    logVarId_t y = logGetVarId("stabilizer", "yaw"), t = logGetVarId("stabilizer", "thrust");
    logVarId_t m1 = logGetVarId("pwm", "m1_pwm"), m2 = logGetVarId("pwm", "m2_pwm");
    logVarId_t m3 = logGetVarId("pwm", "m3_pwm"), m4 = logGetVarId("pwm", "m4_pwm");
    logVarId_t v = logGetVarId("pm", "vbat");
    while (1) {
        printf("ATT roll=%7.2f pitch=%7.2f yaw=%7.2f | thrust=%6.0f | pwm=%5d %5d %5d %5d | vbat=%.2f\n",
               logGetFloat(r), logGetFloat(p), logGetFloat(y), logGetFloat(t),
               logGetInt(m1), logGetInt(m2), logGetInt(m3), logGetInt(m4), logGetFloat(v));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
#endif

void app_main()
{
    /*
    * Initialize the platform and Launch the system task
    * app_main will initialize and start everything
    */

    /* S2 Mini: give the host time to enumerate USB CDC console before the
     * heavy init below; without this the port never shows up on macOS. */
    vTaskDelay(pdMS_TO_TICKS(5000));

    /* initialize nvs flash prepare for Wi-Fi */
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    /*Initialize the platform.*/
    if (platformInit() == false) {
        while (1);//if  firmware is running on the wrong hardware, Halt
    }

    /*launch the system task */
    systemLaunch();

#ifdef CONFIG_DEBUG_PRINT_ATTITUDE
    xTaskCreate(attitudePrintTask, "ATTPRINT", 3072, NULL, 1, NULL);
#endif

}
