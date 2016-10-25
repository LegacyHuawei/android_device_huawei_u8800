#
# Copyright (C) 2012 The Android Open-Source Project
# Copyright (C) 2014 Rudolf Tammekivi <rtammekivi@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# WARNING: This line must come *before* including the proprietary
# variant, so that it gets overwritten by the parent (which goes
# against the traditional rules of inheritance).
# The proprietary variant sets USE_CAMERA_STUB := false, this way
# we use the camera stub when the vendor tree isn't present, and
# the true camera library when the vendor tree is available.
USE_CAMERA_STUB := true

# Inherit from proprietary
include vendor/huawei/u8800/BoardConfigVendor.mk

# Inherit from common
include device/huawei/msm7x30-common/BoardConfigCommon.mk

TARGET_BOOTLOADER_BOARD_NAME := u8800

# We don't build bootloader nor radio.
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Radio
BOARD_PROVIDES_LIBRIL := true

# Wi-Fi
BOARD_WLAN_DEVICE := qcwcn
BOARD_HAS_QCOM_WLAN := true
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
WPA_SUPPLICANT_VERSION := VER_0_8_X
HOSTAPD_VERSION := VER_0_8_X
WIFI_DRIVER_MODULE_PATH := "/system/lib/modules/wlan.ko"
WIFI_DRIVER_MODULE_NAME := "wlan"
WIFI_DRIVER_MODULE_ARG := ""
WIFI_EXT_MODULE_PATH := "/system/lib/modules/librasdioif.ko"
WIFI_EXT_MODULE_NAME := "librasdioif"
WIFI_EXT_MODULE_ARG := ""
WIFI_DRIVER_FW_PATH_STA := "sta"
WIFI_DRIVER_FW_PATH_AP := "ap"
WIFI_DRIVER_FW_PATH_P2P := "p2p"

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/huawei/u8800/bluetooth

# FM
BOARD_HAVE_QCOM_FM := true
AUDIO_FEATURE_ENABLED_FM := true

# Kernel
TARGET_KERNEL_CONFIG := u8800_defconfig

# Generic flags
BOARD_USE_LEGACY_SENSORS_FUSION := true

# inherit from the proprietary version
-include vendor/huawei/msm7x30-common/BoardConfigVendor.mk
