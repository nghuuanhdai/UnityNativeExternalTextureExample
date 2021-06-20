#include "RenderingPlugin.h"

#include <GLES2/gl2.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "RENDERING_PLUGIN"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static JavaVM *gJavaVM;
static jobject gCallbackObject;

static void UNITY_INTERFACE_API OnRenderEvent(int eventCode)
{
	int status;
	JNIEnv *env;
	int isAttached = 0;

	if ((status = gJavaVM->GetEnv((void **)&env, JNI_VERSION_1_6)) < 0)
	{
		if ((status = gJavaVM->AttachCurrentThread(&env, NULL)) < 0)
		{
			return;
		}
		isAttached = 1;
	}

	jclass cls = env->GetObjectClass(gCallbackObject);
	if (!cls)
	{
		if (isAttached)
			gJavaVM->DetachCurrentThread();
		return;
	}

	jmethodID method = env->GetMethodID(cls, "JavaOnRenderEvent", "(I)V");
	if (!method)
	{
		if (isAttached)
			gJavaVM->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(gCallbackObject, method, eventCode);

	if (isAttached)
		gJavaVM->DetachCurrentThread();
}

// --------------------------------------------------------------------------
// GetRenderEventFunc, used to get a rendering event callback function.
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
	return OnRenderEvent;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	gJavaVM = vm;

	return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL
Java_com_nnhhaadd_nativetextureplugin_RenderingCallbackManager_nativeInit(JNIEnv *env, jobject obj)
{
    gCallbackObject = env->NewGlobalRef(obj);
}