package com.simviu.IcEngTest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.os.Environment;
import android.content.res.AssetManager;

import com.simviu.IcEng.IcAssetHelper;
import com.simviu.IcEng.IcEngJNI;

import android.app.Activity;
import com.simviu.IcEng.IcActivity;

public class IcTestActivity extends IcActivity{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("IcEngTest");
    }

    @Override protected void onCreate(Bundle icicle) {
        initApp(m_sPathRes);
        super.onCreate(icicle);
        //----- CreateApp()
        copyAssetDir("IcShader");
    }

    @Override protected void onPause() {
        super.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
    }

    //-------------------------------------
    //  IcEng Renderer call back override
    //-------------------------------------
    @Override public void IcEng_onInit()
    {
        super.IcEng_onInit();
    }
    @Override public void IcEng_onViewSize(int w, int h)
    {
        super.IcEng_onViewSize(w, h);
    }
    @Override public void IcEng_onDrawUpdate(float deltaT)
    {
        super.IcEng_onDrawUpdate(deltaT);
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String stringFromJNI();
    public native void initApp(String jsPathRes);
}
