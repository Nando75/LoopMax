#include "idf_pins.h"
#include "common/types.h"

using namespace LoopMax::Types;

namespace LoopMax::Core::Hal {

    bool idf_pins::isValidPin(int pin) const {
        return GPIO_IS_VALID_GPIO((gpio_num_t)pin);
    }


    PinResult idf_pins::setMode(int pin, PinMode mode) {
        if (!isValidPin(pin))
            return PinResult::INVALID_PIN;

        gpio_config_t cfg{};
        cfg.pin_bit_mask = (1ULL << pin);
        cfg.intr_type = GPIO_INTR_DISABLE;

        switch (mode) {
            case PinMode::Out:
                cfg.mode = GPIO_MODE_OUTPUT;
                cfg.pull_up_en = GPIO_PULLUP_DISABLE;
                cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
                break;

            case PinMode::In:
                cfg.mode = GPIO_MODE_INPUT;
                cfg.pull_up_en = GPIO_PULLUP_DISABLE;
                cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
                break;

            case PinMode::InPullUp:
                cfg.mode = GPIO_MODE_INPUT;
                cfg.pull_up_en = GPIO_PULLUP_ENABLE;
                cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
                break;

            case PinMode::InPullDown:
                cfg.mode = GPIO_MODE_INPUT;
                cfg.pull_up_en = GPIO_PULLUP_DISABLE;
                cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
                break;

            case PinMode::OpenDrain:
                cfg.mode = GPIO_MODE_OUTPUT_OD;
                cfg.pull_up_en = GPIO_PULLUP_DISABLE;
                cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
                break;

            default:
                return PinResult::UNSUPPORTED_MODE;
        }

        esp_err_t err = gpio_config(&cfg);
        return err == ESP_OK ? PinResult::OK : PinResult::ERROR;
    }

    PinResult idf_pins::write(int pin, PinLevel level) {
        if (!isValidPin(pin))
            return PinResult::INVALID_PIN;

        esp_err_t err = gpio_set_level(
            (gpio_num_t)pin,
            level == PinLevel::High ? 1 : 0
        );

        return err == ESP_OK ? PinResult::OK : PinResult::ERROR;
    }

    PinResult idf_pins::read(int pin, PinLevel& level) {
        if (!isValidPin(pin))
            return PinResult::INVALID_PIN;

        int val = gpio_get_level((gpio_num_t)pin);
        level = val ? PinLevel::High : PinLevel::Low;
        return PinResult::OK;
    }

    // ================= PWM (LEDC) =================

    PinResult idf_pins::setupPwm(int pin, const PwmConfig& cfg) {
        if (!isValidPin(pin))
            return PinResult::INVALID_PIN;

        ledc_timer_config_t timer{};
        timer.speed_mode       = LEDC_LOW_SPEED_MODE;
        timer.timer_num        = LEDC_TIMER_0;
        timer.freq_hz          = cfg.frequency;
        timer.duty_resolution  = (ledc_timer_bit_t)cfg.resolution;
        timer.clk_cfg          = LEDC_AUTO_CLK;

        if (ledc_timer_config(&timer) != ESP_OK)
            return PinResult::ERROR;

        ledc_channel_config_t channel{};
        channel.speed_mode = LEDC_LOW_SPEED_MODE;
        channel.channel    = (ledc_channel_t)cfg.channel;
        channel.timer_sel  = LEDC_TIMER_0;
        channel.gpio_num   = pin;
        channel.duty       = 0;
        channel.hpoint     = 0;

        if (ledc_channel_config(&channel) != ESP_OK)
            return PinResult::ERROR;

        return PinResult::OK;
    }

    PinResult idf_pins::writePwm(int pin, uint32_t duty) {
        // NB: come per Arduino, il mapping pin → channel
        // va gestito a livello superiore
        ledc_channel_t ch = LEDC_CHANNEL_0;

        esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, ch, duty);
        if (err != ESP_OK)
            return PinResult::ERROR;

        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, ch);
        return err == ESP_OK ? PinResult::OK : PinResult::ERROR;
    }

    bool idf_pins::supportsPwm(int pin) const {
        return isValidPin(pin);
    }

}
