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

#ifdef __cplusplus
extern "C" {
#endif

ew32_bool_t ew32_hal_i2c_isadress(ew32_dev_t* device, ew32_ui8_t address);
ew32_error_t ew32_hal_i2c_probe(ew32_dev_t* device, void*);
ew32_error_t ew32_hal_i2c_setup(ew32_dev_t* device, void*) ;
ew32_ssize_t ew32_hal_i2c_read(ew32_dev_t* device, void* data, const ew32_size_t size) ;
ew32_ssize_t ew32_hal_i2c_write(ew32_dev_t* device, const void* data, const ew32_size_t size) ;
ew32_error_t ew32_hal_i2c_set_state(ew32_dev_t* device, ew32_ui8_t state_id, ew32_state_t state);
ew32_state_t ew32_hal_i2c_get_state(ew32_dev_t* device, ew32_ui8_t state_id) ;

#ifdef __cplusplus
}
#endif
