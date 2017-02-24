package com.simviu.IcEngTest;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.simviu.IcEng.IcEngJNI;

public class MainActivity extends Activity {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("IcEngTest");
    }
    static native void initIcApp(String sPathCache);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //---- Create IcApp at very beginning
        String sPathCache = getCacheDir().toString();
        initIcApp(sPathCache);

        //----- Copy asset
        IcEngJNI.copyAssetDir(this, "IcShader");


    }

    public void onBtn_IcTest(View v) {

        //---- Start MyTestActivity
        Intent intent = new Intent(MainActivity.this, MyTestActivity.class);
        MainActivity.this.startActivity(intent);

    }
}
