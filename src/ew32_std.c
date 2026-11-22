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

#include "ew32_devm/ew32_std.h"

#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_std.h"
#include <ew32_base/ew32_logging.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "ew32_devm/ew32_devm.h"

#define EW32_DEVICE_CLASS_INSTD     0x99
#define DEVICE __global_ew32_virtualdev_device__

ew32_dev_t DEVICE[3];

uint8_t ew32_std_probe(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_std_setup(ew32_dev_t* device, void* cfg) {
    return 0;
}

int32_t ew32_std_write(ew32_dev_t* device, void* data, int32_t size) {
    if (device == NULL || data == NULL || size <= 0) return -1;

    char* __inf = (char*)data;
    int _user = device->raw_userdata;
    ew32_dev_t *parent = device->parent;
    int32_t _ret = size;

    if(_user == EW32_STDOUT) {
        if (parent == NULL) {
            xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "stdout", "%.*s", (int)size, (const char*)data);
        } else {
            _ret = ew32_device_send(parent, data, size);
        }
    } else if(_user == EW32_STDERR) {
        if (parent == NULL) {
            xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "stderr", "%.*s", (int)size, (const char*)data);
        } else {
            _ret = ew32_device_send(parent, data, size);
        }
    }

    return size;
}
int32_t ew32_std_read(ew32_dev_t* device, void* data, int32_t size) {
    if (device == NULL || data == NULL || size <= 0) return -1;

    int _user = device->raw_userdata;
    ew32_dev_t *parent = device->parent;
    int32_t _ret = size;

    if (_user != EW32_STDIN) return 0;

    if (parent != NULL) {
        return ew32_device_read(parent, data, size);
    }

    char* _buf = (char*)data;
    int32_t _i = 0;

    while (_i < size) {
        int _c = getc(stdin);
        if (_c == EOF) break;
        _buf[_i++] = (char)_c;
    }

    return _i;
}




void ew32_dev_std_setdevice(const uint8_t id, ew32_dev_t* parent) {
    if (id >= 3) return;

    if (parent != NULL && strcmp(parent->subpath, DEVICE[id].subpath) == 0) {
        DEVICE[id].parent = NULL;
        return;
    }

    DEVICE[id].parent = parent;
}

uint8_t ew32_dev_std_setup(const uint8_t id, ew32_dev_t** device) {
  
    if (id >= 3) return EW32_DEVICE_ERROR_INVALID_ARG; /* bzw. dein passender Fehlercode */
    
    memset(&DEVICE[id], 0, sizeof(ew32_dev_t));

    // Instanz initialisieren
    DEVICE[id].subpath[0] = 's';
    DEVICE[id].subpath[1] = 't';
    DEVICE[id].subpath[2] = 'd';

    if(id == EW32_STDOUT)  {
        DEVICE[id].subpath[3] = 'o';
        DEVICE[id].subpath[4] = 'u';
        DEVICE[id].subpath[5] = 't';
    }
    if(id == EW32_STDERR)  {
        DEVICE[id].subpath[3] = 'e';
        DEVICE[id].subpath[4] = 'r';
        DEVICE[id].subpath[5] = 'r';
    }
    if(id == EW32_STDIN)  {
        DEVICE[id].subpath[3] = 'i';
        DEVICE[id].subpath[4] = 'n';
    }

    DEVICE[id].cls = EW32_DEVICE_CLASS_INSTD;
    DEVICE[id].type = EW32_DEVICE_TYPE_STREAM;
    DEVICE[id].rw = (id == EW32_STDIN) ? EW32_DEVICE_READ_ONLY : EW32_DEVICE_WRITE_ONLY;
    DEVICE[id].parent = NULL;
    DEVICE[id].set_state = 0;
    DEVICE[id].get_state = 0;
    DEVICE[id].probe = ew32_std_probe;
    DEVICE[id].setup = ew32_std_setup;
    DEVICE[id].write = ew32_std_write;
    DEVICE[id].read = ew32_std_read;

    DEVICE[id].raw_userdata = id;
    DEVICE[id].parent = NULL;
    DEVICE[id].on_run = NULL;

    //*device = &DEVICE[id];
    return ew32_devicemanager_registdevice(&DEVICE[id], NULL, 1);
}