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


public class MainActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        TextView  tv = new TextView(this);
        tv.setText( "..." );
        setContentView(tv);
        //change the path to your things
        File in = new File("/storage/emulated/0/AppProjects/AMLModTemplate/libs/architrcutr/libmodtemplate.so");
        File out = new File("/storage/emulated/0/Android/data/com.modded.app/mods/libmodtemplate.so");
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
