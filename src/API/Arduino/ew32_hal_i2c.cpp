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
#include "ew32_base/osa/ew32_osa_api.h"

#if EW32_PLATFORM == EW32_PLATFORM_ESP ||  EW32_PLATFORM_RPPICO || EW32_PLATFORM_ARDUINO_ESP32

#include "ew32_devm/ew32_i2c.h"
#include "ew32_devm/API/ew32_hal_i2c.h"

#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_gpio.h"
 

#include <stdio.h>

#include <Wire.h>

extern "C" {
uint8_t ew32_hal_i2c_isadress(ew32_dev_t* device, uint8_t address) {
     ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    return (handle->address_set[address]); 
}

uint8_t ew32_hal_i2c_probe(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    uint8_t error;
    
    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    #if EW32_TASKAPI_PLATFORM_SUB == 2  
        wire.setSDA(handle->pin_sda);
        wire.setSCL(handle->pin_scl);
        wire.begin();
    #elif EW32_TASKAPI_PLATFORM_SUB == 1 
        wire.begin(handle->pin_sda,handle->pin_scl);
    #endif

    for (uint8_t address  = 1; address < 127; address++) {
        wire.beginTransmission(address);
        error = wire.endTransmission();

        if (error == 0) {
            handle->address_set[address] = 1;
            xewLoggingInfo1(device->subpath, "Found device 0x%02X\n", address);
        }
        
    }

    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_hal_i2c_setup(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    if(ew32_hal_gpio_can_used_pin(handle->pin_sda) != EW32_ERROR_OK) return 1;
    if(ew32_hal_gpio_can_used_pin(handle->pin_scl) != EW32_ERROR_OK) return 1;

    ew32_hal_gpio_marked_pin(handle->pin_sda, OUTPUT, EW32_DEVICE_GPIO_USED_I2C);
    ew32_hal_gpio_marked_pin(handle->pin_scl, OUTPUT, EW32_DEVICE_GPIO_USED_I2C);

    #if EW32_TASKAPI_PLATFORM_SUB == 2 
        wire.setSDA(handle->pin_sda);
        wire.setSCL(handle->pin_scl);
        wire.begin();
    #elif EW32_TASKAPI_PLATFORM_SUB == 1 
        wire.begin(handle->pin_sda,handle->pin_scl);
    #endif

    return EW32_DEVICE_NO_ERROR;
}


int32_t ew32_hal_i2c_read(ew32_dev_t* device, void* data, const int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;

    

    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;

    if(handle->powerstate <= 2 || device->state == EW32_DEVICE_STATE_SUSPEND) return -1;

    int8_t _ret = 0;

    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    if(msg->message_id == EW32_I2C_RECIVE_MSG_ID) {

        wire.beginTransmission(msg->addr);
        wire.write((uint8_t*)msg->message, msg->msg_size);
         /// Errors:
    //  0 : Success
    //  1 : Data too long
    //  2 : NACK on transmit of address
    //  3 : NACK on transmit of data
    //  4 : Other error
    //  5 : Timeouts
        _ret = wire.endTransmission();
        if(_ret != 0) {
            _ret = -_ret; 
        } else {
            wire.requestFrom(msg->addr, msg->reg_size);
            int i = 0;
            while (wire.available()){
                msg->reg_addr[i] = wire.read(); 
                i = i + 1;
            }
            _ret = i;
        }
    }

    return _ret;
}

int32_t ew32_hal_i2c_write(ew32_dev_t* device, const void* data, const int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;


    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;

    if(handle->powerstate <= 2 || device->state == EW32_DEVICE_STATE_SUSPEND) return -1;

    int8_t _ret = 0;

    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    if(msg->message_id == EW32_I2C_NEWDEV_MSG_ID) {
        _ret = 0;
    } else if (msg->message_id == EW32_I2C_TRANSMIT_MSG_ID ) {

        wire.beginTransmission(msg->addr);
        wire.write(msg->message, msg->msg_size);
        _ret =  wire.endTransmission();

        if(_ret != 0) _ret = -_ret;

    }
    return _ret;
}


uint8_t ew32_hal_i2c_set_state(ew32_dev_t* device, uint8_t state_id, uint8_t state) {
    if (device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;

     ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    if (state_id == EW32_DEVICE_STATEID_MODE) {
        device->state = state;
    }

    if (state_id == EW32_DEVICE_STATEID_POWERSTATE) {
        handle->powerstate = state;
    }

    return 0;
}
uint8_t ew32_hal_i2c_get_state(ew32_dev_t* device, uint8_t state_id) {

    if (device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;

    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    if (state_id == EW32_DEVICE_STATEID_MODE) return device->state;
    if (state_id == EW32_DEVICE_STATEID_POWERSTATE) return handle->powerstate;

    return 0;
}
}

#endif