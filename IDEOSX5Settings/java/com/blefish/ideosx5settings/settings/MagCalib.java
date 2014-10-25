package com.blefish.ideosx5settings.settings;

import java.io.File;

/**
 * Created by rudolf on 2/1/14.
 */
public class MagCalib implements Setting {

    private static final String MAG_CALIB_FILE = "/data/misc/sensors/lsm303dlh_mag";

    @Override
    public boolean isSupported() {
        File f = new File(MAG_CALIB_FILE);
        return f.exists();
    }

    public void calibrate() {
        File f = new File(MAG_CALIB_FILE);
        f.delete();
    }
}
