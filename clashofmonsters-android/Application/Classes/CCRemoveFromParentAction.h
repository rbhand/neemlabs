//
//  CCRemoveFromParentAction.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/2/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"

NS_CC_BEGIN

class CCRemoveFromParentAction : public CCActionInstant
{
public:
    CCRemoveFromParentAction() {}
    virtual ~CCRemoveFromParentAction() {}
	
    //super methods
    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone *pZone);

public:
    //override static method
    /** Allocates and initializes the action */
    static CCRemoveFromParentAction * create();
};

NS_CC_END
