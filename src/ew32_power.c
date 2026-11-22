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

#if EW32_DEVICE_USE_SIMPLE_POWERMANAGER == EW32_CONFIG_YES

#include "ew32_devm/ew32_power.h"
#include <ew32_base/ew32_logging.h>
#include <ew32_devm/ew32_devm.h>
#include <stddef.h>
#include <string.h>

#include "ew32_devm/ew32_devm.h"

#define DEVICE __global_ew32_powerstate_device__


typedef struct ew32_powermanager_cfg {
    uint8_t _current;
    uint8_t _soll;

    ew32_powermanager_handle test;
    ew32_powermanager_prechange prechange;
} ew32_power_cfg_t;

ew32_dev_t DEVICE;

uint8_t ew32_powermanager_probe(ew32_dev_t* device, void* cfg) {
    return 0;
}

uint8_t ew32_powermanager_setup(ew32_dev_t* device, void* cfg) {
    if (device == NULL || device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    return 0;
}

uint8_t ew32_powermanager_get_state(ew32_dev_t* device, uint8_t stateid) {
    if (device == NULL) return 0;
    return (stateid == EW32_DEVICE_STATEID_POWERSTATE) ? device->state : 0;
}


uint8_t ew32_powermanager_set_state(ew32_dev_t* device, uint8_t state_id, uint8_t state) {
    if (device == NULL) return EW32_DEVICE_ERROR_NULL;
    if (state_id == EW32_DEVICE_STATEID_MODE) {
        xewLoggingDebug1(device->subpath, "set mode state to: %d\n", state);
        device->state = state;
    }
    return 0;
}

int32_t ew32_powermanager_write(ew32_dev_t* device, void* data, int32_t size) {
    if (device == NULL || device->userdata == NULL) return -1;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    if (data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t* _msg = (ew32_powermanager_io_t*)data;

        if (_msg->msg == EW32_POWERMANAGER_NEW_STATE) {
            _pstate->_soll = _msg->state;
        }
    }

    return size;
}


int32_t ew32_powermanager_read(ew32_dev_t* device, void* data, int32_t size) {
    if (device == NULL || device->userdata == NULL) return -1;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    if (data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t* _msg = (ew32_powermanager_io_t*)data;

        if (_msg->msg == EW32_POWERMANAGER_GET_STATE) {
            _msg->state = _pstate->_current;
        }
    }

    return size;
}


void ew32_powermanager_onrun(struct ew32device* device, uint64_t ticks) {
    if (device == NULL) return;

    if (device->state == EW32_DEVICE_STATE_SUSPEND) {
        xewLoggingDebug(device->subpath, "PowerManager is suspend\n");
    }
    if (device->userdata == NULL) return;

    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    uint8_t _current = _pstate->_current;
    uint8_t _soll = _pstate->_soll;
    ew32_powermanager_handle _test = _pstate->test;
    ew32_powermanager_prechange _prechange = _pstate->prechange;


    if (_soll == _current && (_test != NULL)) {
        _soll = _test(_current);
    }

    if (_soll != _current) {
        uint8_t _mustChange = (_prechange == NULL) ? 1 : (_prechange(_current, _soll) == 0);

        if (_mustChange) {
            _pstate->_current = _soll;
            _pstate->_soll = _soll;

            ew32_devicemanager_powerstate_set(_soll);
        }
    }
}





void ew32_device_powermanager_sethandle(ew32_powermanager_handle test, ew32_powermanager_prechange prechange) {
    if (DEVICE.userdata == NULL) return;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)DEVICE.userdata;

    _pstate->test = test;
    _pstate->prechange = prechange;
}

uint8_t ew32_device_powermanager_setup(ew32_dev_t** device){

    static ew32_power_cfg_t _cfg;
    memset(&_cfg, 0, sizeof(ew32_power_cfg_t));
    _cfg._current = 5;
    _cfg._soll = 5;

    DEVICE.subpath[0] = 'b';
    DEVICE.subpath[1] = 'u';
    DEVICE.subpath[2] = 's';
    DEVICE.subpath[3] = EW32_VFS_DELIMETER;
    DEVICE.subpath[4] = 'p';
    DEVICE.subpath[5] = 'm';
    DEVICE.subpath[6] = 'n';
    DEVICE.subpath[7] = '0';

    // Instanz initialisieren
    DEVICE.cls = ES32_DEVICE_CLASS_VIRTUAL;
    DEVICE.type = EW32_DEVICE_TYPE_POWERMANAGER;
    DEVICE.parent = NULL;
    DEVICE.on_run_r = ew32_powermanager_onrun;
    DEVICE.rw =  EW32_DEVICE_READ_WRITE;
    DEVICE.set_state = ew32_powermanager_set_state;
    DEVICE.get_state = ew32_powermanager_get_state;
    DEVICE.probe = ew32_powermanager_probe;
    DEVICE.setup = ew32_powermanager_setup;
    DEVICE.write = ew32_powermanager_write;
    DEVICE.read = ew32_powermanager_read;

    DEVICE.userdata = &_cfg;

    DEVICE.state =EW32_DEVICE_STATE_SUSPEND;

   //*device = &DEVICE;

    return ew32_devicemanager_registdevice(&DEVICE, NULL, 1);
}

#endif