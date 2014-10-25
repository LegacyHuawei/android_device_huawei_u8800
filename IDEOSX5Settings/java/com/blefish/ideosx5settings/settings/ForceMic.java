package com.blefish.ideosx5settings.settings;

import android.content.Context;
import android.media.AudioManager;
import android.os.SystemProperties;
import android.widget.Toast;

import com.blefish.ideosx5settings.R;

/**
 * Created by rudolf on 2/1/14.
 */
public class ForceMic implements Setting {

    private static final String FORCE_MIC_SUPPORTED_PROP = "ro.hs_intmic.supported";
    private static final String FORCE_MIC_ENABLE_PROP = "persist.service.audio.hs_intmic";

    @Override
    public boolean isSupported() {
        return SystemProperties.getBoolean(FORCE_MIC_SUPPORTED_PROP, false);
    }

    public Boolean getEnabled() {
        return SystemProperties.getBoolean(FORCE_MIC_ENABLE_PROP, false);
    }

    public void setEnabled(Boolean value) {
        SystemProperties.set(FORCE_MIC_ENABLE_PROP, value ? "1" : "0");
    }

    public void showToastIfNeeded(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);

        /* Need to reset the routing process. */
        if (audioManager.isWiredHeadsetOn()) {
            Toast.makeText(context, context.getString(R.string.pref_general_force_mic_toast), Toast.LENGTH_SHORT).show();
        }
    }
}
