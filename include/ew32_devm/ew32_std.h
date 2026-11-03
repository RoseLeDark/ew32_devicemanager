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

#define EW32_STDOUT  0
#define EW32_STDERR  1
#define EW32_STDIN   2

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ew32_dev_std_setup(const uint8_t id, ew32_dev_t** device);

void ew32_dev_std_setdevice(const uint8_t id, ew32_dev_t* parent);

#ifdef __cplusplus
}
#endif
