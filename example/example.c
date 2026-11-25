

/* Ausgabe with buildflag: -DEW32_PLATFORM=1 -DEW32_DEBUG=1 on esp32-S1
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:4640
load:0x40078000,len:15632
load:0x40080400,len:3164
entry 0x4008059c
[Debug:DEVM] regist new device bus/i2c0 with id: 1
[Info:bus/i2c0] Found device 0x19
[Info:bus/i2c0] Found device 0x29
[Info:bus/i2c0] Found device 0x60
[Info:bus/i2c0] Found device 0x70
[Info:bus/i2c0] Found device 0x77
[Debug:/dev/gpio] marked pin 21 as used
[Debug:/dev/gpio] marked pin 22 as used
[Debug:DEVM] regist new device bus/pmn0 with id: 2
[Debug:DEVM] regist new device null with id: 3
[Debug:/dev/null] ew32_nulldev_probe
[Debug:/dev/null] ew32_nulldev_setup
[Debug:DEVM] regist new device stdout with id: 4
[Debug:DEVM] regist new device stderr with id: 5
[Debug:DEVM] regist new device stdin with id: 6
[Debug:ST0] Device Manager Create ... [OK]
[Debug:ST0] Initializing gpio and create isr state ... [OK]
[Debug:ST0] Initializing tasksystem ... [OK]
[Debug:ST0] Initializing VFS ... [OK]
[Info:ST0] Using ew32_base 0.5.01 and ew32_devm 0.5.0 

[Info:ST1] Enter to stage 1 ...
[Debug:MAIN] Device Manager run ... [OK]
[Debug:bus/pmn0] set mode state to: 2
[Debug:/dev/null] ew32_nulldev_set_state
[Debug:EW32_DEVM] Device 0: /dev/bus/uart0, Probe State: 4, Power State: not support
[Debug:EW32_DEVM] Device 1: /dev/bus/i2c0, Probe State: 4, Power State: 5
[Debug:EW32_DEVM] Device 2: /dev/bus/pmn0, Probe State: 4, Power State: 5
[Debug:EW32_DEVM] Device 3: /dev/null, Probe State: 4, Power State: 5
[Debug:EW32_DEVM] Device 4: /dev/stdout, Probe State: 4, Power State: not support
[Debug:EW32_DEVM] Device 5: /dev/stderr, Probe State: 4, Power State: not support
[Debug:EW32_DEVM] Device 6: /dev/stdin, Probe State: 4, Power State: not support

*/


#include "ew32_devm/ew32_devm.h"

char* xewLoggingLevelToString(uint8_t level) {
    if(level == EW32_LOGGING_LEVEL_DEBUG )   { return "Debug"; }
    if(level == EW32_LOGGING_LEVEL_INFO )   { return "Info"; }     
    if(level == EW32_LOGGING_LEVEL_WARNING )   { return "Warn"; }
    if(level == EW32_LOGGING_LEVEL_ERROR )   { return "Error"; }
    return "Critical"; 
}

int8_t __logging_handler(uint8_t level, const char* tag, const char* text) {

#if EW32_DEVICE_USE_UART == EW32_CONFIG_YES
    char buf[256];

    sprintf(buf, "[%s:%s] %s", xewLoggingLevelToString(level), tag, text);
    ew32_device_uart_send(EW32_DEVICE_UART_STDPORT, buf, strlen(buf));
#endif

    return 0;
}

// example the baterry is by 20% then switch to EW32_POWERSTATE_L1. EW32_POWERSTATE_l5 is full powerstate
uint8_t _ew32_power_change_handle(uint8_t curpwstate) {
    // gib zurück in welchen powserstate gewechselt werden soll EW32_POWERSTATE_L1 - EW32_POWERSTATE_L5 gloabal all devices
    return EW32_POWERSTATE_L5;
}

// return 0 when the cheak ok is, and 1 on wait
uint8_t __ew32_power_change_cheak(uint8_t curstate, uint8_t sollstate) {
    return 0;
}



void setup() {
    ew32_init_framework(&__logging_handler, EW32_LOGGING_LEVEL_DEBUG, EW32_LOGGING_LEVEL_CRITICAL);

    //### Start Add devices and set devices configs
    ew32_device_powermanager_sethandle(_ew32_power_change_handle, __ew32_power_change_cheak);


    //### End Add devices and set devices configs
    ew32_devicemanager_probeall();
    ew32_devicemanager_setup();
    if(ew32_devicemanager_run() != 0) {
        xewLoggingDebug("MAIN", "Device Manager run ... [Error]\n");
    } else {
        xewLoggingDebug("MAIN", "Device Manager run ... [OK]\n");
    }
    ew32_devicemanager_setstate_by_id(EW32_DEVICEMANAGER_ALL, EW32_DEVICE_STATEID_MODE, EW32_DEVICE_STATE_RESUME);

    ew32_devicemanager_dump(EW32_DEVICEMANAGER_STATE_ALL);
}

void loop(void) {
    ew32_devicemanager_loop(NULL);
}
