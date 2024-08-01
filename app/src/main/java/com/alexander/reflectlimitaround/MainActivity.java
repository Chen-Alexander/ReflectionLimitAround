package com.alexander.reflectlimitaround;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.Choreographer;

import com.alexander.reflectlimitaround.databinding.ActivityMainBinding;
import com.alexander.reflectlimitaround.util.ReflectUtils;


public class MainActivity extends AppCompatActivity {

    // Used to load the 'reflectlimitaround' library on application startup.
    static {
        System.loadLibrary("reflectlimitaround");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        try {
            // JNI破坏调用栈，使VM无法识别调用栈，进而无法识别调用方来达成目的()
            setHiddenApiExemptionsNative();
            Choreographer choreographer = Choreographer.getInstance();
            Object frameIntervalNanos = ReflectUtils.reflectObject(
                    choreographer, "mFrameIntervalNanos",
                    166666667L
            );
            Object vsyncReceiver = ReflectUtils.reflectObject(choreographer, "mDisplayEventReceiver", null);
            Object callbackQueueLock = ReflectUtils.reflectObject(choreographer, "mLock", null);
            Log.e("MainActivity", "执行");
        } catch (Exception e) {
             e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'reflectlimitaround' native library,
     * which is packaged with this application.
     */
    public native void setHiddenApiExemptionsNative();
}