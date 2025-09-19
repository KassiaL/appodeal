#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "extension_private.h"
#include "com_defold_extension_ExtensionAppodealAds.h"
#include "extension_callback_private.h"

JNIEXPORT void JNICALL Java_com_defold_extension_ExtensionAppodealAds_AddToQueue(JNIEnv *env, jclass cls, jint jmsg, jstring jjson)
{
	const char *json = env->GetStringUTFChars(jjson, 0);
	dmAppodealAds::AddToQueueCallback((dmAppodealAds::MessageId)jmsg, json);
	env->ReleaseStringUTFChars(jjson, json);
}

namespace dmAppodealAds
{

	struct App
	{
		jobject m_AppJNI;

		jmethodID m_Initialize;

		jmethodID m_IsInterstitialLoaded;
		jmethodID m_ShowInterstitial;

		jmethodID m_IsRewardedLoaded;
		jmethodID m_ShowRewarded;

		// jmethodID m_SetUserConsent;
	};

	static App g_app;

	static void CallVoidMethod(jobject instance, jmethodID method)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		env->CallVoidMethod(instance, method);
	}

	static bool CallBoolMethod(jobject instance, jmethodID method)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		jboolean return_value = (jboolean)env->CallBooleanMethod(instance, method);
		return JNI_TRUE == return_value;
	}

	static void CallVoidMethodChar(jobject instance, jmethodID method, const char *cstr)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		jstring jstr = env->NewStringUTF(cstr);
		env->CallVoidMethod(instance, method, jstr);
		env->DeleteLocalRef(jstr);
	}

	static void CallVoidMethodCharInt(jobject instance, jmethodID method, const char *cstr, int cint)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		jstring jstr = env->NewStringUTF(cstr);
		env->CallVoidMethod(instance, method, jstr, cint);
		env->DeleteLocalRef(jstr);
	}

	static void CallVoidMethodCharBool(jobject instance, jmethodID method, const char *cstr, bool cbool)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		jstring jstr = env->NewStringUTF(cstr);
		jboolean jbool = cbool ? JNI_TRUE : JNI_FALSE;
		env->CallVoidMethod(instance, method, jstr, jbool);
		env->DeleteLocalRef(jstr);
	}

	static void CallVoidMethodInt(jobject instance, jmethodID method, int cint)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		env->CallVoidMethod(instance, method, cint);
	}

	static void CallVoidMethodBool(jobject instance, jmethodID method, bool cbool)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();

		env->CallVoidMethod(instance, method, cbool);
	}

	static void InitJNIMethods(JNIEnv *env, jclass cls)
	{
		g_app.m_Initialize = env->GetMethodID(cls, "initialize", "(Ljava/lang/String;Z)V");

		g_app.m_IsInterstitialLoaded = env->GetMethodID(cls, "isInterstitialLoaded", "()Z");
		g_app.m_ShowInterstitial = env->GetMethodID(cls, "showInterstitial", "(Ljava/lang/String;)V");

		g_app.m_IsRewardedLoaded = env->GetMethodID(cls, "isRewardedLoaded", "()Z");
		g_app.m_ShowRewarded = env->GetMethodID(cls, "showRewarded", "(Ljava/lang/String;)V");

		// g_app.m_SetUserConsent = env->GetMethodID(cls, "setUserConsent", "(Z)V");
	}

	void Initialize_Ext()
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv *env = threadAttacher.GetEnv();
		jclass cls = dmAndroid::LoadClass(env, "com.defold.extension.ExtensionAppodealAds");

		InitJNIMethods(env, cls);

		jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");

		g_app.m_AppJNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
	}

	void ActivateApp()
	{
	}

	void Initialize(const char *app_key, bool testing)
	{
		CallVoidMethodCharBool(g_app.m_AppJNI, g_app.m_Initialize, app_key, testing);
	}

	// ------------------------------------------------------------------------------------------

	bool IsInterstitialLoaded()
	{
		return CallBoolMethod(g_app.m_AppJNI, g_app.m_IsInterstitialLoaded);
	}

	void ShowInterstitial(const char *place)
	{
		CallVoidMethodChar(g_app.m_AppJNI, g_app.m_ShowInterstitial, place);
	}

	// ------------------------------------------------------------------------------------------

	bool IsRewardedLoaded()
	{
		return CallBoolMethod(g_app.m_AppJNI, g_app.m_IsRewardedLoaded);
	}

	void ShowRewarded(const char *place)
	{
		CallVoidMethodChar(g_app.m_AppJNI, g_app.m_ShowRewarded, place);
	}

	// void SetUserConsent(bool val)
	// {
	//     CallVoidMethodInt(g_app.m_AppJNI, g_app.m_SetUserConsent, val);
	// }
}

#endif
