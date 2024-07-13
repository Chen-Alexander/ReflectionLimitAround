package com.alexander.reflectlimitaround;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.MessageQueue;
import android.util.Log;
import android.widget.TextView;

import com.alexander.reflectlimitaround.databinding.ActivityMainBinding;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'reflectlimitaround' library on application startup.
    static {
        System.loadLibrary("reflectlimitaround");
    }

    private ActivityMainBinding binding;
    private static Object vmRuntime;
    private static Method setHiddenApiExemptions;

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            Log.e("MainActivity", "msg.what:" + msg.what);
        }
    };

    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            Log.e("MainActivity", "I am runnable");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        try {
//            Method forName = Class.class.getDeclaredMethod("forName", String.class);
//            Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod",
//                    String.class, Class[].class);
//            Class<?> vmRunTimeClass = (Class<?>) forName.invoke(null, "dalvik.system.VMRuntime");
//            Method getRuntime = (Method) getDeclaredMethod.invoke(vmRunTimeClass, "getRuntime", null);
//            vmRuntime = getRuntime.invoke(null);
//            setHiddenApiExemptions = (Method) getDeclaredMethod.invoke(vmRunTimeClass, "setHiddenApiExemptions", new Class[]{String[].class});
//
//            Looper looper = Looper.getMainLooper();
//            Method method = Looper.class.getDeclaredMethod("postSyncBarrier", null);
//            setHiddenApiExemptions.invoke(method);

//            // 单一套娃执行(执行每个方法前都要套娃一次)
//            handler.post(runnable);
//            Method forName = Class.class.getDeclaredMethod("forName", String.class);
//            Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod",
//                    String.class, Class[].class);
//            Class<MessageQueue> queueClass = (Class<MessageQueue>) forName.invoke(null, "android.os.MessageQueue");
//            Method hasMessages = (Method) getDeclaredMethod.invoke(queueClass, "hasMessages", new Class[]{Handler.class, Runnable.class, Object.class});
//            hasMessages.setAccessible(true);
//            MessageQueue queue = Looper.getMainLooper().getQueue();
//            boolean retCode = (boolean) hasMessages.invoke(queue, new Object[]{handler, runnable, null});
//
//            handler.sendEmptyMessage(0);

            // JNI破坏调用栈，使VM无法识别调用栈，进而无法识别调用方来达成目的()
            MessageQueue queue = Looper.getMainLooper().getQueue();
            Class clazz = Class.class;
            Method method = getDeclaredMethod(clazz, "hasMessages", new Class[]{Handler.class, Runnable.class, Object.class});
            Log.e("MainActivity", "执行");
        } catch (Exception e) {
             e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'reflectlimitaround' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void setHiddenApiExemptionsNative();
}