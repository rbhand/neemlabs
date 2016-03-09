//
//  GameOverScreen.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/19/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "GameOverScreen.h"
#include "SoundManager.h"
#include "Session.h"
#include "CCAlertBox.h"
#include "ChartboostManager.h"
#include "GoogleAnalytics.h"

using namespace std;
using namespace cocos2d;
using namespace monsters;
using namespace cocos2d_utils;



bool GameOverScreen::init()
{
	if (!CCScene::init())
	{
		return false;
	}
	
	bg_ = CCLayer::create();
	
	menu_ = CCMenu::create();
	menu_->setPosition(CCPoint(0, 0));
	
	CCSprite* bg_menu = CCSprite::create(cocos2d_utils::textureName("Textures/Menu-Background.jpg").c_str());
	bg_menu->setScale(getContentSize().height / bg_menu->getContentSize().height);
	bg_menu->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_menu->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->addChild(bg_menu);
	
	CCSprite* logo = CCSprite::createWithSpriteFrameName("lb_winner.png");
	logo->setAnchorPoint(CCPoint(0.5, 1));
	logo->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(20)));
	bg_->addChild(logo);
	
	
	outPoints_ = CCLabelBMFont::create(createWithFormat("%d", 20000).c_str(), cocos2d_utils::textureName("Fonts/badaboom_points.fnt").c_str());
	outPoints_->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->addChild(outPoints_);
	
	lbPoints_ = CCLabelBMFont::create(createWithFormat(" XP ", 0).c_str(), cocos2d_utils::textureName("Fonts/badaboom_points.fnt").c_str());
	lbPoints_->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->addChild(lbPoints_);
	
	outPoints_->setPosition(CCPoint(getContentSize().width * 0.5 - lbPoints_->getContentSize().width * 0.5, getContentSize().height * 0.6));
	lbPoints_->setPosition(CCPoint(getContentSize().width * 0.5 + outPoints_->getContentSize().width * 0.5, getContentSize().height * 0.6));
	
	
	lbEarned_ = CCLabelTTF::create(" YOU EARNED ", "BadaBoom BB", ct(22.0));
	lbEarned_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbEarned_->setColor(ccc3(52, 87, 113));
	lbEarned_->setPosition(CCPoint(getContentSize().width * 0.5, outPoints_->getPosition().y + outPoints_->getContentSize().height * 0.75));
	bg_->addChild(lbEarned_);
	
	lbTitle_ = CCLabelTTF::create(" WINNER NAME ", "BadaBoom BB", ct(34.0));
	lbTitle_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbTitle_->setColor(ccc3(52, 87, 113));
	lbTitle_->setPosition(CCPoint(getContentSize().width * 0.5, lbEarned_->getPosition().y + lbEarned_->getContentSize().height * 1.5));
	bg_->addChild(lbTitle_);
	
	
	lbLoss_ = CCSprite::createWithSpriteFrameName("lb_you_lost.png");
	lbLoss_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbLoss_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_->addChild(lbLoss_);
	
	btnMenu_ = menuItemSpriteHighlighted("btn_menu_2.png", this,
		menu_selector(GameOverScreen::menu));
	btnMenu_->setPosition(CCPoint(getContentSize().width * 0.5 - btnMenu_->getContentSize().width * 0.5 - ct(8), btnMenu_->getContentSize().height));
	menu_->addChild(btnMenu_);
	
	//btnReplay_ = menuItemSpriteHighlighted("btn_replay.png", this,
	btnReplay_ = menuItemSpriteHighlighted("btn_play_again.png", this,
		menu_selector(GameOverScreen::replay));
	btnReplay_->setPosition(CCPoint(getContentSize().width * 0.5 + btnReplay_->getContentSize().width * 0.5 + ct(-5), btnReplay_->getContentSize().height));
	menu_->addChild(btnReplay_);
	
	outAccumulatedPoints_ = CCLabelBMFont::create(createWithFormat("%d", 20000).c_str(), cocos2d_utils::textureName("Fonts/badaboom_points.fnt").c_str());
	outAccumulatedPoints_->setAnchorPoint(CCPoint(1, 0.5));
	outAccumulatedPoints_->setScale(0.7);
	bg_->addChild(outAccumulatedPoints_);
	
	lbAccumulatedPoints_ = CCLabelTTF::create(" ACCUMULATED POINTS SO FAR! ",
		"BadaBoom BB", ct(18.0), CCSize(ct(120), 0), kCCTextAlignmentCenter);
	lbAccumulatedPoints_->setAnchorPoint(CCPoint(0, 0.5));
	lbAccumulatedPoints_->setColor(ccc3(52, 87, 113));
	bg_->addChild(lbAccumulatedPoints_);
	
	lbLoginIntoFacebook_ = CCLabelTTF::create(" YOUR EXPERIENCE POINTS ONLY ACCUMULATE IF YOU LOG IN TO FACEBOOK. PLEASE LOG INTO FACEBOOK. ",
		"BadaBoom BB", ct(18.0), CCSize(ct(310), 0), kCCTextAlignmentCenter);
	lbLoginIntoFacebook_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbLoginIntoFacebook_->setColor(ccc3(52, 87, 113));
	//bg_->addChild(lbLoginIntoFacebook_);
	
	lbShareToFacebook_ = CCLabelTTF::create(" POST YOUR SCORE AND GET ", "BadaBoom BB", ct(18.0));
	lbShareToFacebook_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbShareToFacebook_->setColor(ccc3(52, 87, 113));
	bg_->addChild(lbShareToFacebook_);
	
	lb5000XP_ = CCSprite::createWithSpriteFrameName("lb_5000xp.png");
	lb5000XP_->setAnchorPoint(CCPoint(0.5, 0.5));
	bg_->addChild(lb5000XP_);
	
	btnPostToFacebook_ = menuItemSpriteHighlighted("btn_post_to_facebook.png", this,
		menu_selector(GameOverScreen::facebook));
	btnPostToFacebook_->setAnchorPoint(CCPoint(0.5, 0.5));
	menu_->addChild(btnPostToFacebook_);
	
	outAccumulatedPoints_->setPosition(CCPoint(getContentSize().width * 0.5 + ct(15), outPoints_->getPosition().y - lbAccumulatedPoints_->getContentSize().height * 1.5 ));
	lbAccumulatedPoints_->setPosition(CCPoint(getContentSize().width * 0.5 + ct(15), outPoints_->getPosition().y - lbAccumulatedPoints_->getContentSize().height * 1.5 ));
	
	lbLoginIntoFacebook_->setPosition(CCPoint(getContentSize().width * 0.5, (btnMenu_->getPosition().y + outPoints_->getPosition().y) / 2 ));
	lb5000XP_->setPosition(CCPoint(getContentSize().width * 0.5, (btnMenu_->getPosition().y + outPoints_->getPosition().y) / 2.2 - btnPostToFacebook_->getContentSize().height * 0.25));
	btnPostToFacebook_->setPosition(CCPoint(getContentSize().width * 0.5,
		lb5000XP_->getPosition().y - btnPostToFacebook_->getContentSize().height - lb5000XP_->getContentSize().height * 0.0 ));
	lbShareToFacebook_->setPosition(CCPoint(getContentSize().width * 0.5,
		lb5000XP_->getPosition().y + lbShareToFacebook_->getContentSize().height + lb5000XP_->getContentSize().height * 0.25 ));
	
	
	lbUnlocked_ = CCLabelTTF::create(" YOU HAVE UNLOCKED ", "BadaBoom BB", ct(24.0));
	lbUnlocked_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbUnlocked_->setColor(ccc3(52, 87, 113));
	lbUnlocked_->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.6));
	bg_->addChild(lbUnlocked_);
	
	lbUnlockedUnitName_ = CCLabelTTF::create(" MEGA OCTOPUS NAPRIMER ", "BadaBoom BB", ct(24.0));
	lbUnlockedUnitName_->setAnchorPoint(CCPoint(0.5, 0.5));
	lbUnlockedUnitName_->setColor(ccc3(52, 87, 113));
	lbUnlockedUnitName_->setPosition(CCPoint(lbUnlocked_->getPositionX() + lbUnlocked_->getContentSize().width * 0.5,
		lbUnlocked_->getPositionY() + lbUnlocked_->getContentSize().height));
	bg_->addChild(lbUnlockedUnitName_);
	
	
	addChild(bg_);
	addChild(menu_);
	
	
	return true;
}


void GameOverScreen::setGame(Game* game)
{
	game_ = game;
}

//void GameOverScreen::setOnlineGame(OnlineGame* onlineGame)
//{
//	onlineGame_ = onlineGame;
//}

void GameOverScreen::hideUnlockedUnitsHUD()
{
	if (iconUnlockedUnit_)
	{
		iconUnlockedUnit_->setVisible(false);
		iconUnlockedUnit_->setOpacity(0);
	}
		
	lbUnlocked_->setVisible(false);
	lbUnlockedUnitName_->setVisible(false);
	
	lbUnlocked_->setOpacity(0);
	lbUnlockedUnitName_->setOpacity(0);
	
	lbEarned_->setOpacity(255);
	outPoints_->setOpacity(255);
	lbPoints_->setOpacity(255);
	outAccumulatedPoints_->setOpacity(255);
	lbAccumulatedPoints_->setOpacity(255);
}

void GameOverScreen::onEnter()
{
	CCScene::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Game Over");
	
	//adding points
	if (game_->type() == GameType_Local &&
		game_->player(game_->winnerIndex()).type() != PlayerType_AI)
	{
		//add xp points for 1st player
		Session::sharedSession().addExperiencePoints(game_->playerPoints(0));
	}
//	else if (game_->winnerIndex() == onlineGame_->userIndex())
//	{
//		//add xp points for winner
//		Session::sharedSession().addExperiencePoints(game_->playerPoints(onlineGame_->userIndex()));
//	}
	
	
	SoundManager::sharedSoundManager()->stopBackgroundMusic();
	
	//hide fb button
	lbShareToFacebook_->setVisible(false);
	lbAccumulatedPoints_->setVisible(false);
	outAccumulatedPoints_->setVisible(false);
	lb5000XP_->setVisible(false);
	btnPostToFacebook_->setVisible(false);
	facebookButtonWasShown_ = false;
	
	this->hideUnlockedUnitsHUD();
	
	if (game_->type() == GameType_Local)
	{
		btnReplay_->setVisible(true);
	
		//show message for 1st player only
		//lbLoginIntoFacebook_->setVisible(!Session::sharedSession().isFacebookLoggedIn() && game_->winnerIndex() == 0);
		
		if ( game_->player(game_->winnerIndex()).type() == PlayerType_AI )
		{
			lbTitle_->setString(" AI ");
			lbEarned_->setVisible(false);
			outPoints_->setVisible(false);
			lbPoints_->setVisible(false);
			
			SoundManager::sharedSoundManager()->playEffect("gameover_loss");
        
            ChartboostManager::sharedManager().showInterstitial();
		}
		else
		{
			lbTitle_->setString(createWithFormat(" PLAYER %d ", (game_->winnerIndex() + 1)).c_str());
			lbEarned_->setVisible(true);
			outPoints_->setVisible(true);
			lbPoints_->setVisible(true);
			
			SoundManager::sharedSoundManager()->playEffect("gameover_win");
			
			outPoints_->setString(createWithFormat(" %d ", currentPointsNumber_).c_str());
			CCScene::schedule(schedule_selector(GameOverScreen::updatePointsCounter), 0.01);
		}
		
		/*//show accumulated xp points for 1st player only
		if (Session::sharedSession().experiencePoints() && game_->winnerIndex() == 0)
		{
			//currentPointsNumber_ = Session::sharedSession().experiencePoints() - game_->playerPoints(game_->winnerIndex());
			currentPointsNumber_ = Session::sharedSession().experiencePoints();
			updateToPointsNumber_ = currentPointsNumber_ + game_->playerPoints(game_->winnerIndex());
		}
		else //show xp points for winner
		{*/
			currentPointsNumber_ = 0;
			updateToPointsNumber_ = game_->playerPoints(game_->winnerIndex());
		//}
	}
	else
	{
//		btnReplay_->setVisible(false);
//	
//		// local player is winner
//		if (game_->winnerIndex() == onlineGame_->userIndex())
//		{
//			lbLoginIntoFacebook_->setVisible(!Session::sharedSession().isFacebookLoggedIn());
//			
//			lbTitle_->setString((" " + Session::sharedSession().user().name() + " ").c_str());
//			lbEarned_->setVisible(true);
//			outPoints_->setVisible(true);
//			lbPoints_->setVisible(true);
//			
//			/*//currentPointsNumber_ = Session::sharedSession().experiencePoints() - game_->playerPoints(game_->winnerIndex());
//			currentPointsNumber_ = Session::sharedSession().experiencePoints();
//			updateToPointsNumber_ = currentPointsNumber_ + game_->playerPoints(game_->winnerIndex());*/
//			currentPointsNumber_ = 0;
//			updateToPointsNumber_ = game_->playerPoints(game_->winnerIndex());
//			
//			outPoints_->setString(createWithFormat(" %d ", currentPointsNumber_).c_str());
//			CCScene::schedule(schedule_selector(GameOverScreen::updatePointsCounter), 0.01);
//		}
//		else
//		{
//			lbLoginIntoFacebook_->setVisible(false);
//		
//			lbTitle_->setString((" " + onlineGame_->opponent().name() + " ").c_str());
//			lbEarned_->setVisible(false);
//			outPoints_->setVisible(false);
//			lbPoints_->setVisible(false);
//		}
	}
	
	currentTotalPointsNumber_ = Session::sharedSession().experiencePoints();
	outAccumulatedPoints_->setString(createWithFormat(" %d ", Session::sharedSession().experiencePoints()).c_str());
	
	lbLoss_->setVisible(!lbPoints_->isVisible());
	
//	//adding points
//	if (game_->type() == GameType_Local)
//	{
//		//add xp points for 1st player
//		Session::sharedSession().addExperiencePoints(game_->playerPoints(0));
//	}
//	else if (game_->winnerIndex() == onlineGame_->userIndex())
//	{
//		//add xp points for winner
//		Session::sharedSession().addExperiencePoints(game_->playerPoints(onlineGame_->userIndex()));
//	}
}


void GameOverScreen::onExit()
{
	CCScene::onExit();
	
	CCScene::unschedule(schedule_selector(GameOverScreen::updatePointsCounter));
}


void GameOverScreen::updatePointsCounter(float dt)
{
	currentPointsNumber_ += (int) (dt * 10000);
	currentTotalPointsNumber_ += (int) (dt * 10000);
	
	if (currentPointsNumber_ > 0)
	{
		if (currentPointsNumber_ > updateToPointsNumber_)
		{
			currentPointsNumber_ = updateToPointsNumber_;
			CCScene::unschedule(schedule_selector(GameOverScreen::updatePointsCounter));
			
			SoundManager::sharedSoundManager()->playEffect("gameover_points_counting");
			
            ChartboostManager::sharedManager().showInterstitial();
        
			if ( game_->player(game_->winnerIndex()).type() != PlayerType_AI )
			{
				showFacebookButton();
				
				this->runAction(CCSequence::create(CCDelayTime::create(1.25),
					CCCallFunc::create(this, callfunc_selector(GameOverScreen::showUnlockedUnits)),
					0));
			}
			
//			if (Session::sharedSession().isFacebookLoggedIn() &&
//				((game_->type() == GameType_RemoteClient && game_->winnerIndex() == onlineGame_->userIndex())
//					|| (game_->winnerIndex() == 0 && game_->type() == GameType_Local)))
//			{
//				showFacebookButton();
//			}
		}
		
		outPoints_->setString(createWithFormat(" %d ", currentPointsNumber_).c_str());
	}
	
	if (currentTotalPointsNumber_ != Session::sharedSession().experiencePoints())
	{
		if (currentTotalPointsNumber_ > Session::sharedSession().experiencePoints())
			currentTotalPointsNumber_ = Session::sharedSession().experiencePoints();
		
		outAccumulatedPoints_->setString(createWithFormat(" %d ", currentTotalPointsNumber_).c_str());
	}
}


void GameOverScreen::showFacebookButton()
{
	if (!facebookButtonWasShown_)
	{
		facebookButtonWasShown_ = true;
		
//		if (Session::sharedSession().isFacebookLoggedIn() && game_->winnerIndex() == 0)
		{
			lbShareToFacebook_->setVisible(true);
			lbShareToFacebook_->setOpacity(255);
			lb5000XP_->setVisible(true);
			lb5000XP_->setOpacity(255);
			btnPostToFacebook_->setVisible(true);
			btnPostToFacebook_->setOpacity(255);
			
			lbAccumulatedPoints_->setVisible(true);
			outAccumulatedPoints_->setVisible(true);
		}
//		else
//		{
//			lbShareToFacebook_->setVisible(false);
//			lb5000XP_->setVisible(false);
//			btnPostToFacebook_->setVisible(false);
//		}
	}
}


void GameOverScreen::facebook()
{
	FacebookSharing::sharedSharing().facebookShare(
		createWithFormat("I have won and earned %d experience points in Pirates VS Ninjas. Check out this cool game.",
			game_->playerPoints(game_->winnerIndex())).c_str(),
	FacebookSharing::ShareCallback(this,
		(FacebookSharing::ShareCallbackSelector)&GameOverScreen::didFacebookShare));
}


void GameOverScreen::didFacebookShare(bool published, const Error& error)
{
	if (!error.isOk())
	{
		CCAlertBox::show("Share Failed", error.description(), "Dismiss");
	}
	else if (published)
	{
		lbShareToFacebook_->runAction(CCFadeOut::create(0.25));
		lb5000XP_->runAction(CCFadeOut::create(0.25));
		btnPostToFacebook_->runAction(CCFadeOut::create(0.25));
		
		//delegate()->gameOverScreenPostToFacebook();
		
		currentPointsNumber_ = game_->playerPoints(game_->winnerIndex());
		currentTotalPointsNumber_ = Session::sharedSession().experiencePoints();
		Session::sharedSession().addExperiencePoints(5000);
		updateToPointsNumber_ = game_->playerPoints(game_->winnerIndex()) + 5000;
		CCScene::schedule(schedule_selector(GameOverScreen::updatePointsCounter), 0.01);
	}
}


void GameOverScreen::showUnlockedUnits()
{
	UnitType* unit_type = NULL;
	Avatar* avatar = game_->player(0).config().avatar();
	
	int points_before_game = Session::sharedSession().experiencePoints() - game_->playerPoints(game_->winnerIndex());
	int points_after_game = Session::sharedSession().experiencePoints();
	CCLOG("Points before game %d", points_before_game);
	CCLOG("Points after game %d", points_after_game);
	
	for (std::vector<UnitType*>::const_iterator i=(*avatar).largeUnits().begin(),
		e=(*avatar).largeUnits().end(); i!=e; ++i)
	{
		if ((*i)->isLarge() && (*i)->cost() > points_before_game && (*i)->cost() < points_after_game)
		{
			unit_type = (*i);
			break;
		}
	}
	
	if (!unit_type)
	{
		avatar = game_->player(1).config().avatar();
		
		for (std::vector<UnitType*>::const_iterator i=(*avatar).largeUnits().begin(),
			e=(*avatar).largeUnits().end(); i!=e; ++i)
		{
			if ((*i)->isLarge() && (*i)->cost() > points_before_game && (*i)->cost() < points_after_game)
			{
				unit_type = (*i);
				break;
			}
		}
	}
	
	if (unit_type && lbUnlocked_->getOpacity() == 0)
	{
		lbUnlockedUnitName_->setString(unit_type->name().c_str());
		
		if (iconUnlockedUnit_)
		{
			iconUnlockedUnit_->removeFromParentAndCleanup(true);
		}
		
		iconUnlockedUnit_ = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("menu_avatar_%d_big_%d.png",
			unit_type->avatar()->index(),
			unit_type->index())->getCString());
		iconUnlockedUnit_->setAnchorPoint(CCPoint(0.5, 0.5));
		iconUnlockedUnit_->setPosition(CCPoint(getContentSize().width * 0.5,
			getContentSize().height * 0.5 + (isIPhone() ? 30 : 60 )));
		bg_->addChild(iconUnlockedUnit_);
		
		lbUnlocked_->setPosition(CCPoint(getContentSize().width * 0.5,
			iconUnlockedUnit_->getPositionY() + iconUnlockedUnit_->getContentSize().height - lbUnlocked_->getContentSize().height * 0.5));
		lbUnlockedUnitName_->setPosition(CCPoint(getContentSize().width * 0.5,
			iconUnlockedUnit_->getPositionY() - iconUnlockedUnit_->getContentSize().height + lbUnlockedUnitName_->getContentSize().height * 0.5));
		
		lbEarned_->runAction(CCFadeOut::create(0.25));
		outPoints_->runAction(CCFadeOut::create(0.25));
		lbPoints_->runAction(CCFadeOut::create(0.25));
		outAccumulatedPoints_->runAction(CCFadeOut::create(0.25));
		lbAccumulatedPoints_->runAction(CCFadeOut::create(0.25));
		
		lbUnlocked_->setVisible(true);
		lbUnlockedUnitName_->setVisible(true);
		
		iconUnlockedUnit_->runAction(CCFadeIn::create(0.5));
		lbUnlocked_->runAction(CCFadeIn::create(0.5));
		lbUnlockedUnitName_->runAction(CCFadeIn::create(0.5));
	}
}


void GameOverScreen::menu()
{
	delegate()->gameOverScreenBackToMenu();
}


void GameOverScreen::replay()
{
	delegate()->gameOverScreenReplay();
}


