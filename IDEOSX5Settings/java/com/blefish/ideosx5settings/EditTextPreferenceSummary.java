package com.blefish.ideosx5settings;

import android.content.Context;
import android.preference.EditTextPreference;
import android.preference.Preference;
import android.util.AttributeSet;

/**
 * Created by rudolf on 2/1/14.
 * Updates the "summary" of the ListPreference to always match the selected entry.
 */
public class EditTextPreferenceSummary extends EditTextPreference implements Preference.OnPreferenceChangeListener {

    public EditTextPreferenceSummary(Context context) {
        super(context);
        setOnPreferenceChangeListener(this);
    }

    public EditTextPreferenceSummary(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object o) {
        preference.setSummary(getText());
        return true;
    }

    @Override
    public CharSequence getSummary() {
        return super.getText();
    }

    @Override
    public void setSummary(CharSequence summary) {
        setText(summary.toString());
        super.setSummary(summary);
    }
}
