#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN_1 3  // GPIO for one side of the buzzer

void setup_pwm(uint gpio, uint freq, float duty_cycle) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);  // Set GPIO to PWM mode
    uint slice = pwm_gpio_to_slice_num(gpio);  // Get PWM slice
    uint channel = pwm_gpio_to_channel(gpio);  // Get PWM channel

    uint32_t clkdiv = 50; // Clock divider (adjustable for fine tuning)
    uint32_t wrap = (clock_get_hz(clk_sys) / (clkdiv * freq)) - 1;
    
    pwm_set_clkdiv(slice, clkdiv);  // Set clock divider
    pwm_set_wrap(slice, wrap);  // Set PWM frequency
    pwm_set_chan_level(slice, channel, (uint16_t)(wrap * duty_cycle));  // Set duty cycle
    pwm_set_enabled(slice, true);  // Enable PWM
}

int main() {
    stdio_init_all();

    while(true) {
        setup_pwm(BUZZER_PIN_1, 726, 0.5);  // 50% duty cycle on first pin

        sleep_ms(500);  // Buzzer ON for 2 seconds

        // Disable PWM
        pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN_1), false);

        sleep_ms(2000);  // Buzzer ON for 2 seconds
    }

    return 0;
}