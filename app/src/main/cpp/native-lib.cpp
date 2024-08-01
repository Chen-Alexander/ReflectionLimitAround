#include <jni.h>
#include <string>
#include "future"

JNIEnv *currentEnv = nullptr;
JavaVM *globalVM = nullptr;

JNIEnv *attachCurrentThread() {
    JNIEnv *env;
    int resCode = globalVM->AttachCurrentThread(&env, nullptr);
    return env;
}

void detachCurrentThread() {
    int resCode = globalVM->DetachCurrentThread();
    printf("abc");
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    if (vm->GetEnv((void **) &currentEnv, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
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