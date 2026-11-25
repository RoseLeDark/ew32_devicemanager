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

#include "ew32_devm/ew32_devicemanager.h"

#include <stdio.h>
#include <string.h>

#include "ew32_devm/ew32_i2c.h"
#include "ew32_devm/ew32_vfs.h"
#include "ew32_devm/ew32_power.h"
#include "ew32_devm/ew32_nulldev.h"
#include "ew32_devm/ew32_std.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_uart.h"

typedef struct ew32_devmanager_entry {
    ew32_dev_t* device;
    ew32_i8_t      probe_state; // = 0 (no probe), 1 (probe), 2 (init/run) - (-1= not used), -10 = error
    void*       cfg;
} ew32_devm_entry_t;  

ew32_devm_entry_t __devman[EW32_DEVICE_MANAGER_DEVICES];
ew32_ui8_t           __devm_global_state = 0;

ew32_ui8_t ew32_devicemanager_create(){
    if(__devm_global_state != 0) return 0;
     __devm_global_state = EW32_DEVICEMANAGER_STATE_INIT;
    
    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        __devman[i].device = NULL;
        __devman[i].probe_state = -1;
        __devman[i].cfg = NULL;
    }
 
#if EW32_DEVICE_USE_UART == 1
    ew32_device_uart_setup_ex( EW32_DEVICE_UART_STDPORT, EW32_DEVICE_UART_STDPORT_BAUD, 0x800001c /*SERIAL_8N1*/, 
                               EW32_DEVICE_UART_STDPORT_RX, EW32_DEVICE_UART_STDPORT_TX );
#endif

#if EW32_DEVICE_USE_I2C == 1
    ew32_device_i2c_setup(EW32_DEVICE_USE_I2C_PORT_SDA, EW32_DEVICE_USE_I2C_PORT_SCL, EW32_DEVICE_USE_I2C_PORT_NUM);
#endif

#if EW32_DEVICE_USE_SIMPLE_POWERMANAGER == 1
    ew32_device_powermanager_setup();
#endif

#if EW32_DEVICE_USE_VNULL == 1
    ew32_device_virtualnull_setup();
#endif

#if EW32_DEVICE_USE_STDDEVICE == 1

    ew32_dev_std_setup(EW32_STDOUT, NULL);
    ew32_dev_std_setup(EW32_STDERR, NULL);
    ew32_dev_std_setup(EW32_STDIN, NULL);
#endif

    return 0;
}

ew32_devid_t get_free_id() {
    ew32_i16_t _id = -1;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == -1) {
            __devman[i].probe_state = 0;
            _id = i;
            break;
        }
    }

    return _id;
} 

ew32_error_t ew32_devicemanager_registdevice(ew32_dev_t* device, void* cfg, ew32_bool_t preInit) {
    ew32_i16_t _id = get_free_id();
    ew32_ui8_t _ret = 0;

    if(_id == -1) return EW32_DEVICEMANAGER_FULL;

    xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "DEVM", "regist new device %s with id: %d\n", device->subpath, _id);
    __devman[_id].device = device;
    __devman[_id].cfg = cfg;
    __devman[_id].probe_state = 0;


    if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_PROBE || preInit == ewTRUE) {// allready_probe
        
        _ret = ew32_device_probe(__devman[_id].device, __devman[_id].cfg);

        if(_ret == EW32_DEVICE_NO_ERROR)   {
            __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_PROBE;
        }
        else {
#if EW32_DEVICE_USE_ERROR_VFS == EW32_YES
            __devman[_id].device->id = ew32_vfsdb_regist_device(__devman[_id].device->subpath, 
                                                      __devman[_id].device, 0);
#endif

            return _ret;
        } 
    }

    if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_SETUP || preInit == 1 ) {
        _ret = ew32_device_setup(__devman[_id].device, __devman[_id].cfg);
        
        if(_ret == EW32_DEVICE_NO_ERROR)   { 
            __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_SETUP;
        } else {
#if EW32_DEVICE_USE_ERROR_VFS == EW32_YES
            __devman[_id].device->id = ew32_vfsdb_regist_device(__devman[_id].device->subpath, 
                                                      __devman[_id].device, 0);
#endif
            return _ret;
        }
    }

    if( __devm_global_state >= EW32_DEVICEMANAGER_STATE_RUNNING || preInit == 1) {
        __devman[_id].device->id = ew32_vfsdb_regist_device(__devman[_id].device->subpath, 
                                                      __devman[_id].device, 1);

        __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING;
    }
    

    return _ret;
}

ew32_error_t ew32_devicemanager_probeall() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_INIT) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_PROBE; 

    ew32_error_t _devret = 0;
    ew32_ui16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].device == NULL) continue;

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

ew32_error_t ew32_devicemanager_setup() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_PROBE) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_SETUP;

    ew32_error_t _devret = 0;
    ew32_ui16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].device == NULL) continue;

        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_PROBE) {
            _devret = ew32_device_setup(__devman[i].device, __devman[i].cfg);
            if (_devret == 0) {
                 __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_SETUP ;

                 __devman[i].device->id = ew32_vfsdb_regist_device(__devman[i].device->subpath, 
                                                      __devman[i].device, 1);
            } else { 
                __devman[i].probe_state = _devret * -10; 
                _error++; 

#if EW32_DEVICE_USE_ERROR_VFS == EW32_YES
                __devman[i].device->id = ew32_vfsdb_regist_device(__devman[i].device->subpath, 
                                                      __devman[i].device, 0);
#endif 
            }
        }
    }
    return (_error == 0) ? 0 : 1;
}
ew32_error_t ew32_devicemanager_run() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_SETUP) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_RUNNING;


    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].device == NULL) continue;

        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_SETUP) {

            ew32_device_set_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE, 5);
            __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING ;


        }
    }
    return 0;
}

void ew32_devicemanager_powerstate_set( ew32_ui8_t newState) {

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].device == NULL) continue;

        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            ew32_device_set_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE, newState);
        }
    }
}

ew32_ui16_t ew32_devicemanager_get_running() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_RUNNING) return 0;

    ew32_ui16_t _ret = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++)  {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING)
            _ret++;
    }
    return _ret;
}


void ew32_devicemanager_dump(ew32_ui8_t level) {
    ew32_i8_t _state = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].device == NULL) continue;

        if(__devman[i].probe_state == level || level == EW32_DEVICEMANAGER_STATE_ALL) {
          _state =  ew32_device_get_state(__devman[i].device, EW32_DEVICE_STATEID_POWERSTATE);

            if(_state != -1) {
                xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "EW32_DEVM", "Device %d: /dev/%s, Probe State: %d, Power State: %d\n", 
                    i, 
                    __devman[i].device->subpath, 
                    __devman[i].probe_state, 
                    _state  );
            } else {
                xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, "EW32_DEVM", "Device %d: /dev/%s, Probe State: %d, Power State: not support\n", 
                    i, 
                    __devman[i].device->subpath, 
                    __devman[i].probe_state);
            }
        }
    }
}
ew32_error_t ew32_devicemanager_setstate(const char* path, ew32_ui8_t state_id, ew32_state_t state) {
    ew32_dev_t* device = NULL;
    if(ew32_devicemanager_get_device(path, &device) != 0) {
        return EW32_DEVICE_ERROR_NOT_FOUND;
    }
    if(device == NULL) return EW32_DEVICE_ERROR_NOT_FOUND;

    return ew32_device_set_state(device, state_id, state);
}

ew32_error_t ew32_devicemanager_setstate_by_id(ew32_devid_t id, ew32_ui8_t state_id, ew32_state_t state) {
    
   
    if(id == EW32_DEVICEMANAGER_ALL) {
        for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
            if(__devman[i].device == NULL) continue;
            
            if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
                ew32_device_set_state(__devman[i].device, state_id, state);
            }
        }
    } else {
         if(id < 0 || id >= EW32_DEVICE_MANAGER_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;

         if(__devman[id].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            return ew32_device_set_state(__devman[id].device, state_id, state);
        }
    }
    return 0;
}

ew32_state_t ew32_devicemanager_getstate(const char* path, ew32_ui8_t state_id) {
    ew32_dev_t* device = NULL;
    if(ew32_devicemanager_get_device(path, &device) != 0) {
        return 10;
    }

    if(device == NULL) return -EW32_DEVICE_ERROR_NOT_FOUND;

    return ew32_device_get_state(device, state_id);
}

ew32_state_t ew32_devicemanager_getstate_by_id(ew32_devid_t id, ew32_ui8_t state_id) {
    if(id < 0 || id >= EW32_DEVICE_MANAGER_DEVICES) return -EW32_DEVICE_ERROR_INVALID_ARG;

    if(__devman[id].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
        return ew32_device_get_state(__devman[id].device, state_id);
    }
    return -EW32_DEVICE_ERROR_NOT_FOUND;
}

#define EW32_IFBREAK(IF , VR,  RET) if(IF) { VR = RET; break; }

static const char* ew32_match_and_strip(const char* path, const char* prefix, size_t len) {
    if (path == NULL || prefix == NULL || len == 0) return NULL;
    if (strncmp(path, prefix, len) != 0) return NULL;
    if (path[len] != EW32_VFS_DELIMETER) return NULL;
    if (path[len + 1] == '\0') return NULL;

    return path + len + 1;
}

ew32_error_t ew32_devicemanager_get_device(const char* path, ew32_dev_t** device) {
    if (path == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;

    ew32_ui8_t _ret = EW32_DEVICE_ERROR_NOT_FOUND;
    const char* c_path = NULL;

    do {
        /* /dev/<subpath> mit Fallback bei fehlerhaft gesetztem EW32_VFS_DEV_PATH */
        c_path = ew32_match_and_strip(path, EW32_VFS_DEV_PATH, strlen(EW32_VFS_DEV_PATH));
        if (c_path == NULL) {
            c_path = ew32_match_and_strip(path, "/dev", 4);
        }
        if (c_path == NULL) {
            c_path = ew32_match_and_strip(path, "dev", 3);
        }

#if EW32_DEVICE_USE_ERROR_VFS == EW32_CONFIG_YES
        /* /sys/error/<subpath> mit Fallback bei fehlerhaft gesetztem EW32_VFS_SYS_PATH */
        if (c_path == NULL) {
            c_path = ew32_match_and_strip(path, EW32_VFS_SYS_PATH "/error", strlen(EW32_VFS_SYS_PATH "/error"));
            if (c_path == NULL) {
                c_path = ew32_match_and_strip(path, "/sys/error", 10);
            }
            if (c_path == NULL) {
                c_path = ew32_match_and_strip(path, "sys/error", 9);
            }
        }
#endif

        EW32_IFBREAK(c_path == NULL, _ret, EW32_DEVICE_ERROR_INVALID_PATH);

        for (ew32_ui32_t i = 0; i < EW32_DEVICE_MANAGER_DEVICES; i++) {
            if (__devman[i].device == NULL) continue;

            if (strcmp(c_path, __devman[i].device->subpath) == 0) {

                *device = __devman[i].device;
                _ret = EW32_DEVICE_NO_ERROR;
                break;
            }
        }
    } while (0);

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
    ew32_osa_delay(1);
}