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
#include "ew32_devm/ew32_i2c.h"

#include "ew32_base/osa/ew32_osa_api.h"
#include "ew32_devm/ew32_devm.h"
#include "ew32_devm/API/ew32_hal_i2c.h"
#include "string.h"

#if EW32_DEVICE_USE_I2C == EW32_CONFIG_YES

static ew32_dev_t __i2c_devices[EW32_DEVICE_I2C_MAX_DEVICES];     
static ew32_i2c_cfg_t __global_cfg[EW32_DEVICE_I2C_MAX_DEVICES];


ew32_error_t ew32_device_i2c_setup(ew32_ui8_t sda, ew32_ui8_t scl, ew32_ui8_t port_num) {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if( __global_cfg[port_num].used == 1) return EW32_DEVICE_ERROR_ALLREADY_INIT;


    
    __global_cfg[port_num].used = 1;
    __global_cfg[port_num].pin_scl = scl;
    __global_cfg[port_num].pin_sda = sda;
    __global_cfg[port_num].port_num = port_num;
    __global_cfg[port_num].hardware = port_num > EW32_HAL_I2C_HARDWARE_NUM; // port 0 und 1 used hardware i2c
    __global_cfg[port_num].powerstate = 5;
    
    __i2c_devices[port_num].subpath[0] = 'b';
    __i2c_devices[port_num].subpath[1] = 'u';
    __i2c_devices[port_num].subpath[2] = 's';
    __i2c_devices[port_num].subpath[3] = EW32_VFS_DELIMETER;
    __i2c_devices[port_num].subpath[4] = 'i';
    __i2c_devices[port_num].subpath[5] = '2';
    __i2c_devices[port_num].subpath[6] = 'c';
    __i2c_devices[port_num].subpath[7] = port_num +'0';


   // Instanz initialisieren
    __i2c_devices[port_num].cls = ES32_DEVICE_CLASS_BUS;
    __i2c_devices[port_num].type = EW32_DEVICE_TYPE_I2C;
    __i2c_devices[port_num].parent = NULL;
    __i2c_devices[port_num].rw =  EW32_DEVICE_READ_WRITE;
    __i2c_devices[port_num].set_state = ew32_hal_i2c_set_state;
    __i2c_devices[port_num].get_state = ew32_hal_i2c_get_state;
    __i2c_devices[port_num].probe = ew32_hal_i2c_probe;
    __i2c_devices[port_num].setup = ew32_hal_i2c_setup;
    __i2c_devices[port_num].write = ew32_hal_i2c_write;
    __i2c_devices[port_num].read = ew32_hal_i2c_read;
    __i2c_devices[port_num].on_run = NULL;

    __i2c_devices[port_num].userdata = &__global_cfg[port_num];


    ew32_devicemanager_registdevice(&__i2c_devices[port_num], NULL, ewTRUE);

    return EW32_DEVICE_NO_ERROR;
}
ew32_bool_t ew32_device_i2c_isadress(ew32_ui8_t port_num, ew32_ui8_t address)  {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return ewFALSE;
    if( __global_cfg[port_num].used != 1) return ewFALSE; // not in use


    return ew32_hal_i2c_isadress(&__i2c_devices[port_num], address);
}


ew32_ssize_t ew32_device_i2c_read(uint8_t port_num,  ew32_i2c_io_t* data, ew32_size_t size) {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if( __global_cfg[port_num].used != 1) return EW32_DEVICE_ERROR_NOT_USED; // not in use

    return ew32_hal_i2c_read(&__i2c_devices[port_num], data, size);
}

ew32_ssize_t ew32_device_i2c_write(uint8_t port_num,  ew32_i2c_io_t* data, ew32_size_t size) {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if( __global_cfg[port_num].used != 1) return EW32_DEVICE_ERROR_NOT_USED; // not in use

    return ew32_hal_i2c_write(&__i2c_devices[port_num], data, size);
}

#endif
