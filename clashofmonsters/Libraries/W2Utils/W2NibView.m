//
//  W2NibView.m
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import "W2NibView.h"

@implementation W2NibView

@synthesize contentView = contentView_;

- (id)initWithCoder:(NSCoder*)aDecoder
{
	if ((self = [super initWithCoder:aDecoder]))
	{
		[[NSBundle mainBundle] loadNibNamed:NSStringFromClass([self class])
			owner:self options:nil];
		
		self.autoresizingMask = self.contentView.autoresizingMask;
		self.bounds = self.contentView.bounds;
		[self addSubview:self.contentView];
		self.contentView.frame = self.bounds;
		
		self.backgroundColor = [UIColor clearColor];
		
		[self viewDidLoad];
	}
	
	return self;
}

- (id)init
{
	if ((self = [super init]))
	{
		//NSLog(@"%@", NSStringFromClass([self class]));
		
		[[NSBundle mainBundle] loadNibNamed:NSStringFromClass([self class])
			owner:self options:nil];
		
		self.autoresizingMask = self.contentView.autoresizingMask;
		self.bounds = self.contentView.bounds;
		[self addSubview:self.contentView];
		
		self.backgroundColor = [UIColor clearColor]; 
		
		callbacks_ = [[NSMutableDictionary alloc] init];
		
		[self viewDidLoad];
	}
	
	return self;
}

- (void)viewDidLoad
{
}
- (void)onAction:(NSString*)action invoke:(W2NibViewCallback)callback
{
	[callbacks_ setObject:callback forKey:action];
}

- (void)dispatchActionFromSelector:(SEL)selector sender:(id)sender
{
	[self dispatchAction:NSStringFromSelector(selector) sender:sender];
}

- (void)dispatchAction:(NSString*)action sender:(id)sender
{
	W2NibViewCallback callback = (W2NibViewCallback)[callbacks_ objectForKey:action];
	if (callback)
		callback(sender);
}

@end
