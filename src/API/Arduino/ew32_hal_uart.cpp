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

#if EW32_PLATFORM == EW32_PLATFORM_ARDUINO ||  EW32_PLATFORM_RPPICO || EW32_PLATFORM_ARDUINO_ESP32
#include "ew32_devm/ew32_device.h"
#include "ew32_devm/API/ew32_hal_uart.h"
#include "ew32_devm/ew32_gpio.h"

#include "HardwareSerial.h"
#include "ew32_devm/API/ew32_hal_uart.h"

extern "C" {

ew32_error_t ew32_hal_uart_probe(ew32_dev_t* device, void* cfg) {

    return 0;
}

ew32_error_t ew32_hal_uart_setup(ew32_dev_t* device, void* cfg) {
  
    ew32_uart_cfg_t* config = (ew32_uart_cfg_t*)device->userdata;
    
    if(config->port_num == 0) {
        Serial.begin(config->baud, config->config, config->rxPin, config->txPin, config->invert);
    }
    else if(config->port_num == 1) {
        Serial1.begin(config->baud, config->config, config->rxPin, config->txPin,
                                            config->invert, config->timeout_ms, config->rxfifo_full_thrhd);
    }
    else if(config->port_num == 2) {
        Serial2.begin(config->baud, config->config, config->rxPin, config->txPin,
                                        config->invert, config->timeout_ms, config->rxfifo_full_thrhd);
    }
    else return 1;

    if(config->rxPin != -1)
    ew32_hal_gpio_marked_pin(config->rxPin, EW32_DEVICE_GPIO_OUTPUT, EW32_DEVICE_GPIO_USED_UART);

    if(config->txPin != -1)
    ew32_hal_gpio_marked_pin(config->txPin, EW32_DEVICE_GPIO_OUTPUT, EW32_DEVICE_GPIO_USED_UART);

    return 0;
}

ew32_ssize_t ew32_hal_uart_write(ew32_dev_t* device, const void* data, const ew32_size_t size) {
  
    ew32_uart_cfg_t* _cfg = (ew32_uart_cfg_t*)device->userdata;

    if(_cfg->port_num== 0) return Serial.write( (ew32_ui8_t*)data, size);
    if(_cfg->port_num== 1) return Serial1.write( (ew32_ui8_t*)data, size);
    if(_cfg->port_num == 2) return Serial2.write( (ew32_ui8_t*)data, size);

    return -1;
}
ew32_ssize_t ew32_hal_uart_read(ew32_dev_t* device, void* data, const ew32_size_t size) {
   
    ew32_uart_cfg_t* _cfg = (ew32_uart_cfg_t*)device->userdata;

    ew32_i32_t _used = ew32_hal_uart_avaible(_cfg->port_num);
    _used = _used > size ? size : _used;

    if(_used ) {
        if(_cfg->port_num == 0) return Serial.readBytes((ew32_ui8_t*) data, size);
        if(_cfg->port_num == 1) return Serial1.readBytes((ew32_ui8_t*) data, size);
        if(_cfg->port_num == 2) return Serial2.readBytes((ew32_ui8_t*) data, size);
    }
    return -1;
}


ew32_ui32_t ew32_hal_uart_avaible(ew32_ui8_t portnum) {

    if(portnum == 0) return Serial.available();
    if(portnum == 1) return Serial1.available();
    if(portnum == 2) return Serial2.available();
    return -1;
}

}
#endif