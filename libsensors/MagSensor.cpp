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

#define MAG_DEBUG 0

#include <cutils/log.h>
#include <fcntl.h>
#include <string.h>

#include "MagSensor.h"

/*****************************************************************************/

template <class T> const T& min (const T& a, const T& b) {
  return !(b<a)?a:b;     // or: return !comp(b,a)?a:b; for version (2)
}
template <class T> const T& max (const T& a, const T& b) {
  return (a<b)?b:a;     // or: return comp(a,b)?b:a; for version (2)
}


MagSensor::MagSensor()
    : SensorBase(LSM303DLH_MAG_DEVICE, LSM303DLH_MAG_NAME),
      mInputReader(4), mEnabled(0), mHasPendingEvent(false),
      calibrationInfo(false)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: Initializing...");

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_M;
    mPendingEvent.type = SENSOR_TYPE_MAGNETIC_FIELD;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    open_device();

    int enabled;
    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_GET_ENABLE, &enabled))
        ALOGE("MagSensor: Failed to get status.");

    mEnabled = enabled ? true : false;

    if (!mEnabled)
        close_device();
}

MagSensor::~MagSensor() {
    close_device();
}

int MagSensor::setEnable(int32_t handle, int enabled)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: enable %d %d", handle, enabled);

    bool _enabled = enabled ? true : false;

    if (mEnabled == _enabled)
        return 0;

    if (_enabled) {
        restoreCalibrationInfo();
        open_device();
    }

    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_SET_ENABLE, &enabled))
        ALOGE("MagSensor: Failed to set enable.");

    mEnabled = _enabled;

    if (!_enabled) {
        saveCalibrationInfo();
        close_device();
    }

    return 0;
}

int MagSensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: setDelay %d %lld", handle, ns);

    if (!mEnabled)
        return 0;

    int ms = ns / 1000000;

    if (ioctl(dev_fd, LSM303DLH_MAG_IOCTL_SET_DELAY, &ms))
        ALOGE("MagSensor: Failed to set delay.");

    return 0;
}

bool MagSensor::hasPendingEvents() const
{
    return mHasPendingEvent;
}

int MagSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent)
    {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event))
    {
        int type = event->type;
        if (type == EV_ABS)
        {
            if (event->code == EVENT_TYPE_MAGV_X)
                mPendingEvent.magnetic.x = getCorrectReading(event->code, event->value);
            else if (event->code == EVENT_TYPE_MAGV_Y)
                mPendingEvent.magnetic.y = getCorrectReading(event->code, event->value);
            else if (event->code == EVENT_TYPE_MAGV_Z)
                mPendingEvent.magnetic.z = getCorrectReading(event->code, event->value);
        }
        else if (type == EV_SYN)
        {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled)
            {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("MagSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

float MagSensor::getCorrectReading(int code, int value)
{
    /* Shift to zero and calibrate it on the go. */
    switch (code) {
    case EVENT_TYPE_MAGV_X:
        if (!calibrationInfo) {
            minVal[X] = min(minVal[X], value);
            maxVal[X] = max(maxVal[X], value);
        }

        value = value - ((minVal[X] + maxVal[X]) / 2);
        break;
    case EVENT_TYPE_MAGV_Y:
        if (!calibrationInfo) {
            minVal[Y] = min(minVal[Y], value);
            maxVal[Y] = max(maxVal[Y], value);
        }

        value = value - ((minVal[Y] + maxVal[Y]) / 2);
        break;
    case EVENT_TYPE_MAGV_Z:
        if (!calibrationInfo) {
            minVal[Z] = min(minVal[Z], value);
            maxVal[Z] = max(maxVal[Z], value);
        }

        value = value - ((minVal[Z] + maxVal[Z]) / 2);
        break;
    default:
        break;
    }

    return value * CONVERT_M;
}

void MagSensor::restoreCalibrationInfo()
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: Restoring calibration information");
    int cal_fd =
        open(LSM303DLH_MAG_CALIBRATION, O_RDONLY);

    if (cal_fd < 0) {
        ALOGD_IF(MAG_DEBUG, "MagSensor: No calibration information");
        minVal[X] = minVal[Y] = minVal[Z] = INT_MAX;
        maxVal[X] = maxVal[Y] = maxVal[Z] = -INT_MAX;
        calibrationInfo = false;
        return;
    }

    char buffer[128];

    read(cal_fd, buffer, 128);

    if (sscanf(buffer,
        "XminVal=%d XmaxVal=%d\n"
        "YminVal=%d YmaxVal=%d\n"
        "ZminVal=%d ZmaxVal=%d\n",
        &minVal[X], &maxVal[X],
        &minVal[Y], &maxVal[Y],
        &minVal[Z], &maxVal[Z]) != 6) {
        ALOGE("MagSensor: Invalid calibration information");
        close(cal_fd);
        minVal[X] = minVal[Y] = minVal[Z] = INT_MAX;
        maxVal[X] = maxVal[Y] = maxVal[Z] = -INT_MAX;
        calibrationInfo = false;
        return;
    }

    calibrationInfo = true;
    close(cal_fd);
}

void MagSensor::saveCalibrationInfo()
{
    ALOGD_IF(MAG_DEBUG, "MagSensor: Saving calibration information");
    int cal_fd =
        open(LSM303DLH_MAG_CALIBRATION, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (cal_fd < 0) {
        ALOGE("MagSensor: Cannot open calibration information");
        return;
    }

    char buffer[128];

    int length = snprintf(buffer, 128,
        "XminVal=%d XmaxVal=%d\n"
        "YminVal=%d YmaxVal=%d\n"
        "ZminVal=%d ZmaxVal=%d\n",
        minVal[X], maxVal[X], minVal[Y], maxVal[Y], minVal[Z], maxVal[Z]);

    write(cal_fd, buffer, length);

    close(cal_fd);
}
