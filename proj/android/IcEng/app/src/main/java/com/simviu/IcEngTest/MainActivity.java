package com.simviu.IcEngTest;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.simviu.IcEng.IcEngJNI;

public class MainActivity extends Activity {
    // Used to load the 'native-lib' library on application startup.
    static {
 //       System.loadLibrary("IcAppJNI");
    }
 //   static native void createIcApp();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //---- Create IcApp at very beginning
   //     createIcApp();
        //---- Init IcApp with JNI
        IcEngJNI.initIcApp(this);


    }

    public void onBtn_IcTest(View v) {

        //---- Start MyTestActivity
        Intent intent = new Intent(MainActivity.this, MyTestActivity.class);
        MainActivity.this.startActivity(intent);

    }
}
