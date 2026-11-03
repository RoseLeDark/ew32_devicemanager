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

#include "ew32_error.h"
#include "ew32_config.h"
#include "stdint.h"

/** @name Device Classes */
/// @{
#define ES32_DEVICE_CLASS_VIRTUAL   0x00  /**< Virtual or software-emulated device class. */
#define ES32_DEVICE_CLASS_BUS       0x01  /**< Bus controller or communication interface device class. */
#define ES32_DEVICE_CLASS_SECONDARY 0x02  /**< Secondary or peripheral device class. */
#define ES32_DEVICE_CLASS_NETWORK   0x07 /**< Network interface or communication device class. */
#define ES32_DEVICE_CLASS_PROTOCOL  0x08 /**< Protocol handler or communication protocol device class. */
#define ES32_DEVICE_CLASS_GPIO      0x09 /**< General-purpose input/output device class. */
#define ES32_DEVICE_CLASS_SENSOR    0x0A /**< Sensor or measurement device class.

/// @}

/** @name Device Types */
/// @{
/** @name Device Types */
/// @{
#define EW32_DEVICE_TYPE_UNKNOWN                 0x0000 / **< Unknown or unspecified device type. */
#define EW32_DEVICE_TYPE_I2C                     0x0001 /**< I2C bus controller or device type. */
#define EW32_DEVICE_TYPE_SPI                     0x0002 /**< SPI bus controller or device type. */
#define EW32_DEVICE_TYPE_UART                    0x0003 /**< UART serial communication device type. */
#define EW32_DEVICE_TYPE_CONTROLLER              0x0004 /**< Generic controller or management device type. */
#define EW32_DEVICE_TYPE_DISK                    0x0005 /**< Disk storage or block device type. */
#define EW32_DEVICE_TYPE_INPORT_PORT             0x0006 /**< Input port or peripheral device type. */
#define EW32_DEVICE_TYPE_KEYBOARD                0x0007 /**< Keyboard input device type. */
#define EW32_DEVICE_TYPE_MIDI_IN                 0x0008 /**< MIDI input device type. */
#define EW32_DEVICE_TYPE_MIDI_OUT                0x0009 /**< MIDI output device type. */
#define EW32_DEVICE_TYPE_MOUSE                   0x000A /**< Mouse input device type. */
#define EW32_DEVICE_TYPE_ETH                     0x000B /**< Ethernet network interface device type. */
#define EW32_DEVICE_TYPE_NULL                    0x000C /**< Null or placeholder device type. */
#define EW32_DEVICE_TYPE_PARALLELPORT            0x000D /**< Parallel port or printer interface device type. */
#define EW32_DEVICE_TYPE_WIFI                    0x000E /**< Wi-Fi network interface device type. */
#define EW32_DEVICE_TYPE_PRINTER                 0x000F /**< Printer output device type. */
#define EW32_DEVICE_TYPE_SCANNER                 0x0010 /**< Scanner input device type. */
#define EW32_DEVICE_TYPE_SERIALPORT              0x0011 /**< Serial port or RS-232 interface device type. */
#define EW32_DEVICE_TYPE_SCREEN                  0x0012 /**< Screen or display output device type. */
#define EW32_DEVICE_TYPE_SOUND                   0x0013 /**< Sound or audio output device type. */
#define EW32_DEVICE_TYPE_STREAM                  0x0014 /**< Stream device type. */
#define EW32_DEVICE_TYPE_TAPE                    0x0015 /**< Tape storage device type. */
#define EW32_DEVICE_TYPE_TRANSPORT               0x0016 /**< Transport device type. */
#define EW32_DEVICE_TYPE_VIDEO                   0x0017 /**< Video output device type. */
#define EW32_DEVICE_TYPE_AUDIOOUT                0x0018 /**< Audio output device type. */
#define EW32_DEVICE_TYPE_AUDIOIN                 0x0019 /**< Audio input device type. */
#define EW32_DEVICE_TYPE_I2S                     0x001A /**< I2S audio interface device type. */
#define EW32_DEVICE_TYPE_ONE_WIRE                0x001B /**< One-Wire bus interface device type. */
#define EW32_DEVICE_TYPE_BATTERY                 0x001C /**< Battery management or monitoring device type. */
#define EW32_DEVICE_TYPE_BUS_EXTENDER            0x001D /**< Bus extender or bridge device type. */
#define EW32_DEVICE_TYPE_MODEM                   0x001E /**< Modem or communication device type. */
#define EW32_DEVICE_TYPE_RS485                   0x001F /**< RS-485 serial communication device type. */
#define EW32_DEVICE_TYPE_STORAGE                 0x0020 /**< Storage device type. */
#define EW32_DEVICE_TYPE_SMARTCARD               0x0021 /**< Smartcard reader or interface device type. */
#define EW32_DEVICE_TYPE_CRYPT                   0x0022 /**< Cryptographic or security device type. */
#define EW32_DEVICE_TYPE_BLUETOOTH               0x0023 /**< Bluetooth communication device type. */
#define EW32_DEVICE_TYPE_BIOMETRIC               0x0024 /**< Biometric sensor or authentication device type. */
#define EW32_DEVICE_TYPE_DEVAPI                  0x0025 /**< Device API or driver interface device type. */
#define EW32_DEVICE_TYPE_CONSOLE                 0x0026 /**< Console or terminal interface device type. */
#define EW32_DEVICE_TYPE_SYSENV                  0x0027 /**< System environment or configuration device type. */
#define EW32_DEVICE_TYPE_VBLOCK                  0x0028 /**< Virtual block device type. */
#define EW32_DEVICE_TYPE_STORAGE_REPLICATION     0x0029 /**< Storage replication or backup device type. */
#define EW32_DEVICE_TYPE_TRUSTENV                0x002A /**< Trusted environment or secure enclave device type. */
#define EW32_DEVICE_TYPE_PMEM                    0x002B /**< Persistent memory or non-volatile memory device type. */
#define EW32_DEVICE_TYPE_HARDWARE_ACCELERATOR    0x002C /**< Hardware accelerator or co-processor device type. */


/* --- Embedded & Robotik Sensoren / Aktoren --- */
#define EW32_DEVICE_TYPE_GPS                     0x002D /**< GPS sensor or navigation device type. */
#define EW32_DEVICE_TYPE_LIGHT_SENSOR            0x002E /**< Light sensor or ambient light measurement device type. */
#define EW32_DEVICE_TYPE_TEMP_SENSOR             0x002F /**< Temperature sensor or thermal measurement device type. */
#define EW32_DEVICE_TYPE_HUMIDITY_SENSOR         0x0030 /**< Humidity sensor or environmental measurement device type. */
#define EW32_DEVICE_TYPE_PRESSURE_SENSOR         0x0031 /**< Pressure sensor or barometric measurement device type. */
#define EW32_DEVICE_TYPE_IMU                     0x0032 /**< Inertial Measurement Unit (IMU) sensor or motion tracking device type. */
#define EW32_DEVICE_TYPE_DISTANCE_SENSOR         0x0033 /**< Distance sensor or proximity measurement device type. */
#define EW32_DEVICE_TYPE_GAS_SENSOR              0x0034 /**< Gas sensor or air quality measurement device type. */
#define EW32_DEVICE_TYPE_MOTOR                   0x0035 /**< Motor actuator or motion control device type. */
#define EW32_DEVICE_TYPE_SERVO                   0x0036 /**< Servo actuator or position control device type. */
#define EW32_DEVICE_TYPE_RELAY                   0x0037 /**< Relay actuator or switching device type. */

#define EW32_DEVICE_TYPE_POWERMANAGER            0x0100 /**< Power management or power state control device type. */
 
#define EW32_DEVICE_TYPE_USER                    0x1000 /**< User-defined or custom device type. */
#define EW32_DEVICE_TYPE_MAX                     0x1FFF /**< Maximum valid device type value. */
/// @}


#define EW32_DEVICE_READ_ONLY                    0x01 /**< Device is read-only; write operations are not permitted. */
#define EW32_DEVICE_WRITE_ONLY                   0x10 /**< Device is write-only; read operations are not permitted. */
#define EW32_DEVICE_READ_WRITE                   0x11 /**< Device supports both read and write operations. */

#define EW32_DEVICE_STATE_DESTROY                   0x00 /**< Device is being destroyed or deinitialized. */
#define EW32_DEVICE_STATE_SUSPEND                   0x01 /**< Device is suspended; operations may be temporarily halted. */
#define EW32_DEVICE_STATE_RESUME                    0x02 /**< Device is resumed; operations may continue after suspension. */

#define EW32_DEVICE_STATEID_POWERSTATE              1
#define EW32_DEVICE_STATEID_MODE                    2 
#define EW32_DEVICE_STATEID_USER                    3

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Core device structure representing any hardware node or virtual entity.
 */
typedef struct EW32_PACKED ew32device  {
    char    subpath[16];                                                 /**< Subpath or identifier for the device within the system. */
    ew32_file_t id;                                                     /**< Unique instance identifier or slot index. */
    uint16_t cls;                                                    /**< Device classification (e.g., system, external, virtual). */
    uint16_t type;                                                   /**< Device operational type (input, output, inout). */
    uint8_t  rw;                                               /**< Read write status: 0 => read-only, 1 => write-only, 2 => read-write */
    uint8_t  state;                                               /**< Current operational state of the device (e.g., suspended, resumed). */
    
    struct ew32device* parent;                                      /**< Pointer to the parent device node (e.g., bus controller). */
    

    uint8_t (* probe)(struct ew32device* device, void* cfg);         /**< Function pointer for discovering hardware capabilities. */
    uint8_t (* setup)(struct ew32device* device, void* cfg);         /**< Function pointer for initializing the device. */
    int32_t (* write)(struct ew32device* device, void* data, int32_t size);   /**< Function pointer for writing data or control messages. */
    int32_t (* read)(struct ew32device* device, void* data, int32_t size);    /**< Function pointer for reading data from the device. */

    uint8_t (* get_state)(struct ew32device* device, uint8_t state_id);
    uint8_t (* set_state)(struct ew32device* device, uint8_t state_id, uint8_t state);

    union {
        void (* on_run)(struct ew32device* device);                       /**< Function pointer for handling periodic tasks or events. */
        void (* on_run_r)(struct ew32device* device, uint64_t millis);  
    };
    union  {
        void*       userdata;                                                 /**< Pointer to private driver-specific configuration or state data. */
        uint32_t    raw_userdata;                                             /**< Raw representation of the user data pointer for low-level access. */
    };
    
    
} ew32_dev_t ;


/**
 * @brief Sends data or a control command to a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the payload buffer or message structure.
 * @param size Size of the payload data in bytes.
 * @return int32_t Number of bytes written, or a negative error code on failure.
 */
int32_t ew32_device_send(ew32_dev_t* device, void* data, int32_t size);

/**
 * @brief Reads data from a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the destination buffer.
 * @param size Maximum number of bytes to read.
 * @return int32_t Number of bytes read, or a negative error code on failure.
 */
int32_t ew32_device_read(ew32_dev_t* device, void* data, int32_t size);

/**
 * @brief Sets or queries the power state of a device. Obsolet please use ew32_device_set_state(device, EW32_DEVICE_STATEID_POWERSTATE, state)
 * 
 * @param device Pointer to the target device node.
 * @param state Target power state value.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_pstate(ew32_dev_t* device, uint8_t state);

/**
 * @brief Executes the probe routine on a device to detect underlying hardware.
 * 
 * @param device Pointer to the target device node.
 * @param cfg Optional configuration pointer passed to the probe function.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_probe(ew32_dev_t* device, void* cfg);

/**
 * @brief Executes the setup routine to initialize a device for operation.
 * 
 * @param device Pointer to the target device node.
 * @param cfg Optional configuration pointer passed to the setup function.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_setup(ew32_dev_t* device, void* cfg);

/**
 * @brief Assigns private user data or driver context to a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the context structure to attach.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success.
 */
uint8_t ew32_device_set_userdata(ew32_dev_t* device, void* data);

/**
 * @brief Retrieves the private user data or driver context from a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Double pointer to receive the stored context pointer.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code if null.
 */
uint8_t ew32_device_get_userdata(ew32_dev_t* device, void** data);

/**
 * @brief Checks whether a device is currently capable of sending/writing data.
 * 
 * @param device Pointer to the target device node.
 * @return uint8_t EW32_DEVICE_YES if capable, otherwise EW32_DEVICE_NO.
 */
uint8_t ew32_device_can_send(ew32_dev_t* device);

/**
 * @brief Checks whether a device is currently capable of receiving/reading data.
 * 
 * @param device Pointer to the target device node.
 * @return uint8_t EW32_DEVICE_YES if capable, otherwise EW32_DEVICE_NO.
 */
uint8_t ew32_device_can_read(ew32_dev_t* device);

/**
 * @brief Gets the parent device node associated with this device.
 * 
 * @param device Pointer to the target device node.
 * @param parent Double pointer to receive the parent device reference.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_get_parent(ew32_dev_t* device, ew32_dev_t** parent);

/**
 * @brief Sets or updates the parent device node for this device.
 * 
 * @param device Pointer to the target device node.
 * @param parent Pointer to the new parent device node.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success.
 */
uint8_t ew32_device_set_parent(ew32_dev_t* device, ew32_dev_t* parent);

/**
 * @brief Retrieves the current operational state of a device (e.g., suspended, resumed).
 * 
 * @param device Pointer to the target device node.
 * @return uint8_t The current operational state.
 */
uint8_t ew32_device_get_state(ew32_dev_t* device, uint8_t state_id);
/**
 * @brief Sets the operational state of a device (e.g., suspend, resume).
 * 
 * @param device Pointer to the target device node.
 * @param state The desired operational state.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success.
 */
uint8_t ew32_device_set_state(ew32_dev_t* device, uint8_t state_id, uint8_t state);

#ifdef __cplusplus
}
#endif