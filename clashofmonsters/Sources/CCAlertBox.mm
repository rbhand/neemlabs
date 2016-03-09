//
//  CCAlertBox.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "CCAlertBox.h"

using namespace cocos2d;

@interface CCAlertBoxPrivate_objc : NSObject <UIAlertViewDelegate>
{
	CCAlertBoxPrivate* parent_;
}

@end

NS_CC_BEGIN

class CCAlertBoxPrivate
{
public:
	CCAlertBox* parent_;
	CCAlertBoxPrivate_objc* objc_;
	UIAlertView* alertView_;
	ICCAlertBoxDelegate* delegate_;
};

NS_CC_END


@implementation CCAlertBoxPrivate_objc

- (id)initWithParent:(CCAlertBoxPrivate*)parent
{
	if ((self = [super init]))
	{
		parent_ = parent;
	}
	
	return self;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	//parent_->autorelease();
	if (parent_->delegate_)
		parent_->delegate_->alertBoxDidDismiss(parent_->parent_, buttonIndex);
}

@end


NS_CC_BEGIN


CCAlertBox::CCAlertBox(const std::string& title, const std::string& message, const std::string& cancelButton,
		const std::string& otherButton, ICCAlertBoxDelegate* delegate)
{
	d_ = new CCAlertBoxPrivate;
	d_->parent_ = this;
	d_->delegate_ = delegate;
	
	d_->objc_ = [[CCAlertBoxPrivate_objc alloc] initWithParent:d_];
	d_->alertView_ = [[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title.c_str()]
		message:[NSString stringWithUTF8String:message.c_str()]
		delegate:d_->objc_
		cancelButtonTitle:cancelButton.size() ? [NSString stringWithUTF8String:cancelButton.c_str()] : nil
		otherButtonTitles:otherButton.size() ? [NSString stringWithUTF8String:otherButton.c_str()] : nil, nil];
}

void CCAlertBox::show(const std::string& title, const std::string& message, const std::string& cancelButton)
{
	[[[[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title.c_str()]
		message:[NSString stringWithUTF8String:message.c_str()]
		delegate:nil
		cancelButtonTitle:[NSString stringWithUTF8String:cancelButton.c_str()]
		otherButtonTitles:nil] autorelease] show];
}

CCAlertBox::~CCAlertBox()
{
	[d_->alertView_ release];
	delete d_;
}

int CCAlertBox::cancelButtonIndex() const
{
	return d_->alertView_.cancelButtonIndex;
}
	
void CCAlertBox::show()
{
	[d_->alertView_ show];
}

NS_CC_END
