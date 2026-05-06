#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

/* Tempos do Semáforo */
#define RED_TIME_MS    5000
#define GREEN_TIME_MS  10000
#define YELLOW_TIME_MS 2000

/* Mapeamento via DeviceTree Alias */
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

/* Definição dos Estados */
enum semaphore_state {
    STATE_RED,
    STATE_GREEN,
    STATE_YELLOW
};

void main(void)
{
    int ret;
    enum semaphore_state current_state = STATE_RED;

    /* Validação e Configuração inicial dos pinos */
    if (!gpio_is_ready_dt(&led_red) || !gpio_is_ready_dt(&led_green)) {
        return;
    }

    /* Configura todos como saída e garante que iniciem desligados */
    gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_INACTIVE);

    while (1) {
        switch (current_state) {
        case STATE_RED:
            /* Liga Vermelho, desliga o resto */
            gpio_pin_set_dt(&led_red, 1);
            gpio_pin_set_dt(&led_green, 0);
            
            k_msleep(RED_TIME_MS);
            current_state = STATE_GREEN;
            break;

        case STATE_GREEN:
            /* Liga Verde, desliga o resto */
            gpio_pin_set_dt(&led_red, 0);
            gpio_pin_set_dt(&led_green, 1);
            
            k_msleep(GREEN_TIME_MS);
            current_state = STATE_YELLOW;
            break;

        case STATE_YELLOW:
            /* Liga Amarelo, desliga o resto */
            gpio_pin_set_dt(&led_red, 1);
            gpio_pin_set_dt(&led_green, 1);

            
            k_msleep(YELLOW_TIME_MS);
            current_state = STATE_RED;
            break;
        }
    }
}