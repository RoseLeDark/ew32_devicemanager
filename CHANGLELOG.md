# Changelog

# Version 0.3.0
## Changed
- Rename ewi32_devicemanager_setstate to ew32_devicemanager_setstate_by_id
- Rename ewi32_devicemanager_getstate to ew32_devicemanager_getstate_by_id 
- Changed `ew32_devicemanager_get_device` now support /sys/error/ path when `EW32_DEVICE_USE_ERROR_VFS` on is
- Changed setup API for all devices - setup functions regist device it self, not use ew32_dev_t**  
- changed `dependencies` in libary.json `pba3h11aso/ew32_base`: from  `0.3.0` to `0.3.4`
## Added
- Added config `EW32_DEVICE_USE_ERROR_VFS` with `EW32_CONFIG_NO`: when enable then all dérror devices can find in vfs on /sys/error/{sub_path}
- Add `ew32_uart_device` and add configs for uart:
    - `EW32_DEVICE_UART_STDPORT` std is 0  
    - `EW32_DEVICE_UART_STDPORT_RX`  std is -1
    - `EW32_DEVICE_UART_STDPORT_TX` std is -1
    - `EW32_DEVICE_UART_STDPORT_BAUD` std is 115200
    - `EW32_DEVICE_USE_UART` with `EW32_CONFIG_YES` - devmanager use UART 
- Add new configs for i2c:
    - `EW32_DEVICE_USE_I2C_PORT_SDA` std is 21
    - `EW32_DEVICE_USE_I2C_PORT_SCL` std is 22
    - `EW32_DEVICE_USE_I2C_PORT_NUM` std is 0

- With `EW32_DEVICE_USE_SIMPLE_POWERMANAGER` - is  /dev/bus/pmn0 used and avaible in devmanager
- With `EW32_DEVICE_USE_VNULL` - is  /dev/null used and avaible in devmanager
- With `EW32_DEVICE_USE_STDDEVICE` - is /dev/std{out, in, err } used and avaible in devmanager

---
# Version 0.1.8
## Added
start driver `ew32_gpio` to impl  `/dev/bus/gpio` with pin map an dpin cheak it given pin valid/usable is
add in ew32_i2c device `ew32_gpio` pin cheak and pin marked api

## Changed 
changed `dependencies` in libary.json `pba3h11aso/ew32_base`: from  `0.3.0` to `0.3.2`

---
# Version 0.1.4

## Changed
change ew32_task api is now in ew32_base libary 
the poewermanager is now running - see example for details to impl powermanager in your project