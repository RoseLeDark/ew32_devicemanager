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
#include "ew32_devm/API/ew32_hal_gpio.h"
#include "ew32_base/osa/ew32_osa_api.h"
#include "ew32_base/ew32_version.h"

ew32_error_t ew32_init_framework(logging_handler handle, ew32_ui8_t minLevel, ew32_ui8_t maxLevel) {
    
    xewLoggingInitEx(handle, minLevel, maxLevel);

    if(ew32_devicemanager_create() != 0) {
        xewLoggingDebug("ST0", "Device Manager Create ... [Error]\n");
    }  else {
        xewLoggingDebug("ST0", "Device Manager Create ... [OK]\n");
    }

#if EW32_CONFIG_ENABLE_GPIO_ISR == EW32_CONFIG_YES
    if(ew32_hal_gpio_init() != 0) {
        xewLoggingDebug("ST0", "Initializing gpio and create isr state ... [Error]\n");
    } else {
        xewLoggingDebug("ST0", "Initializing gpio and create isr state ... [OK]\n");
    }
#else
    xewLoggingDebug("ST0", "Initializing gpio ... [OK]\n");
#endif

    if(ew32_osa_task_init() != 0) {
        xewLoggingDebug("ST0", "Initializing tasksystem ... [Error]\n");
    } else {
         xewLoggingDebug("ST0", "Initializing tasksystem ... [OK]\n");
    }

    if(ew32_vfs_init() != 0) {
        xewLoggingDebug("ST0", "Initializing VFS ... [Error]\n");
    } else {
         xewLoggingDebug("ST0", "Initializing VFS ... [OK]\n");
    }

    xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "ST0", "Using ew32_base %s and ew32_devm %s \n\n", EW32_BASE_VERSION_STRING, EW32_DEVM_VERSION_STRING);
    xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "ST1", "Enter to stage 1 ...\n");

    return 0;
}