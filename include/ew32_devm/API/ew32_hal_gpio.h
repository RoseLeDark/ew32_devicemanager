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
#define EW32_DEVICE_GPIO_USED_GPIO          6
#define EW32_DEVICE_GPIO_USED_USER          7



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

typedef void (*ew32_hal_gpio_isr)(void*);

typedef struct EW32_PACKED {
    ew32_ui8_t state; // 0 => resaviert, 1 => free, 2 => using
    ew32_ui8_t dev;
    ew32_ui8_t mode;

    ew32_bool_t isr;
    ew32_hal_gpio_isr func_isr;
    ew32_bool_t isr_funct;
    void* pArg;

} ew32_gpio_t;

#if CONFIG_ARDUINO_ISR_IRAM
#define EW32_ISR_ATTR       IRAM_ATTR
#define EW32_ISR_FLAG       ESP_INTR_FLAG_IRAM
#else
#define EW32_ISR_ATTR
#define EW32_ISR_FLAG (0)
#endif

ew32_error_t ew32_hal_gpio_init();

ew32_bool_t ew32_hal_gpio_isvalid_pin(ew32_ui16_t pin) ;

ew32_error_t ew32_hal_gpio_can_used_pin(ew32_ui16_t pin) ;
ew32_error_t ew32_hal_gpio_marked_pin(ew32_ui16_t pin, ew32_ui8_t mode, ew32_ui16_t dev) ;

ew32_error_t ew32_hal_gpio_setup_pin(ew32_ui16_t pin, ew32_ui8_t mode) ;
ew32_error_t ew32_hal_gpio_digital_set(ew32_ui16_t pin, ew32_ui8_t c);
ew32_i8_t ew32_hal_gpio_digital_get(ew32_ui16_t pin);

ew32_error_t ew32_hal_gpio_attache_isr(ew32_ui16_t pin, ew32_hal_gpio_isr isrfunc, void *arg, ew32_i32_t intrt, ew32_bool_t functional);

#ifdef __cplusplus
}
#endif
