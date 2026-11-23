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

#ifdef __cplusplus
extern "C" {
#endif

uint32_t ew32_api_uart_write(uint32_t portnum, const uint8_t* buf, uint32_t size);
uint32_t ew32_api_uart_read(uint32_t portnum, uint8_t* buf, uint32_t size);

uint8_t ew32_hal_uart_setup(ew32_dev_t* device, void* cfg) ;
uint8_t ew32_hal_uart_probe(ew32_dev_t* device, void* cfg) ;
int32_t ew32_hal_uart_write(ew32_dev_t* device, const void* data, const  int32_t size) ;
int32_t ew32_hal_uart_read(ew32_dev_t* device, void* data, const  int32_t size) ;
uint32_t ew32_hal_uart_avaible(uint32_t portnum);

#ifdef __cplusplus
}
#endif
