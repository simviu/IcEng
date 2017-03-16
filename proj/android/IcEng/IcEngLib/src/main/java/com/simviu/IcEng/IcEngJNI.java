/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.simviu.IcEng;

// Wrapper for native library

import android.content.Context;

public class IcEngJNI {

     static {
         System.loadLibrary("IcEng");
         System.loadLibrary("IcAppJNI");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     static native void createIcApp();

     public static native void initIcAppWithDir(String sPathRes, String sPathDoc);
     public static native void initWindow();
     public static native void releaseWindow();
     public static native void setScreenSize(int width, int height);
     public static native void drawUpdate(float deltaT);
     public static native void debugPrint(String sMsg);
     public static native String sendAppCmd(String sCmd);

    //---- Device Status
    //--- Pitch/Roll/Yaw
    public static native void updateDeviceAttitude(float p, float r, float y);

     public static void initIcApp(Context context)
     {
         createIcApp();
         String sPathCache = context.getCacheDir().toString();
         String sPathRes = sPathCache + "/IcData/";
         String sPathDoc = sPathCache;
         initIcAppWithDir(sPathRes, sPathDoc);
         copyAssetDir(context, "IcData");
     }
     //-----  Utils
     public static void copyAssetDir(Context context, String sPath)
     {
        IcAssetHelper hlpr = new IcAssetHelper(context);
        hlpr.copyAssetsDir(sPath);
     }

}
