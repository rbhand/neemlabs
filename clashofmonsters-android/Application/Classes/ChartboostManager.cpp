//
//  ChartboostManager.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 6/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "ChartboostManager.h"
//#include "Chartboost.h"

#include <jni.h>
#include <android/log.h>
#include "AndroidNDKUtils.h"


static jobject g_activity;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_familyroomgames_piratesvsninjasdeluxe_PiratesVSNinjas_initChartboost(JNIEnv *env,
    jclass cls, jobject activity)
{
    g_activity = env->NewGlobalRef(activity);
}

#ifdef __cplusplus
}
#endif


using namespace monsters;

static ChartboostManager* g_sharedManager = 0x0;

ChartboostManager& ChartboostManager::sharedManager()
{
	if (!g_sharedManager)
	{
		g_sharedManager = new ChartboostManager();


		
		// #ifndef IS_PRO
		// Chartboost *cb = [Chartboost sharedChartboost];
		// cb.appId = @"51cd6c4216ba47a50b00001f";
		// cb.appSignature = @"879b5dc8ca1d729bc1e876992b8c6b90fc8098bc";
		// [cb startSession];
		// [cb cacheInterstitial];
		// #endif
	}
	
	return *g_sharedManager;
}

void ChartboostManager::showInterstitial()
{
	/*#ifndef IS_PRO

    int status;
    JNIEnv* env;
    JavaVM* gJavaVM = w2f2::AndroidNDKUtils::getJavaVM();
    bool isAttached = false;

    status = gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
    if(status < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) Failed to get JNI environment, assuming native thread");
        status = gJavaVM->AttachCurrentThread(&env, 0);
        if (status < 0)
        {
            __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
                "(PIRATES) chartboost: failed to attach current thread");
            return;
        }
        isAttached = true;
    }

    jclass activity_class = env->GetObjectClass(g_activity);
    if (!activity_class )
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) chartboost: failed to get class reference");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jmethodID method = env->GetMethodID(activity_class, "showInterstitial", "()V");
    if (!method)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) chartboost: failed to get method ID");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    env->CallVoidMethod(g_activity, method);


	// if ([Chartboost.sharedChartboost hasCachedInterstitial])
	// 	[Chartboost.sharedChartboost showInterstitial];
	// else
	// 	[Chartboost.sharedChartboost cacheInterstitial];

	#endif   */
}