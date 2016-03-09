//
//  ChooseAvatarLayer.cpp
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/18/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "ChooseAvatarLayer.h"
#include "SoundManager.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;

bool ChooseAvatarLayer::init()
{
	if (!NavigationMenuLayer::init(NavigationMenuLayerLayout_Internal))
	{
		return false;
	}
	
	avatarButtonsBaseY_ = getContentSize().height * 0.45;
	leftAvatarType_ = ChooseAvatarArmyType_Pirates;
	rightAvatarType_ = ChooseAvatarArmyType_Ninjas;
	
	leftMenu_ = CCMenu::create();
	leftMenu_->setPosition(CCPoint(0, 0));
	
	rightMenu_ = CCMenu::create();
	rightMenu_->setPosition(CCPoint(0, 0));
	
	
	lb_title_ = CCLabelTTF::create(" CHOOSE YOUR AVATAR ", "BadaBoom BB", ct(22.0));
	lb_title_->setAnchorPoint(CCPoint(0.5, 1));
	lb_title_->setColor(ccc3(52, 87, 113));
	//lb_title_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(75)));
	lb_title_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(65)));
		
	
	leftButtonNinja_ = menuItemSpriteHighlighted("ninja_small.png", this,
		menu_selector(ChooseAvatarLayer::leftAvatarButtonPressed));
	leftButtonNinja_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	leftButtonNinja_->setAnchorPoint(CCPoint(0, 0.5));
	
	leftHighlightNinja_ = CCSprite::createWithSpriteFrameName("ninja_small_highlight.png");
	leftHighlightNinja_->setAnchorPoint(CCPoint(0.5, 0.5));
	leftHighlightNinja_->setPosition(CCPoint(leftButtonNinja_->getContentSize().width * 0.5, leftButtonNinja_->getContentSize().height * 0.5));
	leftButtonNinja_->addChild(leftHighlightNinja_, -1);
	leftMenu_->addChild(leftButtonNinja_, -10);
	
	
	rightButtonNinja_ = menuItemSpriteHighlightedAndFlipped("ninja_small.png", this,
		menu_selector(ChooseAvatarLayer::rightAvatarButtonPressed));
	rightButtonNinja_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	rightButtonNinja_->setAnchorPoint(CCPoint(1, 0.5));
	
	rightHighlightNinja_ = CCSprite::createWithSpriteFrameName("ninja_small_highlight.png");
	rightHighlightNinja_->setFlipX(true);
	rightHighlightNinja_->setAnchorPoint(CCPoint(0.5, 0.5));
	rightHighlightNinja_->setPosition(CCPoint(rightButtonNinja_->getContentSize().width * 0.5, rightButtonNinja_->getContentSize().height * 0.5));
	rightButtonNinja_->addChild(rightHighlightNinja_, -1);
	rightMenu_->addChild(rightButtonNinja_, -10);
	
	
	leftButtonPirate_ = menuItemSpriteHighlighted("pirate_small.png", this,
		menu_selector(ChooseAvatarLayer::leftAvatarButtonPressed));
	leftButtonPirate_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	leftButtonPirate_->setAnchorPoint(CCPoint(0, 0.5));
	
	leftHighlightPirate_ = CCSprite::createWithSpriteFrameName("pirate_small_highlight.png");
	leftHighlightPirate_->setAnchorPoint(CCPoint(0.5, 0.5));
	leftHighlightPirate_->setPosition(CCPoint(leftButtonPirate_->getContentSize().width * 0.5, leftButtonPirate_->getContentSize().height * 0.5));
	leftButtonPirate_->addChild(leftHighlightPirate_, -1);
	leftMenu_->addChild(leftButtonPirate_, -10);
	
	
	rightButtonPirate_ = menuItemSpriteHighlightedAndFlipped("pirate_small.png", this,
		menu_selector(ChooseAvatarLayer::rightAvatarButtonPressed));
	rightButtonPirate_->setPosition(CCPoint(0, getContentSize().height * 0.5));
	rightButtonPirate_->setAnchorPoint(CCPoint(1, 0.5));
	
	rightHighlightPirate_ = CCSprite::createWithSpriteFrameName("pirate_small_highlight.png");
	rightHighlightPirate_->setFlipX(true);
	rightHighlightPirate_->setAnchorPoint(CCPoint(0.5, 0.5));
	rightHighlightPirate_->setPosition(CCPoint(rightButtonPirate_->getContentSize().width * 0.5, rightButtonPirate_->getContentSize().height * 0.5));
	rightButtonPirate_->addChild(rightHighlightPirate_, -1);
	rightMenu_->addChild(rightButtonPirate_, -10);
	
	
	CCSprite* versus = CCSprite::createWithSpriteFrameName("vs.png");
	versus->setPosition(CCPoint(getContentSize().width * 0.5, avatarButtonsBaseY_));
	
	
	
	/*CCMenuItemSprite* btn_next = menuItemSpriteHighlighted("btn_next.png", this,
		menu_selector(ChooseAvatarLayer::next));
	btn_next->setPosition(CCPoint(getContentSize().width * 0.5, btn_next->getContentSize().height * 0.5));
	menu()->addChild(btn_next);*/
	btnNext()->setPosition(CCPoint(btnNext()->getPosition().x, btnNext()->getContentSize().height * 0.5));
	btnBack()->setPosition(CCPoint(btnBack()->getPosition().x, btnBack()->getContentSize().height * 0.5));
	
	
	addChild(lb_title_);
	addChild(leftMenu_, -10);
	addChild(rightMenu_, -10);
	addChild(versus);
	
	
	showButton(leftButtonPirate_, false, false);
	hideButton(leftButtonNinja_, false, false);
	hideButton(rightButtonPirate_, true, false);
	showButton(rightButtonNinja_, true, false);
	
	return true;
}

void ChooseAvatarLayer::visit()
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

void ChooseAvatarLayer::nextButtonPressed()
{
	delegate_->chooseAvatarLayerNext();
}

void ChooseAvatarLayer::backButtonPressed()
{
	delegate_->chooseAvatarLayerBack();
}

void ChooseAvatarLayer::onShow()
{
	leftMenu_->setTouchEnabled(false);
	rightMenu_->setTouchEnabled(false);
	menu()->setTouchEnabled(false);
	
	leftButtonNinja_->setVisible(true);
	leftButtonPirate_->setVisible(true);
	rightButtonNinja_->setVisible(true);
	rightButtonPirate_->setVisible(true);
}

void ChooseAvatarLayer::onShowDone()
{
	leftMenu_->setTouchEnabled(true);
	rightMenu_->setTouchEnabled(true);
	
	/*leftButtonNinja_->setVisible(true);
	leftButtonPirate_->setVisible(true);
	rightButtonNinja_->setVisible(true);
	rightButtonPirate_->setVisible(true);*/
}

void ChooseAvatarLayer::onHide()
{
	leftMenu_->setTouchEnabled(false);
	rightMenu_->setTouchEnabled(false);
}

void ChooseAvatarLayer::onHideDone()
{
	leftMenu_->setTouchEnabled(true);
	rightMenu_->setTouchEnabled(true);
	menu()->setTouchEnabled(true);
	
	leftButtonNinja_->setVisible(false);
	leftButtonPirate_->setVisible(false);
	rightButtonNinja_->setVisible(false);
	rightButtonPirate_->setVisible(false);
}



void ChooseAvatarLayer::setType(GameConfigurationScreenType type)
{
	gameType_ = type;
	
	switch (gameType_)
	{
		case GameConfigurationScreenType_SinglePlayer:
			setTitleText(" CHOOSE YOUR AVATAR ");
			
			rightButtonPirate_->setEnabled(true);
			rightButtonNinja_->setEnabled(true);
			rightHighlightPirate_->setVisible(true);
			rightHighlightNinja_->setVisible(true);
			break;
			
		case GameConfigurationScreenType_TwoPlayers:
			setTitleText(" CHOOSE YOUR AVATARS ");
			
			rightButtonPirate_->setEnabled(true);
			rightButtonNinja_->setEnabled(true);
			rightHighlightPirate_->setVisible(true);
			rightHighlightNinja_->setVisible(true);
			break;
		
		case GameConfigurationScreenType_OnlineGame:
			setTitleText(" CHOOSE YOUR AVATAR ");
			
			rightButtonPirate_->setEnabled(false);
			rightButtonNinja_->setEnabled(false);
			rightHighlightPirate_->setVisible(false);
			rightHighlightNinja_->setVisible(false);
			break;
	}
}

void ChooseAvatarLayer::setTitleText(const char* text)
{
	lb_title_->setString(text);
}



void ChooseAvatarLayer::disableLeftMenu(float mseconds)
{
	leftMenu_->setTouchEnabled(false);
	
	CCAction* enable_menu = CCSequence::createWithTwoActions(
		CCDelayTime::create(mseconds),
		CCCallFunc::create(this, callfunc_selector(ChooseAvatarLayer::enableLeftMenu))
	);
	
	this->runAction(enable_menu);
}

void ChooseAvatarLayer::enableLeftMenu()
{
	leftMenu_->setTouchEnabled(true);
}

void ChooseAvatarLayer::disableRightMenu(float mseconds)
{
	rightMenu_->setTouchEnabled(false);
	
	CCAction* enable_menu = CCSequence::createWithTwoActions(
		CCDelayTime::create(mseconds),
		CCCallFunc::create(this, callfunc_selector(ChooseAvatarLayer::enableRightMenu))
	);
	
	this->runAction(enable_menu);
}

void ChooseAvatarLayer::enableRightMenu()
{
	rightMenu_->setTouchEnabled(true);
}



void ChooseAvatarLayer::leftAvatarButtonPressed()
{
	disableMenu(0.35);
	disableLeftMenu(0.35);
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	leftAvatarType_ = (leftAvatarType_ == ChooseAvatarArmyType_Ninjas) ? ChooseAvatarArmyType_Pirates : ChooseAvatarArmyType_Ninjas;
	
	delegate_->chooseAvatarLayerSelectAvatar(leftAvatarType_, true);
	
	if (leftAvatarType_ == ChooseAvatarArmyType_Ninjas)
	{
		showButton(leftButtonNinja_, false, true);
		hideButton(leftButtonPirate_, false, true);
	}
	else
	{
		hideButton(leftButtonNinja_, false, true);
		showButton(leftButtonPirate_, false, true);
	}
}


void ChooseAvatarLayer::rightAvatarButtonPressed()
{
	disableMenu(0.35);
	disableRightMenu(0.35);
	SoundManager::sharedSoundManager()->playEffect("button_click");
	
	rightAvatarType_ = (rightAvatarType_ == ChooseAvatarArmyType_Ninjas) ? ChooseAvatarArmyType_Pirates : ChooseAvatarArmyType_Ninjas;
	
	delegate_->chooseAvatarLayerSelectAvatar(rightAvatarType_, false);
	
	if (rightAvatarType_ == ChooseAvatarArmyType_Ninjas)
	{
		showButton(rightButtonNinja_, true, true);
		hideButton(rightButtonPirate_, true, true);
	}
	else
	{
		hideButton(rightButtonNinja_, true, true);
		showButton(rightButtonPirate_, true, true);
	}
}


void ChooseAvatarLayer::showButton(CCMenuItemSprite* button, bool from_right_flag, bool animated)
{
	button->setVisible(true);
	
	if (animated)
	{
		if (from_right_flag)
		{
			CCAction* action = CCSequence::createWithTwoActions(
				CCDelayTime::create(0.15),
				CCMoveTo::create(0.15, CCPoint(getContentSize().width, avatarButtonsBaseY_))
			);
			button->runAction(action);
		}
		else
		{
			CCAction* action = CCSequence::createWithTwoActions(
				CCDelayTime::create(0.15),
				CCMoveTo::create(0.15, CCPoint(0, avatarButtonsBaseY_))
			);
			button->runAction(action);
		}
	}
	else
	{
		if (from_right_flag)
		{
			button->setPosition(CCPoint(getContentSize().width, avatarButtonsBaseY_));
		}
		else
		{
			button->setPosition(CCPoint(0, avatarButtonsBaseY_));
		}
	}
}

void ChooseAvatarLayer::hideButton(CCMenuItemSprite* button, bool from_right_flag, bool animated)
{
	if (animated)
	{
		if (from_right_flag)
		{
			CCAction* action = CCMoveTo::create(0.15, CCPoint(getContentSize().width + button->getContentSize().width, avatarButtonsBaseY_));
			button->runAction(action);
		}
		else
		{
			CCAction* action = CCMoveTo::create(0.15, CCPoint(-getContentSize().width, avatarButtonsBaseY_));
			button->runAction(action);
		}
	}
	else
	{
		if (from_right_flag)
		{
			button->setPosition(CCPoint(getContentSize().width + button->getContentSize().width, avatarButtonsBaseY_));
		}
		else
		{
			button->setPosition(CCPoint( - button->getContentSize().width, avatarButtonsBaseY_));
		}
	}
}

void ChooseAvatarLayer::setLeftAvatarType(ChooseAvatarArmyType type, bool animated)
{
	if (leftAvatarType_ != type || !animated)
	{
		leftAvatarType_ = type;
		if (type == ChooseAvatarArmyType_Ninjas)
		{
			showButton(leftButtonNinja_, false, animated);
			hideButton(leftButtonPirate_, false, animated);
		}
		else
		{
			hideButton(leftButtonNinja_, false, animated);
			showButton(leftButtonPirate_, false, animated);
		}
	}
}

void ChooseAvatarLayer::setRightAvatarType(ChooseAvatarArmyType type, bool animated)
{
	if (rightAvatarType_ != type || !animated)
	{
		rightAvatarType_ = type;
		if (type == ChooseAvatarArmyType_Ninjas)
		{
			showButton(rightButtonNinja_, true, animated);
			hideButton(rightButtonPirate_, true, animated);
		}
		else
		{
			hideButton(rightButtonNinja_, true, animated);
			showButton(rightButtonPirate_, true, animated);
		}
	}
}
