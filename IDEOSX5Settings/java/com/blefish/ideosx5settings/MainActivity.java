package com.blefish.ideosx5settings;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
    public static final String LOG_TAG = "IDEOSX5Settings";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new SettingsFragment())
                    .commit();
        }
    }
}
