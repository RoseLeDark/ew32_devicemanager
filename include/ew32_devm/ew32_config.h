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

 #pragma once

#include <ew32_base/ew32_config.h>

/** @name Device Manager Allocation Modes */
/// @{
#define EW32_STATIC                             0    /**< Static allocation mode using fixed compile-time arrays. */
#define EW32_DYNAMIC                            1    /**< Dynamic allocation mode (if supported). */
/// @}


/// ================        CONFIG START        ================

/** @brief Maximum number of supported devices on the I2C bus. */
#ifndef EW32_DEVICE_I2C_MAX_DEVICES
    #define EW32_DEVICE_I2C_MAX_DEVICES         2
#endif

/** @brief Enable or disable I2C bus subsystem support. */
#ifndef EW32_DEVICE_USE_I2C   
    #define EW32_DEVICE_USE_I2C                 EW32_CONFIG_YES
#endif   

/** @brief Enable or disable SPI bus subsystem support. */
#ifndef EW32_DEVICE_USE_SPI   
    #define EW32_DEVICE_USE_SPI                 EW32_CONFIG_YES
#endif  

/** @brief Enable or disable I2S audio bus subsystem support. */
#ifndef EW32_DEVICE_USE_I2S   
    #define EW32_DEVICE_USE_I2S                 EW32_CONFIG_NO
#endif  

/** @brief Enable or disable GPIO pin subsystem support. */
#ifndef EW32_DEVICE_USE_PIN   
    #define EW32_DEVICE_USE_PIN                 EW32_CONFIG_YES
#endif  

/** @brief Enable or disable Bluetooth subsystem support. */
#ifndef EW32_DEVICE_USE_BT  
    #define EW32_DEVICE_USE_BT                  EW32_CONFIG_YES
#endif 

/** @brief Enable or disable Wi-Fi network subsystem support. */
#ifndef EW32_DEVICE_USE_WIFI 
    #define EW32_DEVICE_USE_WIFI                EW32_CONFIG_NO
#endif  

/** @brief Enable or disable GPS sensor subsystem support. */
#ifndef EW32_DEVICE_USE_GPS
    #define EW32_DEVICE_USE_GPS                 EW32_CONFIG_YES
#endif  

#if EW32_DEVICE_USE_GPS == EW32_CONFIG_YES
    /** @brief Enable periodic system time synchronization from GPS data. */
    #ifndef EW32_DEVICE_USE_GPS_CLOCK
        #define EW32_DEVICE_USE_GPS_CLOCK       EW32_CONFIG_YES
    #endif 
#endif

#if EW32_DEVICE_USE_I2C == EW32_CONFIG_YES
    /** @brief Enable automatic bus probing during I2C initialization. */
    #ifndef EW32_DEVICE_USE_I2C_AUTOPROBE
        #define EW32_DEVICE_USE_I2C_AUTOPROBE   EW32_CONFIG_YES
    #endif 
#endif

#if EW32_DEVICE_MANAGER_MODE == EW32_STATIC
    /** @brief Maximum total capacity of statically allocated device entries. */
    #ifndef EW32_DEVICE_MANAGER_DEVICES
        #define EW32_DEVICE_MANAGER_DEVICES     32
    #endif
#endif

#ifndef EW32_MAX_OPEN_FILES
#define EW32_MAX_OPEN_FILES                     16
#endif

#ifndef EW32_VFS_DEV_PATH       
#define EW32_VFS_DEV_PATH                       "/dev"
#endif

#ifndef EW32_VFS_SYS_PATH                 
#define EW32_VFS_SYS_PATH                       "/sys"
#endif

#ifndef EW32_VFS_DELIMETER               
#define EW32_VFS_DELIMETER                       '/'
#endif


#if EW32_MAX_OPEN_FILES >= __INT16_MAX__
    typedef int32_t ew32_file_t;
    typedef int32_t ew32_devid_t;
#elif EW32_MAX_OPEN_FILES >= __INT8_MAX__     
    typedef int16_t ew32_file_t;
    typedef int16_t ew32_devid_t;
#else
   typedef int8_t ew32_file_t;
   typedef int8_t ew32_devid_t;
#endif



