//
//  TutorialScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/15/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "TutorialScreen.h"
/*#import "AppController.h"
#import "TutorialViewController.h"*/
#include "GoogleAnalytics.h"

#include <jni.h>
#include <android/log.h>
#include "AndroidNDKUtils.h"

using namespace monsters;
using namespace cocos2d;

static jobject g_activity;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_familyroomgames_piratesvsninjasdeluxe_PiratesVSNinjas_initTutorial(JNIEnv *env,
    jclass cls, jobject activity)
{
    g_activity = env->NewGlobalRef(activity);
}

#ifdef __cplusplus
}
#endif

namespace monsters
{
	struct TutorialScreenPrivate
	{
//		TutorialViewController* tutorialViewController_;
	};
}

TutorialScreen::TutorialScreen() :
	d_(new TutorialScreenPrivate)
{
//	d_->tutorialViewController_ = [[TutorialViewController alloc] initWithNibName:@"TutorialViewController"
//		bundle:nil];
}

TutorialScreen::~TutorialScreen()
{
//	[d_->tutorialViewController_ release];
	delete d_;
}

void TutorialScreen::show()
{
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Tutorial");

	int status;
    JNIEnv* env;
    JavaVM* gJavaVM = w2f2::AndroidNDKUtils::getJavaVM();
    bool isAttached = false;

    status = gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4);
    if (status < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) Failed to get JNI environment, assuming native thread");
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if (status < 0)
        {
            __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
                "(PIRATES) Tutorial: failed to attach current thread");
            return;
        }
        isAttached = true;
    }

    jclass activity_class = env->GetObjectClass(g_activity);
    if (!activity_class )
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) Tutorial: failed to get class reference");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jmethodID method = env->GetMethodID(activity_class, "showTutorial", "()V");
    if (!method)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) GoogleAnalytics: failed to get method ID");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    env->CallVoidMethod(g_activity, method);

//	[[AppController sharedAppController].viewController presentModalViewController:d_->tutorialViewController_
//		animated:YES];
//	CCDirector::sharedDirector()->pause();
}
