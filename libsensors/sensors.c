/*
 * Copyright (C) 2012 The Android Open-Source Project
 * Copyright (C) 2014 Rudolf Tammekivi <rtammekivi@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hardware/sensors.h>

#include "nusensors.h"

/*****************************************************************************/

/*
 * The SENSORS Module
 */

static const struct sensor_t sSensorList[] = {
    {
        .name       = "LSM303DLH 3-axis Accelerometer",
        .vendor     = "ST Microelectronics",
        .version    = 1,
        .handle     = ID_A,
        .type       = SENSOR_TYPE_ACCELEROMETER,
        .maxRange   = MAX_RANGE_A,
        .resolution = CONVERT_A,
        .power      = 0.5f,
        .minDelay   = 1000,
        .reserved   = { }
    },
    {
        .name       = "APS-12D Light sensor",
        .vendor     = "Everlight",
        .version    = 1,
        .handle     = ID_L,
        .type       = SENSOR_TYPE_LIGHT,
        .maxRange   = 64000.0f,
        .resolution = 1.0f,
        .power      = 0.07f,
        .minDelay   = 53000,
        .reserved   = { }
    },
    {
        .name       = "LSM303DLH Magnetometer",
        .vendor     = "ST Microelectronics",
        .version    = 1,
        .handle     = ID_M,
        .type       = SENSOR_TYPE_MAGNETIC_FIELD,
        .maxRange   = 400.0f,
        .resolution = 0.1f,
        .power      = 0.5f,
        .minDelay   = 14000,
        .reserved   = { }
    },
    {
        .name       = "APS-12D Proximity sensor",
        .vendor     = "Everlight",
        .version    = 1,
        .handle     = ID_P,
        .type       = SENSOR_TYPE_PROXIMITY,
        .maxRange   = 1.0f,
        .resolution = 1.0f,
        .power      = 50.0f,
        .minDelay   = 106000,
        .reserved   = { }
    },
};

static int open_sensors(const struct hw_module_t* module, const char* name,
        struct hw_device_t** device);

static int sensors__get_sensors_list(struct sensors_module_t* module,
        struct sensor_t const** list)
{
    *list = sSensorList;
    return ARRAY_SIZE(sSensorList);
}

static struct hw_module_methods_t sensors_module_methods = {
    .open = open_sensors
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = SENSORS_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = SENSORS_HARDWARE_MODULE_ID,
        .name = "U8800 Sensors Module",
        .author = "The Android Open Source Project",
        .methods = &sensors_module_methods,
        .dso = 0,
        .reserved = { 0 },
    },
    .get_sensors_list = sensors__get_sensors_list,
};

/*****************************************************************************/

static int open_sensors(const struct hw_module_t* module, const char* name,
        struct hw_device_t** device)
{
    return init_nusensors(module, device);
}
