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

#include <stddef.h>
#include <string.h>

#define EW32_DEVM_API_VERSION_100 1 // 1.00
#define EW32_DEVM_API_VERSION_110 0 // 1.10
#define EW32_DEVM_API_VERSION_150 0 // 1.50

#include "ew32_config.h"
#include "ew32_version.h"
#include "ew32_base/ew32_types.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_devicemanager.h"
#include "ew32_devm/ew32_vfs.h"

#if EW32_DEVICE_USE_I2C == EW32_CONFIG_YES
#include "ew32_devm/ew32_i2c.h"
#endif

#if EW32_DEVICE_USE_UART == EW32_CONFIG_YES
#include "ew32_devm/ew32_uart.h"
#endif

#if EW32_DEVICE_USE_SIMPLE_POWERMANAGER == EW32_CONFIG_YES
#include "ew32_devm/ew32_power.h"
#endif

#if EW32_DEVICE_USE_VNULL == EW32_CONFIG_YES
#include "ew32_devm/ew32_nulldev.h"
#endif

#if EW32_DEVICE_USE_STDDEVICE == EW32_CONFIG_YES
#include "ew32_devm/ew32_std.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

ew32_error_t ew32_init_framework(logging_handler handle, ew32_ui8_t minLevel, ew32_ui8_t maxLevel);

#ifdef __cplusplus
}
#endif