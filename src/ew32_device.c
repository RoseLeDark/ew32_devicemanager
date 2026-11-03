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

#include "ew32_devm/ew32_device.h"
#include "string.h"

#include <ew32_base/ew32_logging.h>
#include "ew32_devm/ew32_api.h"

int32_t ew32_device_send(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -EW32_DEVICE_ERROR_NULL;

    ew32_dev_t *_used_device = (device->parent != NULL) ? device->parent : device;

    if(_used_device->write == NULL) return -EW32_DEVICE_ERROR_NOTIMPL;
    if(ew32_device_can_send(_used_device) == EW32_CONFIG_NO) return -EW32_DEVICE_ERROR_NOT_SUPPORTED;


    
    int32_t datasended = 0;
    uint8_t i = 0;

    while(1) {
        if( i >= 3) break;

        datasended = _used_device->write(_used_device, data, size);
        if(datasended >= 0) break;

        i++;

        ew32_task_delay(500);
    };

    return datasended;
}
int32_t ew32_device_read(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -EW32_DEVICE_ERROR_NULL;

    ew32_dev_t *_used_device = (device->parent != NULL) ? device->parent : device;
    
    if(_used_device->read == NULL) return -EW32_DEVICE_ERROR_NOTIMPL;
    if(ew32_device_can_read(_used_device) == EW32_CONFIG_NO) return -EW32_DEVICE_ERROR_NOT_SUPPORTED;

    int32_t dataread = 0;
    uint8_t i = 0;

    while(1) {
        if( i >= 3) break;

        dataread = _used_device->read(_used_device, data, size);
        if(dataread >= 0) break;

        i++;
        ew32_task_delay(500);
    };

    return dataread;
}
uint8_t ew32_device_pstate(ew32_dev_t* device, uint8_t state) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->set_state == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    uint8_t i = 0;
    

    while(1) {
        if( i >= 3) break;
        
        _ret = device->state = device->set_state(device, EW32_DEVICE_STATEID_POWERSTATE, state);

        if(_ret == EW32_DEVICE_ERROR_WAIT) {
            i++;
            ew32_task_delay(500);
            continue;
        } 
        break;
    }

    return _ret;
}
uint8_t ew32_device_probe(ew32_dev_t* device, void* cfg) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->probe == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    
    _ret = device->probe(device, cfg);

    return _ret;

}
uint8_t ew32_device_setup(ew32_dev_t* device, void* cfg) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->setup == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    
    _ret = device->setup(device, cfg);

    return _ret;
}
uint8_t ew32_device_set_userdata(ew32_dev_t* device, void* data) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    device->userdata = data;

    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_device_get_userdata(ew32_dev_t* device, void** data) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    uint8_t _ret = EW32_DEVICE_NO_ERROR;

    if(data != NULL) *data = device->userdata;

    return _ret;
}

uint8_t ew32_device_can_send(ew32_dev_t* device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    return (device->rw == EW32_DEVICE_WRITE_ONLY || device->rw == EW32_DEVICE_READ_WRITE) ? 
        EW32_CONFIG_YES : EW32_CONFIG_NO;
}


uint8_t ew32_device_can_read(ew32_dev_t* device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    return (device->rw == EW32_DEVICE_READ_ONLY || device->rw == EW32_DEVICE_READ_WRITE) ? 
        EW32_CONFIG_YES : EW32_CONFIG_NO;
}


uint8_t ew32_device_get_parent(ew32_dev_t* device, ew32_dev_t** parent) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(parent != NULL) *parent = device->parent;

    return  (device->parent != NULL ? EW32_CONFIG_YES : EW32_CONFIG_NO);
}
uint8_t ew32_device_set_parent(ew32_dev_t* device, ew32_dev_t* parent) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    device->parent = parent;


    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_device_get_state(ew32_dev_t* device, uint8_t state_id) {
    if(device == NULL) return -EW32_DEVICE_ERROR_NULL;
    return (device->get_state != NULL) ? device->get_state(device, state_id) : 10;

}

uint8_t ew32_device_set_state(ew32_dev_t* device, uint8_t state_id, uint8_t state) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    
    if(device->set_state != NULL)
        device->set_state(device, state_id, state);

    return EW32_DEVICE_NO_ERROR;
}