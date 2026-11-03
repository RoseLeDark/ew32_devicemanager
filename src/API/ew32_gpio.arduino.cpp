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

#if EW32_PLATFORM ==  EW32_PLATFORM_ARDUINO_ESP32 || EW32_PLATFORM_ESP
#include "esp_bit_defs.h"
#include "soc/soc_caps.h"


#define EW32_GPIO_PIN_COUNT SOC_GPIO_PIN_COUNT
#define EW32_GPIO_PIN_MASK SOC_GPIO_VALID_GPIO_MASK
#endif

#if EW32_PLATFORM == EW32_PLATFORM_ARDUINO

#define EW32_GPIO_PIN_COUNT 8
#define EW32_GPIO_PIN_MASK 1

#endif

typedef struct EW32_PACKED {
    uint8_t state; // 0 => resaviert, 1 => free, 2 => using
    uint8_t dev;
    uint8_t mode;
} ew32_gpio_t;

static ew32_gpio_t __ew32_pins[EW32_GPIO_PIN_COUNT];

#define EW32_GPIO_NOT_VALID(p) ((p >= EW32_GPIO_PIN_COUNT) || ((EW32_GPIO_PIN_MASK & (1ULL << p)) == 0))

extern "C" {

uint8_t ew32_device_gpio_can_used_pin(uint16_t pin) {
    if (EW32_GPIO_NOT_VALID(pin)) {

        xewLoggingDebug1("/dev/gpio", "pin is not valid: %d\n", pin);
        return 1;
    }

    

    for(int i= 0; i < EW32_GPIO_PIN_COUNT; i++) {
        if(__ew32_pins[i].state == 0 || __ew32_pins[i].state == 2) return 1;
    }

    

    return 0;
}
uint8_t ew32_device_gpio_marked_pin(uint16_t pin, uint8_t mode, uint16_t dev) {
    if (EW32_GPIO_NOT_VALID(pin)) {
        return 1;
    }

    xewLoggingDebug1("/dev/gpio", "marked pin %d as used\n", pin);

    __ew32_pins[pin].state = 1;
    __ew32_pins[pin].dev = dev;
    __ew32_pins[pin].mode = mode;

    return 0;
}


}