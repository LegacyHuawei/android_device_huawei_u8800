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

#define LIGHT_DEBUG 0

#include <cutils/log.h>
#include <string.h>

#include "LightSensor.h"

/*****************************************************************************/

LightSensor::LightSensor()
    : SensorBase(APS_12D_DEVICE, APS_12D_NAME),
      mInputReader(4), mEnabled(0), mHasPendingEvent(false)
{
    ALOGD_IF(LIGHT_DEBUG, "LightSensor: Initializing...");

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_L;
    mPendingEvent.type = SENSOR_TYPE_LIGHT;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    open_device();

    if (ioctl(dev_fd, APS_IOCTL_GET_LIGHT_ENABLE, &mEnabled))
        ALOGE("LightSensor: Failed to get status.");

    if (ioctl(dev_fd, APS_IOCTL_GET_SETTINGS, &settings))
        ALOGE("LightSensor: Failed to get settings.");

    if (!mEnabled)
        close_device();
}

LightSensor::~LightSensor() {
    close_device();
}

int LightSensor::setEnable(int32_t handle, int enabled)
{
    ALOGD_IF(LIGHT_DEBUG, "LightSensor: enable %d %d", handle, enabled);

    bool _enabled = enabled ? true : false;

    if (mEnabled == _enabled)
        return 0;

    if (_enabled)
        open_device();

    if (ioctl(dev_fd, APS_IOCTL_SET_LIGHT_ENABLE, &_enabled))
        ALOGE("LightSensor: Failed to set light enable.");

    mEnabled = _enabled;

    if (!_enabled)
        close_device();

    return 0;
}

int LightSensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD_IF(LIGHT_DEBUG, "LightSensor: setDelay %d %lld", handle, ns);

    if (!mEnabled)
        return 0;

    if (ioctl(dev_fd, APS_IOCTL_SET_LIGHT_DELAY, &ns))
        ALOGE("LightSensor: Failed to set light delay.");

    return 0;
}

bool LightSensor::hasPendingEvents() const
{
    return mHasPendingEvent;
}

int LightSensor::readEvents(sensors_event_t* data, int count)
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
            if (event->code == EVENT_TYPE_LIGHT)
                mPendingEvent.light = getLuxValue(event->value);
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
            ALOGE("LightSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

float LightSensor::getLuxValue(uint16_t adc_count)
{
    float correct_adc = getCorrectADC(adc_count) * GLASS_MULTIPLIER;

    configureRange(adc_count);

    return floorf(correct_adc);
}

float LightSensor::getCorrectADC(uint16_t adc_count)
{
    float countValue;

    /* Read the settings. */
    /*if (ioctl(dev_fd, APS_IOCTL_GET_SETTINGS, &settings))
    {
        ALOGE("LightSensor: Failed to get settings.");
        return -EINVAL;
    }*/

    switch(settings.range)
    {
        case APS_12D_RANGE_0P24_TO_1000:
            countValue = 0.24f;
            break;
        case APS_12D_RANGE_0P96_TO_4000:
            countValue = 0.96f;
            break;
        case APS_12D_RANGE_3P84_TO_16000:
            countValue = 3.84f;
            break;
        case APS_12D_RANGE_15P36_TO_64000:
            countValue = 15.36f;
            break;
        default:
            countValue = 0.24f;
            break;
    }

    return adc_count * countValue;
}

/* Do this only on brightness ADC. */
void LightSensor::configureRange(uint16_t adc_count)
{
    if (!settings.allow_reconfig)
        return;

    struct aps_12d_settings new_settings = settings;
    bool increase;

    if (adc_count <= RANGE_DEC_THRESHOLD)
        increase = false;
    else if (adc_count >= RANGE_INC_THRESHOLD)
        increase = true;
    else
        return;

    if (increase && new_settings.range != APS_12D_RANGE_15P36_TO_64000)
        new_settings.range = static_cast<aps_12d_range>(new_settings.range + 1);
    else if (!increase && new_settings.range != APS_12D_RANGE_0P24_TO_1000)
        new_settings.range = static_cast<aps_12d_range>(new_settings.range - 1);
    else
        return;

    ALOGD_IF(LIGHT_DEBUG, "LightSensor: New range %d", new_settings.range);

    if (ioctl(dev_fd, APS_IOCTL_SET_SETTINGS, &new_settings))
        ALOGE("LightSensor: Failed to set new settings");
    else
        settings = new_settings;
}
