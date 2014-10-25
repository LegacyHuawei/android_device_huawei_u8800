package com.blefish.ideosx5settings;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Toast;

import com.blefish.ideosx5settings.settings.BTPower;
import com.blefish.ideosx5settings.settings.ForceMic;
import com.blefish.ideosx5settings.settings.HWProps;
import com.blefish.ideosx5settings.settings.MagCalib;
import com.blefish.ideosx5settings.settings.USBCurrent;
import com.blefish.ideosx5settings.settings.USBHost;

/**
 * Created by rudolf on 2/1/14.
 */
public class SettingsFragment extends PreferenceFragment implements SharedPreferences.OnSharedPreferenceChangeListener {
    public static final String PREF_FORCE_MIC = "pref_general_force_mic";
    public static final String PREF_BT_POWER = "pref_general_bt_power";
    public static final String PREF_HOST = "pref_usb_host";
    public static final String PREF_CURRENT = "pref_usb_current";
    public static final String PREF_MAG_CALIB = "pref_sensors_mag_calib";
    public static final String PREF_HWADDR_CUSTOM = "pref_hwaddr_custom";
    public static final String PREF_HWADDR_WLAN = "pref_hwaddr_wlan";
    public static final String PREF_HWADDR_BT = "pref_hwaddr_bt";

    private ForceMic forceMic;
    private BTPower btPower;
    private USBHost usbHost;
    private USBCurrent usbCurrent;
    private MagCalib magCalib;
    private HWProps hwProps;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Load the preferences from an XML resource
        addPreferencesFromResource(R.xml.preferences);

        initSettings();
    }

    @Override
    public void onResume() {
        super.onResume();
        getPreferenceScreen().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);

    }

    @Override
    public void onPause() {
        super.onPause();
        getPreferenceScreen().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String s) {
        if (s.equals(PREF_FORCE_MIC)) {
            forceMic.setEnabled(sharedPreferences.getBoolean(s, false));
            forceMic.showToastIfNeeded(getActivity());
        } else if (s.equals(PREF_BT_POWER)) {
            btPower.setPower(Integer.valueOf(sharedPreferences.getString(s, getString(R.string.pref_general_bt_power_default_value))));
            btPower.showToastIfNeeded(getActivity());
        } else if (s.equals(PREF_HOST)) {
            Boolean enabled = sharedPreferences.getBoolean(s, false);
            usbHost.setEnabled(enabled);
            if (enabled)
                usbHost.showNotification(getActivity());
            else
                usbHost.cancelNotification(getActivity());
        } else if (s.equals(PREF_CURRENT)) {
            usbCurrent.setCurrent(Integer.valueOf(sharedPreferences.getString(s, getString(R.string.pref_usb_current_default_value))));
        } else if (s.equals(PREF_HWADDR_CUSTOM)) {
            Boolean enabled = sharedPreferences.getBoolean(s, false);
            findPreference(PREF_HWADDR_BT).setEnabled(enabled);
            findPreference(PREF_HWADDR_WLAN).setEnabled(enabled);
            if (enabled) {
                hwProps.setBtMac(sharedPreferences.getString(PREF_HWADDR_BT, "00:11:22:33:44:55"));
                hwProps.setWlanMac(sharedPreferences.getString(PREF_HWADDR_WLAN, "00:11:22:33:44:55"));
            } else {
                hwProps.reset();

                findPreference(PREF_HWADDR_BT).setSummary(hwProps.getCurrentBtMac());
                findPreference(PREF_HWADDR_WLAN).setSummary(hwProps.getCurrentWlanMac());
            }
        } else if (s.equals(PREF_HWADDR_BT)) {
            hwProps.setBtMac(sharedPreferences.getString(s, "00:11:22:33:44:55"));
        } else if (s.equals(PREF_HWADDR_WLAN)) {
            hwProps.setWlanMac(sharedPreferences.getString(s, "00:11:22:33:44:55"));
        }
    }

    private void initSettings() {
        forceMic = new ForceMic();
        btPower = new BTPower();
        usbHost = new USBHost();
        usbCurrent = new USBCurrent();
        magCalib = new MagCalib();
        hwProps = new HWProps();

        findPreference(PREF_FORCE_MIC).setEnabled(forceMic.isSupported());
        findPreference(PREF_BT_POWER).setEnabled(btPower.isSupported());
        findPreference(PREF_HOST).setEnabled(usbHost.isSupported());
        findPreference(PREF_CURRENT).setEnabled(usbCurrent.isSupported());
        findPreference(PREF_MAG_CALIB).setEnabled(magCalib.isSupported());
        findPreference(PREF_HWADDR_CUSTOM).setEnabled(hwProps.isSupported());

        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(getActivity());
        Boolean enabled = sharedPreferences.getBoolean(PREF_HWADDR_CUSTOM, false);
        findPreference(PREF_HWADDR_BT).setEnabled(enabled);
        findPreference(PREF_HWADDR_WLAN).setEnabled(enabled);
        if (!enabled) {
            findPreference(PREF_HWADDR_BT).setSummary(hwProps.getCurrentBtMac());
            findPreference(PREF_HWADDR_WLAN).setSummary(hwProps.getCurrentWlanMac());
        }

        findPreference(PREF_MAG_CALIB).setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
            @Override
            public boolean onPreferenceClick(Preference preference) {
                magCalib.calibrate();
                preference.setEnabled(false);
                return true;
            }
        });
    }
}
