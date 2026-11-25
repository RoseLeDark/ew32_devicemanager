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

#if EW32_DEVICE_USE_UART == EW32_CONFIG_YES

#include "ew32_devm/ew32_uart.h"
#include <ew32_base/ew32_logging.h>
#include <stddef.h>
#include <string.h>
#include "ew32_base/osa/ew32_osa_api.h"
#include <stdlib.h>
#include "ew32_devm/ew32_devm.h"
#include "ew32_devm/API/ew32_hal_uart.h"

#define DEVICE __global_ew32_uart_device__






ew32_dev_t DEVICE[EW32_HAL_UART_HARDWARE_NUM];
ew32_uart_cfg_t DEVICE_state[EW32_HAL_UART_HARDWARE_NUM];

ew32_error_t ew32_device_uart_setup_ex(ew32_ui8_t port_num, ew32_ui64_t baud, ew32_ui32_t config, ew32_i8_t rx, ew32_i8_t tx) {
    if(port_num > EW32_HAL_UART_HARDWARE_NUM) return 1;
    if(DEVICE_state[port_num].used == 1) return 1;

    DEVICE_state[port_num].used = 1;
    DEVICE_state[port_num].port_num = port_num;
    DEVICE_state[port_num].baud = baud;
    DEVICE_state[port_num].config = config;
    DEVICE_state[port_num].rxPin = rx;
    DEVICE_state[port_num].txPin = tx;
    DEVICE_state[port_num].invert = ewFALSE;
    DEVICE_state[port_num].rxfifo_full_thrhd = 112;
    DEVICE_state[port_num].timeout_ms = 20000UL;
    

    // Instanz initialisieren
    DEVICE[port_num].subpath[0] = 'b';
    DEVICE[port_num].subpath[1] = 'u';
    DEVICE[port_num].subpath[2] = 's';
    DEVICE[port_num].subpath[3] = EW32_VFS_DELIMETER;
    DEVICE[port_num].subpath[4] = 'u';
    DEVICE[port_num].subpath[5] = 'a';
    DEVICE[port_num].subpath[6] = 'r';
    DEVICE[port_num].subpath[7] = 't';
    DEVICE[port_num].subpath[8] = port_num +'0';

    DEVICE[port_num].cls = ES32_DEVICE_CLASS_BUS;
    DEVICE[port_num].type = EW32_DEVICE_TYPE_UART;
    DEVICE[port_num].parent = NULL;
    DEVICE[port_num].rw =  EW32_DEVICE_READ_WRITE;
    DEVICE[port_num].set_state = 0;
    DEVICE[port_num].get_state = 0;
    DEVICE[port_num].probe = ew32_hal_uart_probe;
    DEVICE[port_num].setup = ew32_hal_uart_setup;
    DEVICE[port_num].write = ew32_hal_uart_write;
    DEVICE[port_num].read = ew32_hal_uart_read;
    DEVICE[port_num].on_run = NULL;
    DEVICE[port_num].userdata = &DEVICE_state[port_num];
    DEVICE[port_num].parent = NULL;

    return ew32_devicemanager_registdevice( &DEVICE[port_num], NULL, 1);

}

ew32_ssize_t ew32_device_uart_send(ew32_ui8_t port_num, const char* buf,const  ew32_size_t size) {
    return ew32_hal_uart_write(&DEVICE[port_num], buf, size);  
}
ew32_ssize_t ew32_device_uart_read(ew32_ui8_t port_num, char* buf, const ew32_size_t size) {
    return ew32_hal_uart_read(&DEVICE[port_num], buf, size);
}
ew32_i8_t ew32_device_uart_get(ew32_ui8_t port_num) {
    ew32_i8_t buf[1];

    if(ew32_hal_uart_avaible(port_num) > 0) {
        if(ew32_hal_uart_read(&DEVICE[port_num], (ew32_ui8_t*)buf, 1) >= 1)
            return buf[0];
    }

    return 0;
}
#endif