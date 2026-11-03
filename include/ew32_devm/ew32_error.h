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


/** @name Error and Status Codes */
/// @{
#define EW32_DEVICE_NO_ERROR            0    /**< Operation completed successfully with no errors. */
#define EW32_DEVICE_ERROR_NULL          1    /**< Error: Null pointer encountered or invalid argument. */
#define EW32_DEVICE_ERROR_NOTIMPL       2    /**< Error: Requested operation or function is not implemented. */
#define EW32_DEVICE_ERROR_NOT_SUPPORTED 3    /**< Error: Operation or feature is not supported by this device. */
#define EW32_DEVICE_ERROR_WAIT          4    /**< Error/State: Operation timed out or requires waiting. */
#define EW32_DEVICE_ERROR_BUSY          5    /**< Error/State: Device or resource is currently busy. */
#define EW32_DEVICE_ERROR_INVALID_ARG   6   /**< Error: Invalid argument or parameter provided to the function. */
#define EW32_DEVICE_ERROR_NOT_FOUND     7   /**< Error:  was not found. */
#define EW32_DEVICE_ERROR_NOT_USED      8   /**< Error/State: not used */
#define EW32_DEVICE_ERROR_ALLREADY_INIT 9   /**< Error: Device is already initialized or in use. */
#define EW32_DEVICE_ERROR_INVALID_PATH 10   /**< Error: Invalid path provided. */

#define EW32_DEVICEMANAGER_FULL         1
/// @}