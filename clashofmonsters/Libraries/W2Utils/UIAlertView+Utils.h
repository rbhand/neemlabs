//
//  UIAlertView+Utils.h
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIAlertView (Utils)

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	cancelButton:(NSString*)cancelButton;
+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	delegate:(id)delegate button:(NSString*)button cancelButton:(NSString*)cancelButton;

+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	button:(NSString*)button cancelButton:(NSString*)cancelButton
	completion:(void (^)(NSInteger buttonIndex))completion;
+ (UIAlertView*)showWithTitle:(NSString*)title message:(NSString*)message
	cancelButton:(NSString*)cancelButton
	completion:(void (^)(NSInteger buttonIndex))completion;
	
+ (UIAlertView*)showWithMessage:(NSString*)message
	cancelButton:(NSString*)cancelButton;

@end
