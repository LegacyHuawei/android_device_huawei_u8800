package com.blefish.ideosx5settings.settings;

import java.io.File;
import java.io.IOException;

/**
 * Created by rudolf on 10/05/14.
 */
public class HWProps implements Setting {
    private static final String HWPROPS_DATA_PATH = "/data/misc/hwprops/";
    private static final String HWPROPS_DATA_BT_PATH = HWPROPS_DATA_PATH + "btmac";
    private static final String HWPROPS_DATA_WLAN_PATH = HWPROPS_DATA_PATH + "wlanmac";

    private static final String HWPROPS_SYSFS_PATH = "/sys/hwprops/";
    private static final String HWPROPS_SYSFS_BT_PATH = HWPROPS_SYSFS_PATH + "btmac";
    private static final String HWPROPS_SYSFS_WLAN_PATH = HWPROPS_SYSFS_PATH + "wlanmac";
    private static final String HWPROPS_SYSFS_RESET_PATH = HWPROPS_SYSFS_PATH + "reset";

    @Override
    public boolean isSupported() {
        File f = new File(HWPROPS_SYSFS_PATH);
        return f.isDirectory();
    }

    public void reset() {
        /* Remove custom addresses. */
        File f = new File(HWPROPS_DATA_BT_PATH);
        f.delete();
        f = new File(HWPROPS_DATA_WLAN_PATH);
        f.delete();

        FileUtils.writeLine(HWPROPS_SYSFS_RESET_PATH, "1");
    }

    public void setBtMac(String mac) {
        FileUtils.writeLine(HWPROPS_DATA_BT_PATH, mac);
        try {
            Runtime.getRuntime().exec("/system/bin/hwprops -btmac");
        } catch (IOException e) {
        }
    }

    public void setWlanMac(String mac) {
        FileUtils.writeLine(HWPROPS_DATA_WLAN_PATH, mac);
        try {
            Runtime.getRuntime().exec("/system/bin/hwprops -wlanmac");
        } catch (IOException e) {
        }
    }

    public String getCurrentBtMac() {
        return FileUtils.readOneLine(HWPROPS_SYSFS_BT_PATH);
    }

    public String getCurrentWlanMac() {
        return FileUtils.readOneLine(HWPROPS_SYSFS_WLAN_PATH);
    }
}
