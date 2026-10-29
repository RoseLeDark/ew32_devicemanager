#include <Arduino.h>
#include <Wire.h>

#include "ew32_devm/ew32_api.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_devm.h"


#include <stddef.h>
#include <string.h>

uint8_t ew32_testdev_probe(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_probe");

    return 0;
}
uint8_t ew32_testdev_setup(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_setup");
    return 0;
}
uint8_t ew32_testdev_powerstate(ew32_dev_t* device, uint8_t state) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_powerstate");
    return 0;
}
int32_t ew32_testdev_write(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_write");
    memset(data, 0, size);
    return 0;
}
int32_t ew32_testdev_read(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_read");
    memset(data, 0, size);
    return 0;
}

ew32_dev_t* __global_device[10];

uint8_t ew32_device_test0_setup(uint8_t id, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_device[id]->subpath[0] = 't';
    __global_device[id]->subpath[1] = 'e';
    __global_device[id]->subpath[2] = 's';
    __global_device[id]->subpath[3] = 't';
    __global_device[id]->subpath[4] = id + '0'; // ID in ASCI char


    __global_device[id]->cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_device[id]->type = EW32_DEVICE_TYPE_STREAM;
    __global_device[id]->parent = NULL;
    __global_device[id]->rw =  EW32_DEVICE_READ_WRITE;
    __global_device[id]->powerstate = ew32_testdev_powerstate;
    __global_device[id]->probe = ew32_testdev_probe;
    __global_device[id]->setup = ew32_testdev_setup;
    __global_device[id]->write = ew32_testdev_write;
    __global_device[id]->read = ew32_testdev_read;

    __global_device[id]->userdata = NULL;
    __global_device[id]->parent = 0;

    *device = &__global_device[id];
    return EW32_DEVICE_NO_ERROR;
}



int8_t __logging_handler(uint8_t level, const char* tag, const char* text) {
   Serial.printf("[%d:%s] %s\n", level, tag, text); return 0;
}

void setup() {
   ew32_dev_t* __device = (ew32_dev_t*)malloc(sizeof(ew32_dev_t) * 10);

   Serial.begin(115200);
   xewLoggingInitEx(&__logging_handler, EW32_LOGGING_LEVEL_DEBUG );

   ew32_tasksystem_init();
   ew32_devicemanager_create();

   for(int i = 0; i < 10; i++) {
         ew32_device_test0_setup(i, &__device[i]);
         ew32_devicemanager_registdevice(&__device[i], NULL, 0);
   }

   ew32_devicemanager_probeall();
   ew32_devicemanager_setup();
   ew32_devicemanager_run();


}

void loop(void) {

}
