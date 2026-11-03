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

#define EW32_DEVICEMANAGER_STATE_INIT       1
#define EW32_DEVICEMANAGER_STATE_PROBE      2
#define EW32_DEVICEMANAGER_STATE_SETUP      3
#define EW32_DEVICEMANAGER_STATE_RUNNING    4

#define EW32_DEVICEMANAGER_ALL              -1

#define EW32_POWERSTATE_L0                  0  // deep sleep / sensor poweroff
#define EW32_POWERSTATE_L1                  1   
#define EW32_POWERSTATE_L2                  2
#define EW32_POWERSTATE_L3                  3
#define EW32_POWERSTATE_L4                  4
#define EW32_POWERSTATE_L5                  5   // normal powerstate

#define EW32_POWERSTATE_DEVICEOFF           10 // device can be removed

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ew32_devicemanager_create();
uint8_t ew32_devicemanager_registdevice(ew32_dev_t* device,  void* cfg, uint8_t preInit);
uint8_t ew32_devicemanager_get_device(const char* path, ew32_dev_t** device);

uint8_t ew32_devicemanager_probeall();
uint8_t ew32_devicemanager_setup();
uint8_t ew32_devicemanager_run();

uint16_t ew32_devicemanager_get_running();

void ew32_devicemanager_powerstate_set(uint8_t newState);

void ew32_devicemanager_dump(uint8_t level );

uint8_t ew32_devicemanager_setstate(const char* path, uint8_t state_id, uint8_t state);
uint8_t ewi32_devicemanager_setstate(ew32_devid_t id, uint8_t state_id, uint8_t state);

uint8_t ew32_devicemanager_getstate(const char* path, uint8_t state_id);
uint8_t ewi32_devicemanager_getstate(ew32_devid_t id, uint8_t state_id);


void ew32_devicemanager_loop(void* arg);

#ifdef __cplusplus
}
#endif