//
//  TutorialScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/15/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "TutorialScreen.h"
#import "AppController.h"
#import "TutorialViewController.h"
#import "GoogleAnalytics.h"

using namespace monsters;
using namespace cocos2d;

namespace monsters
{
	struct TutorialScreenPrivate
	{
		TutorialViewController* tutorialViewController_;
	};
}

TutorialScreen::TutorialScreen() :
	d_(new TutorialScreenPrivate)
{
	d_->tutorialViewController_ = [[TutorialViewController alloc] initWithNibName:@"TutorialViewController"
		bundle:nil];
}

TutorialScreen::~TutorialScreen()
{
	[d_->tutorialViewController_ release];
	delete d_;
}

void TutorialScreen::show()
{
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Tutorial");
	
	[[AppController sharedAppController].viewController presentModalViewController:d_->tutorialViewController_
		animated:YES];
	
	CCDirector::sharedDirector()->pause();
}
