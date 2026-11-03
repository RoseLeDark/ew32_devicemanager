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

#define    EW32FILE_O_EXCLUSIVE         24 // for open int mode 

#include "ew32_device.h"

#define ew32_vfs_open_ex(CTX, PATH, FLAGS)  ew32_vfs_fopen( (CTX), (PATH), (FLAGS), EW32FILE_O_EXCLUSIVE)
#define ew32_vfs_open(CTX, PATH, FLAGS)     ew32_vfs_fopen( (CTX), (PATH), (FLAGS), 0)
#define ew32_vfs_read(CTX, BUFFER, SIZE)    ew32_vfs_fread( (CTX), (BUFFER), (SIZE) )
#define ew32_vfs_write(CTX, BUFFER, SIZE)   ew32_vfs_fwrite( (CTX), (BUFFER), (SIZE) )
#define ew32_vfs_seek(CTX, SIZE, MODE)      ew32_vfs_fseek( (CTX), (SIZE), (MODE) ) 
#define ew32_vfs_close(CTX)                 ew32_vfs_fclose( (CTX) )

#ifdef __cplusplus
extern "C" {
#endif

// only call from ew32_devm!!

/**
 * @brief Retrieves a device structure by its unique identifier.
 * 
 * @param id The unique identifier of the device.
 * @param device Double pointer to receive the device structure.
 * @return int8_t Returns 0 on success, or a negative error code if the device is not found.
 */
int8_t ew32_vfsdb_getbyid(int32_t id, ew32_dev_t** device);
/**
 * @brief Registers a device in the virtual file system database.
 * 
 * @param path The path to the device (e.g., "/dev/device_name").
 * @param device Pointer to the device structure to register.
 * @param state The initial state of the device (e.g., enabled or disabled).
 * @return int8_t Returns the unique identifier assigned to the registered device, or a negative error code on failure.
 */
int32_t ew32_vfsdb_regist_device(const char* path, ew32_dev_t* device, uint8_t state);

// Public api for VFS

/**
 * @brief Initializes the virtual file system (VFS) for device management.
 * 
 * This function sets up the necessary data structures and state for the VFS,
 * allowing devices to be registered, opened, read from, written to, and closed
 * using standard file-like operations.
 * 
 * @return uint8_t Returns 0 on success, or a non-zero error code on failure.
 */
uint8_t ew32_vfs_init();

/**
 * @brief Opens a device file in the virtual file system.
 * 
 * This function opens a device file in the virtual file system, allowing it to be read from, written to, and closed
 * using standard file-like operations.
 * 
 * @param ctx The file context structure.
 * @param path The path to the device file.
 * @param flags The flags for opening the file.
 * @param mode The mode for opening the file.
 * @return uint8_t Returns 0 on success, or a non-zero error code on failure.
 */
uint8_t ew32_vfs_fopen(ew32_file_t* ctx, const char * path, int flags, int mode) ;

/**
 * @brief Reads data from a device file in the virtual file system.
 * 
 * @param ctx The file context structure.
 * @param buffer The buffer to read data into.
 * @param size The number of bytes to read.
 * @return ssize_t The number of bytes read, or a negative error code on failure.
 */
ssize_t ew32_vfs_fread (ew32_file_t* ctx, void * buffer, size_t size);

/**
 * @brief Writes data to a device file in the virtual file system.
 * 
 * @param ctx The file context structure.
 * @param buffer The buffer containing data to write.
 * @param size The number of bytes to write.
 * @return ssize_t The number of bytes written, or a negative error code on failure.
 */
ssize_t ew32_vfs_fwrite (ew32_file_t* ctx, void * buffer, size_t size);
/**
 * @brief Seeks within a device file in the virtual file system.
 * 
 * @param ctx The file context structure.
 * @param size The number of bytes to seek.
 * @param mode The mode for seeking.
 * @return off_t The new file position, or a negative error code on failure.
 */
off_t ew32_vfs_fseek(ew32_file_t* ctx, off_t size, int mode);
/**
 * @brief Closes a device file in the virtual file system.
 * 
 * @param ctx The file context structure.
 * @return uint8_t Returns 0 on success, or a non-zero error code on failure.
 */
uint8_t ew32_vfs_fclose(ew32_file_t* ctx);



#ifdef __cplusplus
}
#endif