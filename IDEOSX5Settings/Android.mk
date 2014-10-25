#
# Copyright (c) 2014, Rudolf Tammekivi.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 and
# only version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_STATIC_JAVA_LIBRARIES := android-support-v4-kitkat

LOCAL_SRC_FILES := $(call all-java-files-under, java)

LOCAL_PACKAGE_NAME := IDEOSX5Settings
LOCAL_CERTIFICATE := platform

include $(BUILD_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))
