package com.blefish.ideosx5settings.settings;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.blefish.ideosx5settings.MainActivity;
import com.blefish.ideosx5settings.R;

import java.io.File;

/**
 * Created by rudolf on 2/1/14.
 */
public class USBHost implements Setting {

    private static final String USB_HOST_PATH = "/sys/devices/platform/msm_otg/mode";

    private static final Integer USB_HOST_NOT_ID = 0;

    @Override
    public boolean isSupported() {
        File f = new File(USB_HOST_PATH);
        return f.exists();
    }

    public Boolean getEnabled() {
        return FileUtils.readOneLine(USB_HOST_PATH).equals("1");
    }

    public void setEnabled(Boolean value) {
        FileUtils.writeLine(USB_HOST_PATH, value ? "1" : "0");
    }

    public void showNotification(Context context) {
        Intent appIntent = new Intent(context, MainActivity.class);
        PendingIntent pendingAppIntent = PendingIntent.getActivity(context, 0, appIntent, 0);

        Notification not = new Notification.Builder(context)
                .setSmallIcon(com.android.internal.R.drawable.stat_sys_data_usb)
                .setContentTitle(context.getString(R.string.pref_usb_host_not_title))
                .setContentText(context.getString(R.string.pref_usb_host_not_text))
                .setContentIntent(pendingAppIntent)
                .setAutoCancel(false)
                .build();
        not.flags |= Notification.FLAG_ONGOING_EVENT;

        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(USB_HOST_NOT_ID, not);
    }

    public void cancelNotification(Context context) {
        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(USB_HOST_NOT_ID);
    }
}
