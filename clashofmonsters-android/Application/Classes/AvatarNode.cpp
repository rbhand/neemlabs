//
//  AvatarNode.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/27/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "AvatarNode.h"
#include "Error.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



AvatarImageNode* AvatarImageNode::create(AvatarNodeSize size)
{
	AvatarImageNode* avatar_image_node = new AvatarImageNode();
    if (avatar_image_node && avatar_image_node->init(size))
    {
        avatar_image_node->autorelease();
        return avatar_image_node;
    }
	
    CC_SAFE_DELETE(avatar_image_node);
    return NULL;
}

bool AvatarImageNode::init(AvatarNodeSize size)
{
	avatarImageSize_ = (size == AvatarNodeSize_Small) ? kAvatarImageNodeSize_Small : kAvatarImageNodeSize_Large;
	
	setContentSize(CCSizeMake(avatarImageSize_, avatarImageSize_));
	
	return true;
}

void AvatarImageNode::visit()
{
	GLint scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, &scissor_box[0]);
	GLboolean was_scissors_enabled = glIsEnabled(GL_SCISSOR_TEST);
	
	kmGLPushMatrix();
	glEnable(GL_SCISSOR_TEST);
	
	float scale = getScale();
	CCPoint origin = convertToWorldSpace(CCPoint(0, 0));
	
	CCRect clip = CCRectMake(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
	
	//cout << endl << "width: " << getContentSize().width << " height: " << getContentSize().height << endl;
	
	CCEGLView::sharedOpenGLView()->setScissorInPoints(origin.x * scale, origin.y * scale,
		getContentSize().width * scale, getContentSize().height * scale);
	
	GLint scissor_box_2[4];
    glGetIntegerv(GL_SCISSOR_BOX, &scissor_box_2[0]);
	
	CCRect clip2 = CCRectMake(scissor_box_2[0], scissor_box_2[1], scissor_box_2[2], scissor_box_2[3]);
	
	CCRect clip3 = was_scissors_enabled ? intersectRectangles(clip, clip2) : clip2;
	
	glScissor(clip3.origin.x, clip3.origin.y, clip3.size.width, clip3.size.height);
	
	
	CCNode::visit();
	
	if (!was_scissors_enabled)
		glDisable(GL_SCISSOR_TEST);
	glScissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
	
	kmGLPopMatrix();

}

void AvatarImageNode::setAvatarUrl(const std::string& avatarUrl)
{
	avatarUrl_ = avatarUrl;
	setAvatarType(AvatarNodeType_Url);
//	Session::sharedSession().avatarsCache().load(avatarUrl, AvatarsCache::LoadCallback(this, (AvatarsCache::LoadSelector)&AvatarImageNode::onAvatarImageLoaded));
}

void AvatarImageNode::onAvatarImageLoaded(const std::string& url, cocos2d::CCSpriteFrame* spriteFrame, const Error& error)
{
	if (error.isOk() && avatarUrl_ == url)
	{
		if (avatarImage_)
			avatarImage_->removeFromParentAndCleanup(true);
		
		avatarImage_ = CCSprite::createWithSpriteFrame(spriteFrame);
		avatarImage_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
		
		//scale image if needed
		if ((avatarImage_->getContentSize().width > avatarImageSize_ || avatarImage_->getContentSize().height > avatarImageSize_)
			&& getContentSize().width && getContentSize().height)
		{
			//cout << endl << url << endl;
			//cout << endl << "width: " << avatarImage_->getContentSize().width << " height: " << avatarImage_->getContentSize().height << endl;
			
			if (avatarImage_->getContentSize().width < avatarImage_->getContentSize().height)
			{
				avatarImage_->setScale(avatarImageSize_ / avatarImage_->getContentSize().width);
			}
			else
			{
				
				avatarImage_->setScale(avatarImageSize_ / avatarImage_->getContentSize().height);
			}
		}
		
		addChild(avatarImage_);
		setContentSize(CCSizeMake(avatarImageSize_, avatarImageSize_));
	}
}

void AvatarImageNode::setAvatarType(AvatarNodeType type)
{
	if (avatarImage_)
		avatarImage_->removeFromParentAndCleanup(true);
	
	switch (type)
	{
		case AvatarNodeType_None:
		avatarImage_ = CCSprite::createWithSpriteFrameName("player_avatar.png");
		break;
		
		case AvatarNodeType_1stPlayer:
		avatarImage_ = CCSprite::createWithSpriteFrameName("player1_avatar.png");
		break;
		
		case AvatarNodeType_2ndPlayer:
		avatarImage_ = CCSprite::createWithSpriteFrameName("player2_avatar.png");
		break;
		
		case AvatarNodeType_AI:
		avatarImage_ = CCSprite::createWithSpriteFrameName("comp_avatar.png");
		break;
		
		case AvatarNodeType_Url:
		avatarImage_ = CCSprite::createWithSpriteFrameName("player_avatar.png");
		break;
	}
	
	avatarImage_->setAnchorPoint(CCPoint(0.5, 0.5));
	avatarImage_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	avatarImage_->setScale(avatarImageSize_ / avatarImage_->getContentSize().width);
	addChild(avatarImage_);
}



AvatarNode* AvatarNode::create(AvatarNodeSize size)
{
	AvatarNode* avatar_node = new AvatarNode();
    if (avatar_node && avatar_node->init(size))
    {
        avatar_node->autorelease();
        return avatar_node;
    }
	
    CC_SAFE_DELETE(avatar_node);
    return NULL;
}

bool AvatarNode::init(AvatarNodeSize size)
{
	if (size == AvatarNodeSize_Small)
		setContentSize(CCSizeMake(kAvatarNodeSize_Small, kAvatarNodeSize_Small));
	else
		setContentSize(CCSizeMake(kAvatarNodeSize_Large, kAvatarNodeSize_Large));
	
	bg_ = CCSprite::createWithSpriteFrameName((size == AvatarNodeSize_Small) ? "bg_player_avatar.png" : "bg_player_avatar_large.png");
	bg_->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	avatarImage_ = AvatarImageNode::create(size);
	avatarImage_->setAnchorPoint(CCPoint(0.5, 0.5));
	avatarImage_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	
	addChild(bg_, 10);
	addChild(avatarImage_, 20);
	
	return true;
}

void AvatarNode::setAvatarUrl(const std::string& avatarUrl)
{
	avatarImage_->setAvatarUrl(avatarUrl);
}

void AvatarNode::setAvatarType(AvatarNodeType type)
{
	avatarImage_->setAvatarType(type);
}





