//
//  UIAlertView+Utils.m
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import "UIAlertView+Utils.h"

@interface UIAlertViewBlock : UIAlertView <UIAlertViewDelegate>

- (id)initWithTitle:(NSString *)title message:(NSString *)message
	completion:(void (^)(NSInteger buttonIndex))completion cancelButtonTitle:(NSString *)cancelButtonTitle
	otherButtonTitles:(NSString *)otherButtonTitles, ... NS_REQUIRES_NIL_TERMINATION;

@property (copy, nonatomic) void (^completion)(NSInteger);

@end


@implementation UIAlertView (Utils)

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	cancelButton:(NSString*)cancelButton
{
	UIAlertView* alert = [[UIAlertView alloc] initWithTitle:title message:message
		delegate:nil cancelButtonTitle:cancelButton
		otherButtonTitles:nil];
	[alert show];
	return alert;
}

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	delegate:(id)delegate button:(NSString*)button cancelButton:(NSString*)cancelButton
{
	UIAlertView* alert = [[UIAlertView alloc] initWithTitle:title message:message
		delegate:delegate cancelButtonTitle:cancelButton
		otherButtonTitles:button, nil];
	[alert show];
	return alert;
}

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	button:(NSString*)button cancelButton:(NSString*)cancelButton
	completion:(void (^)(NSInteger buttonIndex))completion
{
	UIAlertViewBlock* alert = [[UIAlertViewBlock alloc] initWithTitle:title message:message
		completion:completion cancelButtonTitle:cancelButton
		otherButtonTitles:button, nil];
	[alert show];
	return alert;
}

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	cancelButton:(NSString*)cancelButton
	completion:(void (^)(NSInteger buttonIndex))completion
{
	UIAlertViewBlock* alert = [[UIAlertViewBlock alloc] initWithTitle:title message:message
		completion:completion cancelButtonTitle:cancelButton
		otherButtonTitles:nil];
	[alert show];
	return alert;
}


+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	delegate:(id)delegate cancelButton:(NSString*)cancelButton
{
	UIAlertView* alert = [[UIAlertView alloc] initWithTitle:title message:message
		delegate:delegate cancelButtonTitle:cancelButton
		otherButtonTitles:nil];
	[alert show];
	return alert;
}

+ (UIAlertView*)showWithMessage:(NSString*)message
	cancelButton:(NSString*)cancelButton
{
	UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:message
		delegate:nil cancelButtonTitle:cancelButton
		otherButtonTitles:nil];
	[alert show];
	return alert;
}

@end


@implementation UIAlertViewBlock

@synthesize completion=_completion;

- (id)initWithTitle:(NSString *)title message:(NSString *)message completion:(void (^)(NSInteger buttonIndex))completion
	cancelButtonTitle:(NSString *)cancelButtonTitle otherButtonTitles:(NSString *)otherButtonTitles, ...
{
	self.completion = completion;
	return [super initWithTitle:title message:message delegate:self cancelButtonTitle:cancelButtonTitle
		otherButtonTitles:otherButtonTitles, nil];
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	self.completion(buttonIndex);
}

@end
