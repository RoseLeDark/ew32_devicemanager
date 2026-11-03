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

#ifdef __cplusplus
extern "C" {
#endif

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
uint8_t ew32_device_virtualnull_setup(ew32_dev_t* parent, ew32_dev_t** device);

#ifdef __cplusplus
}
#endif