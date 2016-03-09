//
//  W2NibViewController.h
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface W2NibViewController : UIViewController
{
	BOOL isActive_;
}

@property (readonly, nonatomic) BOOL isActive;

+ (W2NibViewController*)lastActiveViewController;

@end
