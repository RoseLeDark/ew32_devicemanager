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
#pragma once

#include <stdio.h>
#include <ew32_base/osa/ew32_osa_api.h>
#include "../ew32_device.h"



#define EW32_DEVM_SUPPORT_GPIO 1

#define EW32_DEVICE_GPIO_USED_I2C           0
#define EW32_DEVICE_GPIO_USED_UART          1
#define EW32_DEVICE_GPIO_USED_PIN           2
#define EW32_DEVICE_GPIO_USED_I2S           3
#define EW32_DEVICE_GPIO_USED_SPI           4
#define EW32_DEVICE_GPIO_USED_TOUCH         5
#define EW32_DEVICE_GPIO_USED_USER          6



#define EW32_DEVICE_GPIO_INPUT             0x01
#define EW32_DEVICE_GPIO_OUTPUT            0x03
#define EW32_DEVICE_GPIO_PULLUP            0x04
#define EW32_DEVICE_GPIO_INPUT_PULLUP      0x05
#define EW32_DEVICE_GPIO_PULLDOWN          0x08
#define EW32_DEVICE_GPIO_INPUT_PULLDOWN    0x09
#define EW32_DEVICE_GPIO_OPEN_DRAIN        0x10
#define EW32_DEVICE_GPIO_OUTPUT_OPEN_DRAIN 0x13
#define EW32_DEVICE_GPIO_ANALOG            0xC0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EW32_PACKED {
    uint8_t state; // 0 => resaviert, 1 => free, 2 => using
    uint8_t dev;
    uint8_t mode;
} ew32_gpio_t;


uint8_t ew32_hal_gpio_can_used_pin(uint16_t pin) ;
uint8_t ew32_hal_gpio_marked_pin(uint16_t pin, uint8_t mode, uint16_t dev) ;

uint8_t ew32_hal_gpio_setup_pin(uint16_t pin, uint8_t mode) ;
uint8_t ew32_hal_gpio_digital_set(uint16_t pin, uint8_t c);
int8_t ew32_hal_gpio_digital_get(uint16_t pin);

#ifdef __cplusplus
}
#endif
