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

#include "ew32_devm/ew32_devm.h"

#include <stdio.h>
#include <string.h>

#include "ew32_devm/ew32_i2c.h"
#include "ew32_devm/ew32_vfs.h"
#include "ew32_devm/ew32_power.h"
#include "ew32_devm/ew32_nulldev.h"
#include "ew32_devm/ew32_std.h"
#include "ew32_base/ew32_logging.h"

typedef struct ew32_devmanager_entry {
    ew32_dev_t* device;
    int8_t      probe_state; // = 0 (no probe), 1 (probe), 2 (init/run) - (-1= not used), -10 = error
    void*       cfg;
} ew32_devm_entry_t;  

ew32_devm_entry_t __devman[EW32_DEVICE_MANAGER_DEVICES];
uint8_t           __devm_global_state = 0;


#define __ew32_devicemanager_add_device(ID) \
    ew32_device_probe(__devman[ID].device, NULL); \
    ew32_device_setup(__devman[ID].device, NULL); \
    __devman[ID].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING; \
    __devman[ID].device->id = ew32_vfsdb_regist_device(__devman[ID].device->subpath, __devman[ID].device, 1);

uint8_t ew32_devicemanager_create(){
    if(__devm_global_state != 0) return 0;
     __devm_global_state = EW32_DEVICEMANAGER_STATE_INIT;
    
    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        __devman[i].device = NULL;
        __devman[i].probe_state = -1;
        __devman[i].cfg = NULL;
    }

    /// setup i2c
   ew32_device_i2c_setup(21, 22, 0, &__devman[0].device);
   __ew32_devicemanager_add_device(0);

    /// setup powermanager
    ew32_device_powermanager_setup(&__devman[1].device);
   __ew32_devicemanager_add_device(1);

    // NULL device
   ew32_device_virtualnull_setup(NULL, &__devman[2].device);
    __ew32_devicemanager_add_device(2);

   ew32_dev_std_setup(EW32_STDOUT, &__devman[3].device);
   __ew32_devicemanager_add_device(3);

    ew32_dev_std_setup(EW32_STDERR, &__devman[4].device);
    __ew32_devicemanager_add_device(4);

    ew32_dev_std_setup(EW32_STDIN, &__devman[5].device);
    __ew32_devicemanager_add_device(5);

    return 0;
}

ew32_devid_t get_free_id() {
    int16_t _id = -1;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == -1) {
            __devman[i].probe_state = 0;
            _id = i;
            break;
        }
    }

    return _id;
} 

uint8_t ew32_devicemanager_registdevice(ew32_dev_t* device, void* cfg, uint8_t preInit) {
    int16_t _id = get_free_id();
    uint8_t _ret = 0;

    if(_id == -1) return EW32_DEVICEMANAGER_FULL;

    xewLoggingDebug1("DEVM", "regist new device with id: %d\n", _id);
    __devman[_id].device = device;
    __devman[_id].cfg = cfg;
    __devman[_id].probe_state = 0;


    if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_PROBE || preInit == 1) {// allready_probe
        
        _ret = ew32_device_probe(__devman[_id].device, __devman[_id].cfg);

        if(_ret == EW32_DEVICE_NO_ERROR)   {
            __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_PROBE;
        }
    }

    if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_SETUP || preInit == 1 ) {
        _ret = ew32_device_setup(__devman[_id].device, __devman[_id].cfg);
        
        if(_ret == EW32_DEVICE_NO_ERROR)   { 
            __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_SETUP;
        } 
    }

    if(preInit == 1) {
        __devman[_id].device->id = ew32_vfsdb_regist_device(__devman[_id].device->subpath, 
                                                      __devman[_id].device, 
                                                      _ret == EW32_DEVICE_NO_ERROR ? 1 : 0);
    }
    

    return _ret;
}

uint8_t ew32_devicemanager_probeall() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_INIT) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_PROBE; 

    uint8_t _devret = 0;
    uint16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == 0) {
            _devret = ew32_device_probe(__devman[i].device, __devman[i].cfg);
            if (_devret == 0) __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_PROBE ;
            else { 
                __devman[i].probe_state = _devret * -10; 
                _error++; 
            }
        }
    }

    

    return (_error == 0) ? 0 : 1;
}

uint8_t ew32_devicemanager_setup() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_PROBE) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_SETUP;

    uint8_t _devret = 0;
    uint16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_PROBE) {
            _devret = ew32_device_setup(__devman[i].device, __devman[i].cfg);
            if (_devret == 0)  __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_SETUP ;
            else { 
                __devman[i].probe_state = _devret * -10; 
                _error++; 
            }

            __devman[i].device->id = ew32_vfsdb_regist_device(__devman[i].device->subpath, 
                                                      __devman[i].device, 
                                                      _devret == EW32_DEVICE_NO_ERROR ? 1 : 0);
        }
    }
    return (_error == 0) ? 0 : 1;
}
uint8_t ew32_devicemanager_run() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_SETUP) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_RUNNING;


    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_SETUP) {

            ew32_device_set_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE, 5);
            __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING ;


        }
    }
    return 0;
}

void ew32_devicemanager_powerstate_set( uint8_t newState) {

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            ew32_device_set_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE, newState);
        }
    }
}

uint16_t ew32_devicemanager_get_running() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_RUNNING) return 0;

    uint16_t _ret = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++)  {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING)
            _ret++;
    }
    return _ret;
}


void ew32_devicemanager_dump(uint8_t level) {
    uint8_t _state = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == level) {
            //_state =  (ew32_device_get_state != NULL) ? ew32_device_get_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE) : 5;

            xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "EW32_DEVM", "Device %d: /dev/%s, Probe State: %d, Power State: %d\n", 
                   i, 
                   __devman[i].device->subpath, 
                   __devman[i].probe_state, 
                   5);
        }
    }
}
uint8_t ew32_devicemanager_setstate(const char* path, uint8_t state_id,uint8_t state) {
    ew32_dev_t* device = NULL;
    if(ew32_devicemanager_get_device(path, &device) != 0) {
        return 10;
    }
    return ew32_device_set_state(device, state_id, state);
}

uint8_t ewi32_devicemanager_setstate(ew32_devid_t id, uint8_t state_id, uint8_t state) {
    
   
    if(id == EW32_DEVICEMANAGER_ALL) {
        for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
            if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
                ew32_device_set_state(__devman[i].device, state_id, state);
            }
        }
    } else {
         if(id < 0 || id >= EW32_DEVICE_MANAGER_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;

         if(__devman[id].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            ew32_device_set_state(__devman[id].device, state_id, state);
        }
    }
    
}

uint8_t ew32_devicemanager_getstate(const char* path, uint8_t state_id) {
    ew32_dev_t* device = NULL;
    if(ew32_devicemanager_get_device(path, &device) != 0) {
        return 10;
    }
    return ew32_device_get_state(device, state_id);
}

uint8_t ewi32_devicemanager_getstate(ew32_devid_t id, uint8_t state_id) {
    if(id < 0 || id >= EW32_DEVICE_MANAGER_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;

    if(__devman[id].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
        return ew32_device_get_state(__devman[id].device, state_id);
    }
    return EW32_DEVICE_ERROR_NOT_FOUND;
}

#define EW32_IFBREAK(IF , VR,  RET) if(IF) { VR = RET; break; }

uint8_t ew32_devicemanager_get_device(const char* path, ew32_dev_t** device) {
    if (path == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;

    uint8_t _ret = EW32_DEVICE_ERROR_NOT_FOUND;
    const size_t prefix_len = strlen(EW32_VFS_DEV_PATH);
    char* _path = strdup(path); // Create a mutable copy of the path

    while(1) {
        EW32_IFBREAK(strncmp(_path, EW32_VFS_DEV_PATH, prefix_len) == 0, _ret, EW32_DEVICE_ERROR_INVALID_PATH);

        EW32_IFBREAK(strncmp(_path, "/dev", prefix_len) == 0, _ret, EW32_DEVICE_ERROR_INVALID_PATH);
        EW32_IFBREAK(strncmp(_path, "dev", prefix_len - 1) == 0, _ret, EW32_DEVICE_ERROR_INVALID_PATH);
        EW32_IFBREAK(_path[prefix_len] != EW32_VFS_DELIMETER, _ret, EW32_DEVICE_ERROR_INVALID_PATH);
    }

    // Remove /dev/
    char* c_path = _path + prefix_len + 1;

    // Iterate through registered devices
    for (uint32_t i = 0; i < EW32_DEVICE_MANAGER_DEVICES; i++)
    {
        if (strcmp(c_path, __devman[i].device->subpath) == 0)
        {
            *device = __devman[i].device;
            _ret = 0; // Found
        }
    }
    free(_path); // Free the allocated memory

    return _ret;
}


void ew32_devicemanager_loop(void* arg) {
    

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            if(__devman[i].device->on_run != NULL ) {
                __devman[i].device->on_run_r(__devman[i].device, 0);
            }
        }
    }
    ew32_task_delay(1);
}