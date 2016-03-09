//
//  Utils.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include <string>

namespace cocos2d_utils
{
	std::string fileNameWithoutExtension(const std::string& baseName);
	std::string fileExtension(const std::string& baseName);
	std::string textureName(const std::string& fileName);
	std::string defaultPNGFileName();
	
	cocos2d::CCMenuItemSprite* menuItemSpriteHighlighted(const char* spriteFrameName,
		cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
		
	cocos2d::CCMenuItemSprite* menuItemSpriteHighlightedWithDisabledState(const char* spriteFrameName,
		cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

	cocos2d::CCMenuItemSprite* menuItemSpriteHighlightedAndFlipped(const char* spriteFrameName,
		cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
	
	void menuItemSpriteHighlightedChangeSprite(cocos2d::CCMenuItemSprite* itemSprite, const char* spriteFrameName);

	bool isFileExists(const std::string& fileName);
	
	float ct(float coordinate);
	cocos2d::CCPoint pt(const cocos2d::CCPoint& p);
	
	void showAlert(const char* title, const char* message, const char* cancelButton);
	std::string createWithFormat(const char* format, ...);
	
	bool isIPhone();
	bool isIPhone5();
	bool isRetina();
	
	cocos2d::CCRect intersectRectangles(const cocos2d::CCRect& rect1, const cocos2d::CCRect& rect2);
}
