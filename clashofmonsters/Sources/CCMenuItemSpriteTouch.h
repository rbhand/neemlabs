//
//  CCMenuItemSpriteTouch.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"

NS_CC_BEGIN

class CC_DLL CCMenuItemSpriteTouch : public CCMenuItemSprite
{
public:
    static CCMenuItemSpriteTouch * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
    
    static CCMenuItemSpriteTouch * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
    
    static CCMenuItemSpriteTouch * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);
    
    virtual void selected();
	void setTouchSelector(SEL_MenuHandler selector);
	
private:
	SEL_MenuHandler m_pfnTouchSelector;
};


NS_CC_END