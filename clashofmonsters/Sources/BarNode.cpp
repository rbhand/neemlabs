//
//  BarNode.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 7/1/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "BarNode.h"
#include "CCGL.h"
#include "CCEGLView.h"

using namespace monsters;
using namespace cocos2d;

namespace monsters
{
	class BarFilledNode : public cocos2d::CCNode
	{
	public:
		bool initWithSprite(cocos2d::CCSprite* filledSprite, GLbyte opacity = 255)
		{
			addChild(filledSprite);
			setContentSize(filledSprite->getContentSize());
			filledSprite->setAnchorPoint(CCPoint(0.5, 0.5));
			filledSprite->setPosition(CCPoint(getContentSize().width * 0.5,
				getContentSize().height * 0.5));
			filledSprite->setOpacity(opacity);
			return true;
		}
	
		void visit()
		{
			kmGLPushMatrix();

			glEnable(GL_SCISSOR_TEST);
			
			float scale = 1;//CCDirector::sharedDirector()->getContentScaleFactor();
			CCPoint origin = convertToWorldSpace(CCPoint(0, 0));
			
			switch (direction_)
			{
				case BarNodeDirection_Up:
					CCEGLView::sharedOpenGLView()->setScissorInPoints(origin.x * scale, origin.y * scale,
						getContentSize().width * scale, getContentSize().height * value_ * scale);
				break;
				
				case BarNodeDirection_Down:
					CCEGLView::sharedOpenGLView()->setScissorInPoints(origin.x * scale,
						(origin.y + getContentSize().height * (1.0 - value_)) * scale, getContentSize().width * scale,
						getContentSize().height * value_ * scale);
				break;
			}
			
			CCNode::visit();
			
			glDisable(GL_SCISSOR_TEST);
			kmGLPopMatrix();
		}
		
		float value_;
		BarNodeDirection direction_;
	};
}

bool BarNode::initWithSprites(cocos2d::CCSprite* backgroundSprite,
	cocos2d::CCSprite* filledSprite, GLbyte opacity)
{
	addChild(backgroundSprite);
	
	setContentSize(backgroundSprite->getContentSize());
	
	backgroundSprite->setAnchorPoint(CCPoint(0.5, 0.5));
	backgroundSprite->setPosition(CCPoint(getContentSize().width * 0.5,
		getContentSize().height * 0.5));
	backgroundSprite->setOpacity(opacity);
	
	filledNode_ = new BarFilledNode();
	filledNode_->initWithSprite(filledSprite, opacity);
	filledNode_->setAnchorPoint(CCPoint(0.5, 0.5));
	filledNode_->setPosition(CCPoint(getContentSize().width * 0.5,
		getContentSize().height * 0.5));
	filledNode_->autorelease();
	
	filledNode_->value_ = 0.1;
	
	addChild(filledNode_);
	
	return true;
}

BarNode* BarNode::barNodeWithSprites(cocos2d::CCSprite* backgroundSprite,
	cocos2d::CCSprite* filledSprite, GLbyte opacity)
{
	BarNode* n = new BarNode();
	n->initWithSprites(backgroundSprite, filledSprite, opacity);
	n->autorelease();
	return n;
}

void BarNode::setValue(float value)
{
	filledNode_->value_ = value;
}

float BarNode::value() const
{
	return filledNode_->value_;
}

void BarNode::setDirection(BarNodeDirection direction)
{
	filledNode_->direction_ = direction;
}

BarNodeDirection BarNode::direction() const
{
	return filledNode_->direction_;
}
