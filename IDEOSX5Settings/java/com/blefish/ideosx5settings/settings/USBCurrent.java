package com.blefish.ideosx5settings.settings;

import java.io.File;

/**
 * Created by rudolf on 2/2/14.
 */
public class USBCurrent implements Setting {

    private static final String USB_CURRENT_PATH = "/sys/devices/platform/msm_hsusb/gadget/manual_chg_current";

    @Override
    public boolean isSupported() {
        File f = new File(USB_CURRENT_PATH);
        return f.exists();
    }

    public Integer getCurrent() {
        return Integer.valueOf(FileUtils.readOneLine(USB_CURRENT_PATH));
    }

    public void setCurrent(Integer value) {
        FileUtils.writeLine(USB_CURRENT_PATH, value.toString());
    }
}
