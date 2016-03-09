//
//  W2NibViewController.m
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import "W2NibViewController.h"

static W2NibViewController* g_lastActiveW2NibViewController;

@implementation W2NibViewController

@synthesize isActive = isActive_;

- (id)init
{
	if ((self = [super initWithNibName:NSStringFromClass([self class])
		bundle:nil]))
//		bundle:[NSBundle mainBundle]]))
	{
	}
	return self;
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	isActive_ = YES;
	g_lastActiveW2NibViewController = self;
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
	isActive_ = NO;
}

+ (W2NibViewController*)lastActiveViewController
{
	return g_lastActiveW2NibViewController;
}

@end
