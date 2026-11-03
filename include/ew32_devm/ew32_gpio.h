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

#include "ew32_device.h"

#define EW32_DEVICE_GPIO_USED_I2C           0
#define EW32_DEVICE_GPIO_USED_UART          1
#define EW32_DEVICE_GPIO_USED_PIN           2
#define EW32_DEVICE_GPIO_USED_I2S           3
#define EW32_DEVICE_GPIO_USED_SPI           4
#define EW32_DEVICE_GPIO_USED_TOUCH         5
#define EW32_DEVICE_GPIO_USED_USER          6

#ifdef __cplusplus
extern "C" {
#endif



uint8_t ew32_device_gpio_setup(ew32_dev_t* parent, ew32_dev_t** device);

uint8_t ew32_device_gpio_can_used_pin(uint16_t pin);
uint8_t ew32_device_gpio_marked_pin(uint16_t pin, uint8_t mode, uint16_t dev);

#ifdef __cplusplus
}
#endif