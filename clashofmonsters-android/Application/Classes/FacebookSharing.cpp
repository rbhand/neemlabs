//
//  FacebookSharing.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 5/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "FacebookSharing.h"

#include <jni.h>
#include <android/log.h>
#include "AndroidNDKUtils.h"

//#include "W2FacebookSharing.h"

using namespace monsters;


namespace monsters
{
	class W2FacebookSharingPrivate
	{
	public:
		// Error parseError(NSError* objcError) const
		// {
		// 	if (!objcError)
		// 		return Error();
		// 	else
		// 		return Error([[objcError localizedDescription] UTF8String]);
		// }
		
	public:
		//W2FacebookSharing* facebookSharing_;
	};
}


static FacebookSharing* g_sharedSharing = 0x0;

static jobject g_activity;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_familyroomgames_piratesvsninjasdeluxe_PiratesVSNinjas_initFacebookSharing(JNIEnv *env,
    jclass cls, jobject activity)
{
    g_activity = env->NewGlobalRef(activity);
}

JNIEXPORT void JNICALL Java_com_familyroomgames_piratesvsninjasdeluxe_PiratesVSNinjas_facebookSharingDidFinish(JNIEnv *env,
    jclass cls, jint success, jstring errorString)
{
    const char *nativeString = env->GetStringUTFChars(errorString, 0);
    g_sharedSharing->getShareCallback().call(success ? true : false, success ? Error() : Error(nativeString));
    env->ReleaseStringUTFChars(errorString, nativeString);
}

#ifdef __cplusplus
}
#endif


FacebookSharing::FacebookSharing() :
	d_(new W2FacebookSharingPrivate)
{
    // d_->facebookSharing_ = [[W2FacebookSharing alloc] init];
	// 
	// #ifdef IS_PRO
	// 	d_->facebookSharing_.link = [NSURL URLWithString:@"https://itunes.apple.com/us/app/pirates-versus-ninjas-2-player/id667904125?ls=1&mt=8"];
	// #else
	// 	d_->facebookSharing_.link = [NSURL URLWithString:@"https://itunes.apple.com/us/app/pirates-versus-ninjas-2-player/id667901021?ls=1&mt=8"];
	// #endif
	// 
	// d_->facebookSharing_.picture = [NSURL URLWithString:@"https://s3.amazonaws.com/pirates-vs-ninjas/app-icon.png"];
	// d_->facebookSharing_.name = @"Pirates vs Ninjas";
	// d_->facebookSharing_.description = @"Check this amazing game!";
}

FacebookSharing::~FacebookSharing()
{
//	[d_->facebookSharing_ release];
	delete d_;
}

void FacebookSharing::facebookShare(const std::string& text, const ShareCallback& callback)
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
                "(PIRATES) facebookShare: failed to attach current thread");
            return;
        }
        isAttached = true;
    }

    jclass activity_class = env->GetObjectClass(g_activity);
    if (!activity_class )
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) facebookShare: failed to get class reference");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jmethodID method = env->GetMethodID(activity_class, "facebookShare", "(Ljava/lang/String;)V");
    if (!method)
    {
        __android_log_print(ANDROID_LOG_ERROR, "cocos2dx PiratesVSNinjas",
            "(PIRATES) facebookShare: failed to get method ID");
        if (isAttached)
            gJavaVM->DetachCurrentThread();
        return;
    }

    jstring j_text = env->NewStringUTF(text.c_str());


    shareCallback_ = callback;
    env->CallVoidMethod(g_activity, method, j_text);

    // ShareCallback share_callback = callback;
	// 
	// d_->facebookSharing_.description = [NSString stringWithUTF8String:text.c_str()];
	// [d_->facebookSharing_ shareWithCompletion:^(BOOL published, NSError *error) {
	// 	
	// 	share_callback.call(published ? true : false, d_->parseError(error));
	// 	
	// }];
}

void FacebookSharing::facebookInvite(const InviteCallback& callback)
{
	// InviteCallback invite_callback = callback;
	// 
	// [d_->facebookSharing_ inviteWithCompletion:^(BOOL published, NSError *error) {
	// 	
	// 	invite_callback.call(published ? true : false, d_->parseError(error));
	// 	
	// }];
}

FacebookSharing& FacebookSharing::sharedSharing()
{
	if (!g_sharedSharing)
	{
		g_sharedSharing = new FacebookSharing();
	}
	
	return *g_sharedSharing;
}


