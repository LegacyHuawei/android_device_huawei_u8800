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

#ifndef ANDROID_MAG_SENSOR_H
#define ANDROID_MAG_SENSOR_H

#include <linux/input/lsm303dlh.h>

#include "nusensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

/*****************************************************************************/
#define LSM303DLH_MAG_NAME            "lsm303dlh_mag"
#define LSM303DLH_MAG_DEVICE          "/dev/" LSM303DLH_MAG_NAME
/*****************************************************************************/
#define LSM303DLH_MAG_CALIBRATION     "/data/misc/sensors/lsm303dlh_mag"

struct input_event;

class MagSensor : public SensorBase {
private:
    InputEventCircularReader mInputReader;
    bool mEnabled;
    bool mHasPendingEvent;
    sensors_event_t mPendingEvent;

    enum coordinate {
        X,
        Y,
        Z,
        MAX_COORDS
    };

    int minVal[MAX_COORDS];
    int maxVal[MAX_COORDS];

    bool calibrationInfo;

    float getCorrectReading(int code, int value);
    void restoreCalibrationInfo(void);
    void saveCalibrationInfo(void);

public:
            MagSensor();
    virtual ~MagSensor();
    virtual int setEnable(int32_t handle, int enabled);
    virtual int setDelay(int32_t handle, int64_t ns);
    virtual bool hasPendingEvents() const;
    virtual int readEvents(sensors_event_t* data, int count);
};

/*****************************************************************************/

#endif  // ANDROID_MAG_SENSOR_H
