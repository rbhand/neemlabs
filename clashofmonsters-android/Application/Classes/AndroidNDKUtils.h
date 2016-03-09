#pragma once

#include <jni.h>

namespace w2f2
{
    class AndroidNDKUtils
    {
    public:
        static JavaVM* getJavaVM();
    };
}