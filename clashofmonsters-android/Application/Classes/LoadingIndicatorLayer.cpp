//
//  LoadingIndicatorLayer.cpp
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "LoadingIndicatorLayer.h"

using namespace monsters;
using namespace std;

bool LoadingIndicatorLayer::init()
{
	if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
	{
		return false;
	}
	
	loadingIndicator_ = CCSprite::createWithSpriteFrameName("ships_wheel.png");
	loadingIndicator_->setAnchorPoint(CCPoint(0.5, 0.5));
	loadingIndicator_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	addChild(loadingIndicator_);
	
	caption_ = CCLabelTTF::create("", "MyriadPro-Semibold", ct(20), CCSizeMake(ct(300), 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
	caption_->setColor(ccc3(52, 87, 113));
	caption_->setAnchorPoint(CCPoint(0.5, 1));
	caption_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	addChild(caption_);
	
	linkedNodes_ = 0;
	
	setVisible(false);
	
	return true;
}

void LoadingIndicatorLayer::setLinkedNodes(CCArray* linkedNodes)
{
	if (linkedNodes_)
		linkedNodes_->release();
		
	linkedNodes_ = linkedNodes;
	
	if (linkedNodes_)
		linkedNodes_->retain();
}

void LoadingIndicatorLayer::setCaption(const std::string& caption)
{
	caption_->setString(caption.c_str());
	
	loadingIndicator_->setPositionY(getContentSize().height * 0.5/* + (loadingIndicator_->getContentSize().height + caption_->getContentSize().height)/2*/);
	caption_->setPositionY(loadingIndicator_->getPositionY() - loadingIndicator_->getContentSize().height * 0.75);
}


void LoadingIndicatorLayer::show()
{
	if (isVisible())
		return;
	
	if (linkedNodes_)
		setLinkedNodesVisible(linkedNodes_, false);
	
	setVisible(true);
	loadingIndicator_->runAction(CCRepeatForever::create(CCRotateBy::create(3.2, 360)));
}


void LoadingIndicatorLayer::hide()
{
	if (!isVisible())
		return;
	
	if (linkedNodes_)
		setLinkedNodesVisible(linkedNodes_, true);

	setVisible(false);
	loadingIndicator_->stopAllActions();
}


void LoadingIndicatorLayer::setLinkedNodesVisible(CCArray* linkedNodes, bool visible)
{
	if (!visible)
	{
		CCObject* cc_object;
		CCARRAY_FOREACH(linkedNodes, cc_object)
		{
			if (CCNode* node = dynamic_cast<CCNode*>(cc_object))
			{
				if (node->isVisible())
				{
					node->setVisible(visible);
					nodesHidden_.push_back(node);
				}
			}
		}
	}
	else
	{
		for (list<CCNode*>::iterator i=nodesHidden_.begin(), e=nodesHidden_.end(); i!=e; ++i)
		{
			(*i)->setVisible(visible);
		}
		nodesHidden_.clear();
	}
}
