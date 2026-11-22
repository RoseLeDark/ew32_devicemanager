#pragma once

#include <stddef.h>
#include <string.h>

#define EW32_DEVM_API_VERSION_100 1 // 1.00
#define EW32_DEVM_API_VERSION_110 0 // 1.10
#define EW32_DEVM_API_VERSION_150 0 // 1.50

#include "ew32_config.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_devm.h"
#include "ew32_devm/ew32_vfs.h"

#if EW32_DEVICE_USE_I2C == EW32_CONFIG_YES
#include "ew32_devm/ew32_i2c.h"
#endif

#if EW32_DEVICE_USE_UART == EW32_CONFIG_YES
#include "ew32_devm/ew32_uart.h"
#endif

#if EW32_DEVICE_USE_SIMPLE_POWERMANAGER == EW32_CONFIG_YES
#include "ew32_devm/ew32_power.h"
#endif

#if EW32_DEVICE_USE_VNULL == EW32_CONFIG_YES
#include "ew32_devm/ew32_nulldev.h"
#endif

#if EW32_DEVICE_USE_STDDEVICE == EW32_CONFIG_YES
#include "ew32_devm/ew32_std.h"
#endif




