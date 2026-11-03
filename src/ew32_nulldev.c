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

#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_nulldev.h"
#include <ew32_base/ew32_logging.h>
#include <stddef.h>
#include <string.h>

uint8_t ew32_nulldev_probe(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_probe\n");

    return 0;
}
uint8_t ew32_nulldev_setup(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_setup\n");
    return 0;
}
uint8_t ew32_nulldev_get_state(ew32_dev_t* device, uint8_t state) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_get_state\n");
    return 0;
}

uint8_t ew32_nulldev_set_state(ew32_dev_t* device, uint8_t stateid, uint8_t state) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_set_state\n");
    return 0;
}
int32_t ew32_nulldev_write(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_write\n");
    memset(data, 0, size);
    return 0;
}
int32_t ew32_nulldev_read(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_read\n");
    memset(data, 0, size);
    return 0;
}

ew32_dev_t __global_device;

uint8_t ew32_device_virtualnull_setup(ew32_dev_t* parent, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_device.subpath[0] = 'n';
    __global_device.subpath[1] = 'u';
    __global_device.subpath[2] = 'l';
    __global_device.subpath[3] = 'l';

    __global_device.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_device.type = EW32_DEVICE_TYPE_STREAM;
    __global_device.parent = NULL;
    __global_device.rw =  EW32_DEVICE_READ_WRITE;
    __global_device.set_state = ew32_nulldev_set_state;
    __global_device.get_state = ew32_nulldev_get_state;
    __global_device.probe = ew32_nulldev_probe;
    __global_device.setup = ew32_nulldev_setup;
    __global_device.write = ew32_nulldev_write;
    __global_device.read = ew32_nulldev_read;
    __global_device.on_run = NULL;
    __global_device.userdata = NULL;
    __global_device.parent = parent;

    *device = &__global_device;
    return EW32_DEVICE_NO_ERROR;
}