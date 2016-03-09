//
//  TutorialViewController.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/15/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "cocos2d.h"

@interface TutorialViewController : UIViewController <UIScrollViewDelegate>
{
	IBOutlet UIButton* btnBack_;
	IBOutlet UIButton* btnNext_;
	
	IBOutlet UIScrollView* scrollView_;
	IBOutlet UIPageControl* pageControl_;
	
	//BOOL pageControlBeingUsed_;
}

- (IBAction)back;
- (IBAction)next;

- (IBAction)changePage;

@end
