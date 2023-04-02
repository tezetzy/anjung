/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.rasp.amltempapp;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import android.content.Intent;
import android.widget.Toast;
import android.util.Log;
import android.net.Uri;
import android.os.storage.StorageManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.os.Build;
import android.Manifest;
import android.content.pm.PackageManager;

public class MainActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its content.
         
         */
        TextView  tv = new TextView(this);
        tv.setText( "If you're on android 10 or above and you just granted this app access to android/data, please restart the app\nyou will not see this message later" );
        setContentView(tv);
        
        
        //change the path to your things
        
        File in = new File("/storage/emulated/0/AppProjects/AMLTemplateApp/libs/architecture/libmodtemplate.so");
        File out = new File("/storage/emulated/0/Android/data/com.modded.app/mods/libmodtemplate.so");
        boolean allowed=false;
        
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED) {
                requestPermissions(new String[] {Manifest.permission.READ_EXTERNAL_STORAGE}, 1000);
			}
        }
        
        // for android q and above
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.Q) {
        if (!sp.getBoolean("allowed",allowed))
        {
            sp.edit().putBoolean("allowed",true).apply();
            StorageManager sm = (StorageManager) this.getSystemService(Context.STORAGE_SERVICE);

            Intent intent = sm.getPrimaryStorageVolume().createOpenDocumentTreeIntent();
            //String startDir = "Android";
            //String startDir = "Download"; // Not choosable on an Android 11 device
            //String startDir = "DCIM";
            //String startDir = "DCIM/Camera";  // replace "/", "%2F"
            //String startDir = "DCIM%2FCamera";
            // String startDir = "Documents";
            String startDir = "Android/data";

            Uri uri = intent.getParcelableExtra("android.provider.extra.INITIAL_URI");

            String scheme = uri.toString();

            //Log.d(TAG, "INITIAL_URI scheme: " + scheme);

            scheme = scheme.replace("/root/", "/document/");

            startDir = startDir.replace("/", "%2F");

            scheme += "%3A" + startDir;

            uri = Uri.parse(scheme);

            intent.putExtra("android.provider.extra.INITIAL_URI", uri);

            //Log.d(TAG, "uri: " + uri.toString());
            Toast.makeText(MainActivity.this,"Grant the app access to this folder",Toast.LENGTH_LONG).show();
            this.startActivityForResult(intent,1);// Intent.action.REQUEST_ACTION_OPEN_DOCUMENT_TREE);

            return;
        }}
        https://stackoverflow.com/questions/67726278/android-11-read-android-data-directory-of-all-apps-without-legacy-request-fil#:~:text=if%20(android.os,REQUEST_ACTION_OPEN_DOCUMENT_TREE)%3B%0A%0A%20%20%20%20%20%20%20%20return%3B%0A%20%20%20%20%7D
        try {
            copy(in,out);
            Intent li = getPackageManager().getLaunchIntentForPackage("com.modded.app");
            startActivity(li);
            Toast.makeText(MainActivity.this,"Copied",Toast.LENGTH_LONG).show();
            finishAndRemoveTask();
        } catch(IOException e) {
            tv.setText("Failed".concat(e.toString()));
        }

    }
    public static void copy(File src, File dst) throws IOException{
        InputStream in = new FileInputStream(src);
        try{
            OutputStream out = new FileOutputStream(dst);
            try{
                byte[] buff = new byte[1024];
                int len;
                while ((len=in.read(buff))>0) {
                    out.write(buff,0,len);
                }

            } finally {
                out.close();
            }
        } finally {
            in.close();
        }
    }
    
}
