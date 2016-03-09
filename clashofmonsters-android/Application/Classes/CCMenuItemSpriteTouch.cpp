//
//  CCMenuItemSpriteTouch.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/13/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "CCMenuItemSpriteTouch.h"

NS_CC_BEGIN

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;


CCMenuItemSpriteTouch * CCMenuItemSpriteTouch::create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite)
{
    return CCMenuItemSpriteTouch::create(normalSprite, selectedSprite, disabledSprite, NULL, NULL);
}

CCMenuItemSpriteTouch * CCMenuItemSpriteTouch::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
    return CCMenuItemSpriteTouch::create(normalSprite, selectedSprite, NULL, target, selector);
}

CCMenuItemSpriteTouch * CCMenuItemSpriteTouch::create(CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    CCMenuItemSpriteTouch *pRet = new CCMenuItemSpriteTouch();
    pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector); 
    pRet->autorelease();
    return pRet;
}

bool CCMenuItemSpriteTouch::initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItem::initWithTarget(target, selector); 
    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);

    if(m_pNormalImage)
    {
        this->setContentSize(m_pNormalImage->getContentSize());
    }
    return true;
}


void CCMenuItemSpriteTouch::selected()
{
    CCMenuItem::selected();
	
	if (m_pListener && m_pfnTouchSelector)
	{
		(m_pListener->*m_pfnTouchSelector)(this);
	}

    if (m_pNormalImage)
    {
        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }

        if (m_pSelectedImage)
        {
            m_pNormalImage->setVisible(false);
            m_pSelectedImage->setVisible(true);
        }
        else
        {
            m_pNormalImage->setVisible(true);
        }
    }
}

void CCMenuItemSpriteTouch::setTouchSelector(SEL_MenuHandler selector)
{
	m_pfnTouchSelector = selector;
}

NS_CC_END