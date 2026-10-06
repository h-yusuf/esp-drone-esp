// Bench test: ramps ALL 4 motors together to find the duty where the battery sags so far the ESP resets.
// REMOVE PROPELLERS. Run on battery (USB unplugged). Flash esp-drone back when done.
//
// Ramp: 10% -> 100%, +5% every 2 s. Progress is saved to NVS before each step, so after a reset
// the next boot knows how far it got. Result is shown on the blue LED (GPIO15) and printed on USB:
//   LED solid          = ramp running
//   N blinks + pause   = last run died at N*10..N*10+9 % (e.g. 6 blinks = 60-69 %)
//   slow blink         = last run reached 100 % without dying
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_system.h"

static const int pins[4] = {5, 6, 3, 4}; // M1..M4
#define LED 15
#define STEP_PCT 5
#define STEP_MS 2000

static nvs_handle_t nvs;

static void motors(int pct)
{
    for (int i = 0; i < 4; i++) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, i, 1023 * pct / 100);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, i);
    }
}

static void save(const char *key, int32_t v)
{
    nvs_set_i32(nvs, key, v);
    nvs_commit(nvs);
}

static void show_result_forever(int32_t died_at)
{
    while (1) {
        if (died_at < 0) { // reached 100 %
            printf("RESULT: last run reached 100%% with no reset\n");
            gpio_set_level(LED, 1); vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_set_level(LED, 0); vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        printf("RESULT: last run died at %ld%% duty (thrust ~%ld)\n", (long)died_at, (long)died_at * 65535 / 100);
        for (int i = 0; i < died_at / 10; i++) {
            gpio_set_level(LED, 1); vTaskDelay(pdMS_TO_TICKS(250));
            gpio_set_level(LED, 0); vTaskDelay(pdMS_TO_TICKS(250));
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    ledc_timer_config_t t = {
        .speed_mode = LEDC_LOW_SPEED_MODE, .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT, .freq_hz = 15000, .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&t);
    for (int i = 0; i < 4; i++) {
        ledc_channel_config_t c = {.gpio_num = pins[i], .speed_mode = LEDC_LOW_SPEED_MODE, .channel = i,
                                   .timer_sel = LEDC_TIMER_0, .duty = 0};
        ledc_channel_config(&c);
    }
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    nvs_flash_init();
    nvs_open("thrust", NVS_READWRITE, &nvs);

    int32_t running = 0, step = 0;
    nvs_get_i32(nvs, "running", &running);
    nvs_get_i32(nvs, "step", &step);

    if (running) { // previous ramp was interrupted -> that's the result
        save("running", 0);
        save("result", step);
        printf("Previous ramp interrupted (reset reason %d) at %ld%%\n", esp_reset_reason(), (long)step);
        show_result_forever(step);
    }

    int32_t result = -2;
    nvs_get_i32(nvs, "result", &result);
    if (result != -2) printf("Stored result from earlier run: %ld (-1 = reached 100%%)\n", (long)result);

    vTaskDelay(pdMS_TO_TICKS(5000)); // hands off, let USB console enumerate
    save("running", 1);
    gpio_set_level(LED, 1);
    for (int pct = 10; pct <= 100; pct += STEP_PCT) {
        save("step", pct);
        printf("RAMP %d%%\n", pct);
        motors(pct);
        vTaskDelay(pdMS_TO_TICKS(STEP_MS));
    }
    motors(0);
    save("running", 0);
    save("result", -1);
    show_result_forever(-1);
}
