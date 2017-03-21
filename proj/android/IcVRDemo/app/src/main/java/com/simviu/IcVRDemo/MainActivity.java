package com.simviu.IcVRDemo;

import android.app.Activity;
import android.content.Intent;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import com.simviu.IcEng.IcEngJNI;
import com.simviu.IcVRDemo.VRDemoActivity;

public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("IcAppJNI");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //---- Init App at very beginning
        IcEngJNI.initIcApp(this);

        // Example of a call to a native method
   //     TextView tv = (TextView) findViewById(R.id.sample_text);
   //     tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
  //  public native String stringFromJNI();

    public void onBtn_VRDemo(View v) {

        //---- Start DemoActivity
        Intent intent = new Intent(MainActivity.this, VRDemoActivity.class);
        MainActivity.this.startActivity(intent);

    }

}
