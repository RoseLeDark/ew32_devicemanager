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
#include "ew32_devm/ew32_power.h"
#include <ew32_base/ew32_logging.h>
#include <ew32_devm/ew32_devm.h>
#include <stddef.h>





ew32_semaphore_t __ew32_powerstate_lock;
ew32_powermanager_handle __gloable_test;
ew32_powermanager_prechange __gloabel_prechange;


typedef struct ew32_powermanager_cfg {
    uint8_t _current;
    uint8_t _soll ;
}ew32_power_cfg_t;

uint8_t ew32_powermanager_probe(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_powermanager_setup(ew32_dev_t* device, void* cfg) {
    __ew32_powerstate_lock = ew32_semaphore_create(EW32_SEMAPHORE_BIN_MUTEX);
    return 0;
}
uint8_t ew32_powermanager_get_state(ew32_dev_t* device, uint8_t stateid) {
    return (stateid == EW32_DEVICE_STATEID_POWERSTATE) ? 5 : 0;
}
uint8_t ew32_powermanager_set_state(ew32_dev_t* device, uint8_t state_id, uint8_t state) {
    if(state_id == EW32_DEVICE_STATEID_MODE) {
        xewLoggingDebug1(device->subpath, "set mode state to: %d\n", state);
        device->state = state;
    }
    return 0;
}
int32_t ew32_powermanager_write(ew32_dev_t* device, void* data, int32_t size) {

    if(device->userdata == NULL) return -1;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    if(data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t *_msg = (ew32_powermanager_io_t*)data;

        if(_msg->msg == EW32_POWERMANAGER_NEW_STATE) {

            ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
            _pstate->_soll = _msg->state;
            ew32_semaphore_unlock(__ew32_powerstate_lock);
        }
    }

    return size;
}
int32_t ew32_powermanager_read(ew32_dev_t* device, void* data, int32_t size) {
    if(device->userdata == NULL) return -1;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    if(data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t *_msg = (ew32_powermanager_io_t*)data;

        if(_msg->msg == EW32_POWERMANAGER_GET_STATE) {
            ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
            _msg->state = _pstate->_current;
            ew32_semaphore_unlock(__ew32_powerstate_lock);
        }
    }
    return size;
}


void ew32_powermanager_onrun(struct ew32device* device, uint64_t ticks) {
    if(device->state == EW32_DEVICE_STATE_SUSPEND) {
        xewLoggingDebug(device->subpath, "PowerManager is suspend\n");
    }
    if(device->userdata == NULL) return ;
    ew32_power_cfg_t* _pstate = (ew32_power_cfg_t*)device->userdata;

    uint8_t _soll = _pstate->_soll;

    if(_soll == _pstate->_current && (__gloable_test != NULL) ) {
        _soll = __gloable_test(_pstate->_current);
    }

    if(_soll != _pstate->_current) {
        uint8_t _mustChange =  (__gloabel_prechange == 0) ? 1 : __gloabel_prechange(_pstate->_current, _soll) == 0;

        if(_mustChange) {
            _pstate->_current = _soll;
            _pstate->_soll = _soll;

            ew32_devicemanager_powerstate_set(_pstate->_soll);
        }
    }
}

ew32_dev_t __global_powermanager;
ew32_power_cfg_t __global_powermanager_state;



void ew32_device_powermanager_sethandle(ew32_powermanager_handle test, ew32_powermanager_prechange prechange) {
    __gloable_test = test;
    __gloabel_prechange = prechange;
}
uint8_t ew32_device_powermanager_setup(ew32_dev_t** device){
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    __global_powermanager.subpath[0] = 'b';
    __global_powermanager.subpath[1] = 'u';
    __global_powermanager.subpath[2] = 's';
    __global_powermanager.subpath[3] = EW32_VFS_DELIMETER;
    __global_powermanager.subpath[4] = 'p';
    __global_powermanager.subpath[5] = 'm';
    __global_powermanager.subpath[6] = 'n';
    __global_powermanager.subpath[7] = '0';

    // Instanz initialisieren
    __global_powermanager.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_powermanager.type = EW32_DEVICE_TYPE_POWERMANAGER;
    __global_powermanager.parent = NULL;
    __global_powermanager.on_run_r = ew32_powermanager_onrun;
    __global_powermanager.rw =  EW32_DEVICE_READ_WRITE;
    __global_powermanager.set_state = ew32_powermanager_set_state;
    __global_powermanager.get_state = ew32_powermanager_get_state;
    __global_powermanager.probe = ew32_powermanager_probe;
    __global_powermanager.setup = ew32_powermanager_setup;
    __global_powermanager.write = ew32_powermanager_write;
    __global_powermanager.read = ew32_powermanager_read;

    __global_powermanager_state._current = 5;
    __global_powermanager_state._soll = 5;
    __global_powermanager.userdata = &__global_powermanager_state;
    __global_powermanager.state =EW32_DEVICE_STATE_SUSPEND;

    *device = &__global_powermanager;


    return EW32_DEVICE_NO_ERROR;
}