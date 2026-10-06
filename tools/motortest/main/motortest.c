// Bench test for the power board: spins M1..M4 one at a time, no IMU/WiFi needed.
// REMOVE PROPELLERS. Flash esp-drone back when done.
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

static const int pins[4] = {5, 6, 3, 4}; // M1..M4, same as esp-drone Kconfig
#define DUTY_PCT 40                     // calibration knob: raise if a motor won't start

void app_main(void)
{
    ledc_timer_config_t t = {
        .speed_mode = LEDC_LOW_SPEED_MODE, .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT, .freq_hz = 15000, .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&t);
    for (int i = 0; i < 4; i++) {
        ledc_channel_config_t c = {
            .gpio_num = pins[i], .speed_mode = LEDC_LOW_SPEED_MODE, .channel = i,
            .timer_sel = LEDC_TIMER_0, .duty = 0,
        };
        ledc_channel_config(&c);
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // let USB CDC console enumerate

    const uint32_t duty = 1023 * DUTY_PCT / 100;
    while (1) {
        for (int i = 0; i < 4; i++) {
            printf("M%d (GPIO%d) ON  %d%% -> motor ini harus berputar 2 detik\n", i + 1, pins[i], DUTY_PCT);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, i, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, i);
            vTaskDelay(pdMS_TO_TICKS(2000));
            ledc_set_duty(LEDC_LOW_SPEED_MODE, i, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, i);
            printf("M%d OFF\n", i + 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        printf("--- ulang ---\n");
    }
}
