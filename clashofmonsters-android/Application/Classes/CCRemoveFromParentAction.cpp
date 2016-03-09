//
//  CCRemoveFromParentAction.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/2/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "CCRemoveFromParentAction.h"

NS_CC_BEGIN

CCRemoveFromParentAction* CCRemoveFromParentAction::create() {
    CCRemoveFromParentAction* pRet = new CCRemoveFromParentAction();

    if (pRet) {
        pRet->autorelease();
    }

    return pRet;
}

void CCRemoveFromParentAction::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->removeFromParentAndCleanup(true);
}

CCObject* CCRemoveFromParentAction::copyWithZone(CCZone *pZone) {

    CCZone *pNewZone = NULL;
    CCRemoveFromParentAction *pRet = NULL;
    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCRemoveFromParentAction*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCRemoveFromParentAction();
        pZone = pNewZone = new CCZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

NS_CC_END
