//
//  W2NibView.h
//
//  Created by Vitaliy Ivanov on 12/4/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void (^W2NibViewCallback)(id);
@interface W2NibView : UIView
{
	NSMutableDictionary* callbacks_;
}

@property (strong, nonatomic) IBOutlet UIView* contentView;

- (void)onAction:(NSString*)action invoke:(W2NibViewCallback)callback;

- (void)dispatchActionFromSelector:(SEL)selector sender:(id)sender;
- (void)dispatchAction:(NSString*)action sender:(id)sender;

- (void)viewDidLoad;

@end
