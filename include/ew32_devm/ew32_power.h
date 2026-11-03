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

#include "ew32_config.h"
#include "ew32_device.h"
#include "ew32_api.h"

#include <stdio.h>

#define EW32_POWERSTATE_L1                  1 // deep sleep
#define EW32_POWERSTATE_L2                  2
#define EW32_POWERSTATE_L3                  3
#define EW32_POWERSTATE_L4                  4
#define EW32_POWERSTATE_L5                  5  // Normal running

#define EW32_POWERMSG_TEST                  0 // call every n tiscks - return level you will change it
#define EW32_POWERMSG_PRE                   1 // can change? return 0 fpr ok and 1 for wait
#define EW32_POWERMSG_POST                  2 // after change

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t (* ew32_powermanager_handle)(uint8_t curpwstate);
typedef uint8_t (* ew32_powermanager_prechange)(uint8_t curpwstate, uint8_t tostate);

#define EW32_POWERMANAGER_NEW_STATE         1
#define EW32_POWERMANAGER_GET_STATE         1



typedef struct ew32_powermanager_io {
    uint8_t msg;
    uint8_t state;
} ew32_powermanager_io_t;

uint8_t ew32_device_powermanager_setup(ew32_dev_t** device);

void ew32_device_powermanager_sethandle(ew32_powermanager_handle test, ew32_powermanager_prechange prechange);

#ifdef __cplusplus
}
#endif