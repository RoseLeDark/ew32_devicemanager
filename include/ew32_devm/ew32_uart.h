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

#ifndef ___UART_EW32_DEVM_H__
#define ___UART_EW32_DEVM_H__

#include "ew32_device.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ew32_uart
{
    uint8_t port_num;
    unsigned long  baud;
    uint32_t config;
    int8_t rxPin;  // -1
    int8_t txPin;  // -1
    ew32_bool_t invert;  // false
    unsigned long timeout_ms; // 20000UL
    uint8_t rxfifo_full_thrhd; // 112
    uint8_t used ;
} ew32_uart_cfg_t;

/**
 * @brief Initializes and assigns a virtual null fallback device node.
 * 
 * Sets up a placeholder device structure with dummy handlers to prevent 
 * null-pointer dereferences for discovered hardware nodes that lack a specific driver.
 * 
 * @param parent Pointer to the parent device (e.g., the bus controller).
 * @param device Double pointer to receive the created virtual device instance.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code on failure.
 */
uint8_t ew32_device_uart_setup_ex(uint8_t port_num, unsigned long baud, uint32_t config, int8_t rx, int8_t tx);

#define ew32_device_uart_setup(port_num, baud, config) ew32_device_uart_setup_ex(port_num, baud, config, -1, -1)

#ifdef __cplusplus
}
#endif


#endif