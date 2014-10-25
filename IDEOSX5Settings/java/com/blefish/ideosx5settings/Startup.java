package com.blefish.ideosx5settings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

import com.blefish.ideosx5settings.settings.BTPower;
import com.blefish.ideosx5settings.settings.ForceMic;
import com.blefish.ideosx5settings.settings.USBCurrent;
import com.blefish.ideosx5settings.settings.USBHost;

/**
 * Created by rudolf on 2/1/14.
 */
public class Startup extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction() == Intent.ACTION_BOOT_COMPLETED) {
            restoreSettings(context);
        }
    }

    private void restoreSettings(Context context) {
        Log.d(MainActivity.LOG_TAG, "Restoring settings");

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

        BTPower btPower = new BTPower();
        USBHost usbHost = new USBHost();
        USBCurrent usbCurrent = new USBCurrent();

        Boolean booleanValue;
        Integer integerValue;

        integerValue = Integer.valueOf(prefs.getString(SettingsFragment.PREF_BT_POWER, "1"));
        btPower.setPower(integerValue);

        booleanValue = prefs.getBoolean(SettingsFragment.PREF_HOST, false);
        if (booleanValue != usbHost.getEnabled()) {
            usbHost.setEnabled(booleanValue);
            usbHost.showNotification(context);
        }

        integerValue = Integer.valueOf(prefs.getString(SettingsFragment.PREF_CURRENT, "0"));
        usbCurrent.setCurrent(integerValue);
    }
}
