#include <jni.h>
#include <string>
#include "future"

JNIEnv *currentEnv = nullptr;
JavaVM *globalVM = nullptr;

jobject global_clazz = nullptr;
jstring global_method_name = nullptr;
jobjectArray global_params = nullptr;

extern "C" JNIEXPORT jstring JNICALL
Java_com_alexander_reflectlimitaround_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEnv *attachCurrentThread() {
    JNIEnv *env;
    int resCode = globalVM->AttachCurrentThread(&env, nullptr);
    return env;
}

void detachCurrentThread() {
    int resCode = globalVM->DetachCurrentThread();
    printf("abc");
}

//jobject getDeclaredMethod_internal(
//        jobject clazz,
//        jstring method_name,
//        jobjectArray params
//) {
//    JNIEnv *env = attachCurrentThread();
//    jclass clazz_class = env->GetObjectClass(clazz);
//    jmethodID get_declared_method_id = env->GetMethodID(clazz_class, "getDeclaredMethod",
//                                                        "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
//    jobject res = env->CallObjectMethod(clazz, get_declared_method_id, method_name, params);
//    DetachCurrentThread();
//    return env->NewGlobalRef(res);
//}

//extern "C" JNIEXPORT jobject JNICALL
//Java_com_alexander_reflectlimitaround_MainActivity_getDeclaredMethod(
//        JNIEnv *env,
//        jobject interface,
//        jobject clazz,
//        jstring method_name,
//        jobjectArray params
//        ) {
////    global_clazz = env->NewGlobalRef(clazz);
////    global_method_name = (jstring) env->GetStringUTFChars(method_name, nullptr);
////
//    auto future = std::async(std::launch::any, &getDeclaredMethod_internal, global_clazz,
//                             global_method_name, params);
//    auto result = future.get();
//
////    jclass class_clazz = env->GetObjectClass(clazz);
//    jclass class_clazz = env->FindClass("java/lang/Class");
//    jmethodID get_declared_method_id = env->GetMethodID(class_clazz, "getDeclaredMethod",
//                                                        "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
//    jobject result = env->CallObjectMethod(clazz, get_declared_method_id, method_name, params);
//
//    return result;
//}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    if (vm->GetEnv((void **) &currentEnv, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
//    globalVM = reinterpret_cast<JavaVM *>(currentEnv->NewGlobalRef(reinterpret_cast<jobject>(vm)));
    globalVM = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    currentEnv->DeleteGlobalRef(reinterpret_cast<jobject>(globalVM));
    globalVM = nullptr;
}

void* setHiddenApiExemptions_internal() {
    JNIEnv *threadEnv = attachCurrentThread();
    jclass vmRuntimeclazz = threadEnv->FindClass("dalvik/system/VMRuntime");
    jmethodID setHiddenApiExemptionsMethodId = threadEnv->GetMethodID(vmRuntimeclazz, "setHiddenApiExemptions", "([Ljava/lang/String;)V");
    jmethodID getRuntimeMethodId = threadEnv->GetStaticMethodID(vmRuntimeclazz, "getRuntime", "()Ldalvik/system/VMRuntime;");
    jobject vmruntime = threadEnv->CallStaticObjectMethod(vmRuntimeclazz, getRuntimeMethodId);
    jstring arg = threadEnv->NewStringUTF("L");
    jobjectArray signaturePrefixes = threadEnv->NewObjectArray(1, threadEnv->FindClass("java/lang/String"), arg);
    threadEnv->CallVoidMethod(vmruntime, setHiddenApiExemptionsMethodId, signaturePrefixes);
    detachCurrentThread();
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_com_alexander_reflectlimitaround_MainActivity_setHiddenApiExemptionsNative(JNIEnv *env,jobject thiz) {
    auto future = std::async(std::launch::any, &setHiddenApiExemptions_internal);
    auto result = future.get();
}