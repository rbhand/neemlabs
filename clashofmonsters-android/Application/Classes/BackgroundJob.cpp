//
//  BackgroundJob.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 11/13/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "BackgroundJob.h"
#include "cocos2d.h"
#include <pthread.h>

using namespace f17y::utils;
using namespace cocos2d;

namespace f17y
{
namespace utils
{
    struct BackgroundJobPrivate
    {
        pthread_t thread_;
        static void* run(void* ptr)
        {
            BackgroundJob* job = (BackgroundJob*)ptr;
            job->main();

            job->d_->setIsRunning(false);

            return 0;
        }

    public:
        BackgroundJobPrivate() :
            isRunning_(false)
        {
            pthread_mutex_init(&mutexRunning_, 0);
        }

        bool isRunning() const
        {
            bool f;
            pthread_mutex_lock(&mutexRunning_);
            f = isRunning_;
            pthread_mutex_unlock(&mutexRunning_);
            return f;
        }

        void setIsRunning(bool running)
        {
            pthread_mutex_lock(&mutexRunning_);
            isRunning_ = running;
            pthread_mutex_unlock(&mutexRunning_);
        }

    private:
        volatile bool isRunning_;
        mutable pthread_mutex_t mutexRunning_;
    };
}
}

BackgroundJob::BackgroundJob() : delegate_(0), d_(new BackgroundJobPrivate)
{

}

BackgroundJob::~BackgroundJob()
{
	delete d_;
}

void BackgroundJob::checkForCompletion()
{
    if (!d_->isRunning())
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(BackgroundJob::checkForCompletion),
            this);
        if (delegate_)
        	delegate_->onBackgroundJobCompleted(this);
    }
}

void BackgroundJob::run()
{
    d_->setIsRunning(true);
    pthread_create(&d_->thread_, 0, &BackgroundJobPrivate::run, (void*)this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(BackgroundJob::checkForCompletion), this, 0, false);
}
