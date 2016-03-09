#include "AndroidNDKUtils.h"
#include "platform/android/jni/JniHelper.h"

using namespace w2f2;
using namespace cocos2d;

JavaVM* AndroidNDKUtils::getJavaVM()
{
    return JniHelper::getJavaVM();
}
