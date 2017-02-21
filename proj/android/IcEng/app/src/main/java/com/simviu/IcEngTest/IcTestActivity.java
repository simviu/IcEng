package com.simviu.IcEngTest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.os.Environment;
import android.content.res.AssetManager;

import com.simviu.IcEng.IcAssetHelper;
import com.simviu.IcEng.IcEngJNI;

import android.app.Activity;
import com.simviu.IcEng.IcActivity;
import com.simviu.IcEng.IcEngView;

public class IcTestActivity extends Activity{//IcActivity{


     @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
     //   setContentView(R.layout.activity_main);




        /*
        //---- Add IcEngView
        FrameLayout icLayout = (FrameLayout) findViewById(R.id.id_IcEngViewLayout); // Find the layout where you want to add button
        IcEngView view = getLayoutInflater()
        */
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
    /*
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
    */

}
