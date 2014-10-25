package com.blefish.ideosx5settings.settings;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.os.SystemProperties;
import android.widget.Toast;

import com.blefish.ideosx5settings.R;

/**
 * Created by rudolf on 2/1/14.
 */
public class BTPower implements Setting {

    private static final String BT_POWER_CLASS_PROP = "persist.service.bt.power";

    @Override
    public boolean isSupported() {
        return true;
    }

    public Integer getPower() {
        return SystemProperties.getInt(BT_POWER_CLASS_PROP, 1);
    }

    public void setPower(Integer value) {
        SystemProperties.set(BT_POWER_CLASS_PROP, value.toString());
    }

    public void showToastIfNeeded(Context context) {
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter != null && bluetoothAdapter.isEnabled())
            Toast.makeText(context, context.getString(R.string.pref_general_bt_power_toast), Toast.LENGTH_SHORT).show();
    }
}
