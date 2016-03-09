//
//  GoogleAnalytics.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 7/3/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "GoogleAnalytics.h"
#include "cocos2d.h"
#include <cassert>
//#include "GAI.h"

#include <jni.h>
#include <android/log.h>
#include "AndroidNDKUtils.h"


static jobject g_activity;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_familyroomgames_piratesvsninjasdeluxe_PiratesVSNinjas_initGoogleAnalytics(JNIEnv *env,
    jclass cls, jobject activity)
{
    g_activity = env->NewGlobalRef(activity);
}

#ifdef __cplusplus
}
#endif


using namespace w2f2;

static GoogleAnalytics* g_GoogleAnalytics = 0;

namespace w2f2
{
	struct GoogleAnalyticsPrivate
	{
		//id<GAITracker> tracker;
	};
}


GoogleAnalytics::GoogleAnalytics(const std::string& trackingId) :
	d_(new GoogleAnalyticsPrivate)
{
	// [GAI sharedInstance].trackUncaughtExceptions = YES;
	// [GAI sharedInstance].dispatchInterval = 20;
	//   
	// d_->tracker = [[GAI sharedInstance] trackerWithTrackingId:[NSString
	// 	stringWithUTF8String:trackingId.c_str()]];
}

GoogleAnalytics::~GoogleAnalytics()
{
//	d_->tracker = nil;
	delete d_;
}

void GoogleAnalytics::trackScreenEnter(const std::string& screenName)
{
    int status;
    JNIEnv* env;
    JavaVM* gJavaVM = w2f2::AndroidNDKUtils::getJavaVM();
    bool isAttached = false;

    status = gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
    if(status < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) Failed to get JNI environment, assuming native thread");
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if (status < 0)
        {
            __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
                "(PIRATES) GoogleAnalytics: failed to attach current thread");
            return;
        }
        isAttached = true;
    }

    jclass activity_class = env->GetObjectClass(g_activity);
    if (!activity_class )
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) GoogleAnalytics: failed to get class reference");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jmethodID method = env->GetMethodID(activity_class, "trackScreenEnter", "(Ljava/lang/String;)V");
    if (!method)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) GoogleAnalytics: failed to get method ID");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jstring j_text = env->NewStringUTF(screenName.c_str());
    env->CallVoidMethod(g_activity, method, j_text);


//	[d_->tracker sendView:[NSString stringWithUTF8String:screenName.c_str()]];
}

void GoogleAnalytics::initialize(const std::string& trackingId)
{
	CC_ASSERT(!g_GoogleAnalytics);
	g_GoogleAnalytics = new GoogleAnalytics(trackingId);
}

GoogleAnalytics& GoogleAnalytics::instance()
{
	CC_ASSERT(g_GoogleAnalytics);
	return *g_GoogleAnalytics;
}
