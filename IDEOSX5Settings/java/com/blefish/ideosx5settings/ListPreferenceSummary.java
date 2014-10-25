package com.blefish.ideosx5settings;

import android.content.Context;
import android.preference.ListPreference;
import android.preference.Preference;
import android.util.AttributeSet;

/**
 * Created by rudolf on 2/1/14.
 * Updates the "summary" of the ListPreference to always match the selected entry.
 */
public class ListPreferenceSummary extends ListPreference implements Preference.OnPreferenceChangeListener {

    public ListPreferenceSummary(Context context) {
        super(context);
        setOnPreferenceChangeListener(this);
    }

    public ListPreferenceSummary(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object o) {
        preference.setSummary(getEntry());
        return true;
    }

    @Override
    public CharSequence getSummary() {
        return super.getEntry();
    }
}
