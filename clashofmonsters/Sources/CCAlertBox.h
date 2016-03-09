//
//  CCAlertBox.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/1/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

#include <string>

NS_CC_BEGIN

class CCAlertBox;
class ICCAlertBoxDelegate
{
public:
	virtual void alertBoxDidDismiss(CCAlertBox* alertBox, int buttonIndex) = 0;
};

class CCAlertBoxPrivate;
class CCAlertBox : public CCObject
{
public:
	CCAlertBox(const std::string& title, const std::string& message, const std::string& cancelButton,
		const std::string& otherButton, ICCAlertBoxDelegate* delegate = 0);
	~CCAlertBox();
	
	void show();
	
	int cancelButtonIndex() const;
	
	static void show(const std::string& title, const std::string& message, const std::string& cancelButton);
	
private:
	CCAlertBoxPrivate* d_;
};

NS_CC_END

