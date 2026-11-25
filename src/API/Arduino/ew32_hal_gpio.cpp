/*
 * SPDX-License-Identifier: EUPL-1.2
 *
 * Copyright (c) 2026 Amber-Sophia "RoseLeBlood" Schröck (gender:divers dey/deren )
 *
 * This file is part of ew32_devm.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the European Union Public Licence (EUPL),
 * version 1.2 or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the European Union Public Licence
 * along with this program. If not, see:
 * https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12
 */

#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_gpio.h"
#include "ew32_base/ew32_logging.h"


#if EW32_PLATFORM ==  EW32_PLATFORM_ARDUINO_ESP32 || EW32_PLATFORM_ESP || EW32_PLATFORM_ARDUINO
#include "esp_bit_defs.h"
#include "soc/soc_caps.h"
#include <Arduino.h>

#include "ew32_devm/API/ew32_hal_gpio.h"

#define EW32_GPIO_PIN_COUNT SOC_GPIO_PIN_COUNT
#define EW32_GPIO_PIN_MASK SOC_GPIO_VALID_GPIO_MASK
#endif

//#define EW32_GPIO_PIN_COUNT 8
//#define EW32_GPIO_PIN_MASK 1

#include "soc/adc_periph.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal-periman.h"
#include "hal/gpio_hal.h"
#include "soc/soc_caps.h"

static ew32_gpio_t __ew32_pins[EW32_GPIO_PIN_COUNT];


#define EW32_GPIO_NOT_VALID(p) ((p >= EW32_GPIO_PIN_COUNT) || ((EW32_GPIO_PIN_MASK & (1ULL << p)) == 0))

extern "C" {

static void EW32_ISR_ATTR __ew32_hal_gpio_isr(void *arg) {
  ew32_gpio_t *isr = (ew32_gpio_t *)arg;

  if (isr->isr_funct) {
    isr->func_isr(isr->pArg);
  }
}
ew32_error_t ew32_hal_gpio_init() {
#if EW32_CONFIG_ENABLE_GPIO_ISR == EW32_CONFIG_YES
    esp_err_t err = gpio_install_isr_service((int)ARDUINO_ISR_FLAG);

    ew32_bool_t _e_bool = (err == ESP_OK) || (err == ESP_ERR_INVALID_STATE);

    return _e_bool ? EW32_ERROR_OK : EW32_ERROR;
#else
    return 0;
#endif
}
ew32_error_t ew32_hal_gpio_can_used_pin(ew32_ui16_t pin) {
    if (EW32_GPIO_NOT_VALID(pin)) {
        xewLoggingDebug1("/dev/gpio", "pin is not valid: %d\n", pin);
        return 1;
    }

    if(__ew32_pins[pin].state != 0 ) {
        xewLoggingDebug1("/dev/gpio", "pin is already in use: %d\n", pin);
        return 1;
    }

    return 0;
}
ew32_error_t ew32_hal_gpio_marked_pin(ew32_ui16_t pin, ew32_ui8_t mode, ew32_ui16_t dev) {
    if (EW32_GPIO_NOT_VALID(pin)) {
        return 1;
    }

    xewLoggingDebug1("/dev/gpio", "marked pin %d as used\n", pin);

    __ew32_pins[pin].state = 1;
    __ew32_pins[pin].dev = dev;
    __ew32_pins[pin].mode = mode;

    return 0;
}
ew32_error_t ew32_hal_gpio_setup_pin(ew32_ui16_t pin, ew32_ui8_t mode) {
    if(ew32_hal_gpio_can_used_pin(pin) != 0) return 1;


    gpio_config_t conf = {
        .pin_bit_mask = (1ULL << pin),         /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_DISABLE,             /*!< GPIO mode: set input/output mode                     */
        .pull_up_en = GPIO_PULLUP_DISABLE,     /*!< GPIO pull-up                                         */
        .pull_down_en = GPIO_PULLDOWN_DISABLE, /*!< GPIO pull-down                                       */
    };

    if (mode < 0x20) {  //io
        conf.mode = (gpio_mode_t) ( mode & (EW32_DEVICE_GPIO_INPUT | EW32_DEVICE_GPIO_OUTPUT) );

        if (mode & EW32_DEVICE_GPIO_PULLUP) {
            conf.pull_up_en = GPIO_PULLUP_ENABLE;
        }
        if (mode & EW32_DEVICE_GPIO_PULLDOWN) {
            conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        }
    }
    if (gpio_config(&conf) != ESP_OK) {
        xewLoggingDebug1("gpio", "IO %u config failed", pin);
        return 1;
    }
    ew32_hal_gpio_marked_pin(pin, mode, EW32_DEVICE_GPIO_USED_GPIO);
    return 0;
}
ew32_error_t ew32_hal_gpio_digital_set(ew32_ui16_t pin, ew32_ui8_t c) {

    gpio_set_level((gpio_num_t)pin, c);
    return 0;
}
ew32_i8_t ew32_hal_gpio_digital_get(ew32_ui16_t pin) {
    return gpio_get_level((gpio_num_t)pin);
}

ew32_bool_t ew32_hal_gpio_isvalid_pin(ew32_ui16_t pin)  {
    return EW32_GPIO_NOT_VALID(pin) ? ewFALSE : ewTRUE;
}

ew32_error_t ew32_hal_gpio_attache_isr(ew32_ui16_t pin, ew32_hal_gpio_isr isrfunc, void *arg, ew32_i32_t intrt, ew32_bool_t functional) {
#if EW32_CONFIG_ENABLE_GPIO_ISR == EW32_CONFIG_YES

    if (EW32_GPIO_NOT_VALID(pin)) {
        xewLoggingDebug1("/dev/gpio", "pin is not valid: %d\n", pin);
        return 1;
    }

    if(__ew32_pins[pin].state != 0 ) {
        xewLoggingDebug1("/dev/gpio", "pin is already in use: %d\n", pin);
        return 1;
    }
    ew32_hal_gpio_setup_pin(pin, EW32_DEVICE_GPIO_INPUT);

    __ew32_pins[pin].state = 1;
    __ew32_pins[pin].func_isr = isrfunc;
    __ew32_pins[pin].isr = ewTRUE;
    __ew32_pins[pin].isr_funct = functional;
    __ew32_pins[pin].pArg = arg;



    gpio_set_intr_type((gpio_num_t)pin, (gpio_int_type_t)(intrt & 0x7));

    if (intrt & 0x8) {
        gpio_wakeup_enable((gpio_num_t)pin, (gpio_int_type_t)(intrt & 0x7));
    }
    gpio_isr_handler_add((gpio_num_t)pin, __ew32_hal_gpio_isr, &__ew32_pins[pin]);

    gpio_hal_context_t gpiohal;
    gpiohal.dev = GPIO_LL_GET_HW(GPIO_PORT_0);
    gpio_hal_input_enable(&gpiohal, pin);

    return 0;
#else
    return 1;
#endif
}



}