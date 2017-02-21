package com.simviu.IcEngDemo;

import android.app.Activity;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.simviu.IcEng.IcActivity;
import com.simviu.IcEng.IcEngJNI;
import com.simviu.IcEng.IcAssetHelper;
import com.simviu.IcEng.IcEngView;

public class DemoActivity  extends IcActivity{

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        //----- CreateApp()
        copyAssetDir("IcShader");
        copyAssetDir("IcDemo");

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
}
