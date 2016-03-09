#include "MainMenu.h"
#include "SoundManager.h"
#include "Utils.h"
#include "Session.h"
#include "GoogleAnalytics.h"

#include "FacebookSharing.h"

using namespace cocos2d;
using namespace cocos2d_utils;
using namespace CocosDenshion;
using namespace monsters;

bool MainMenu::init()
{
	if (!CCScene::init())
	{
		return false;
	}
	
	CCLayer* bg = CCLayer::create();
	
	CCSprite* bg_menu = CCSprite::create(cocos2d_utils::textureName("Menu-Background.jpg").c_str());
	if (getContentSize().height / bg_menu->getContentSize().height >
		getContentSize().width / bg_menu->getContentSize().width)
	{
		bg_menu->setScale(getContentSize().height / bg_menu->getContentSize().height);
	}
	else
	{
		bg_menu->setScale(getContentSize().width / bg_menu->getContentSize().width);
	}
	
	bg_menu->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.5));
	bg_menu->setAnchorPoint(CCPoint(0.5, 0.5));
	bg->addChild(bg_menu);
	
	CCSprite* logo = CCSprite::createWithSpriteFrameName("logo.png");
	logo->setAnchorPoint(CCPoint(0.5, 1));
	logo->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height - ct(10)));
	bg->addChild(logo);
	
	
	menu_ = CCMenu::create();
	menu_->setPosition(CCPoint(0, 0));
	
	CCMenuItemSprite* btn_single_player = menuItemSpriteHighlighted("btn_single_player.png", this,
		menu_selector(MainMenu::singlePlayer));
	btn_single_player->setPosition(CCPoint(getContentSize().width * 0.5, getContentSize().height * 0.55));
	menu_->addChild(btn_single_player);
	
	CCMenuItemSprite* btn_two_players = menuItemSpriteHighlighted("btn_two_players.png", this,
		menu_selector(MainMenu::twoPlayers));
	btn_two_players->setPosition(CCPoint(getContentSize().width * 0.5, btn_single_player->getPositionY() - ct(70)));
	menu_->addChild(btn_two_players);
	
//	CCMenuItemSprite* btn_online_game = menuItemSpriteHighlighted("btn_online_game.png", this,
//		menu_selector(MainMenu::onlineGame));
//	btn_online_game->setPosition(CCPoint(getContentSize().width * 0.5, btn_two_players->getPositionY() - ct(70)));
//	menu_->addChild(btn_online_game);
	
//	btnFacebook_ = menuItemSpriteHighlighted("btn_facebook.png", this,
//		menu_selector(MainMenu::facebookLogin));
//	btnFacebook_->setAnchorPoint(CCPoint(0, 0));
//	btnFacebook_->setPosition(CCPoint(ct(10), ct(10)));
//	menu_->addChild(btnFacebook_);
	
	CCMenuItemSprite* btn_settings = menuItemSpriteHighlighted("btn_settings.png", this,
		menu_selector(MainMenu::settings));
	btn_settings->setAnchorPoint(CCPoint(1, 0));
	btn_settings->setPosition(CCPoint(getContentSize().width - ct(10), ct(10)));
	menu_->addChild(btn_settings);
	
	addChild(bg);
	addChild(menu_);
	
	facebookLoginLoadingIndicatorLayer_ = LoadingIndicatorLayer::create();
	facebookLoginLoadingIndicatorLayer_->setCaption("Signing in...");
	facebookLoginLoadingIndicatorLayer_->setLinkedNodes(CCArray::createWithObject(menu_));
	addChild(facebookLoginLoadingIndicatorLayer_);
	
	loadingIndicatorLayer_ = LoadingIndicatorLayer::create();
	loadingIndicatorLayer_->setCaption("Loading...");
	loadingIndicatorLayer_->setLinkedNodes(CCArray::createWithObject(menu_));
	addChild(loadingIndicatorLayer_);
	
	synchronizingIndicatorLayer_ = LoadingIndicatorLayer::create();
	synchronizingIndicatorLayer_->setCaption("Synchronizing...");
	synchronizingIndicatorLayer_->setLinkedNodes(CCArray::createWithObject(menu_));
	addChild(synchronizingIndicatorLayer_);
	
    return true;
}

MainMenu::~MainMenu()
{
	
}

void MainMenu::didFacebookShare(bool published, const Error& error)
{

}

void MainMenu::singlePlayer(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate_->mainMenuSinglePlayer();
}

void MainMenu::twoPlayers(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate_->mainMenuTwoPlayers();
}

void MainMenu::onlineGame(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate_->mainMenuOnlineGame();
}

void MainMenu::facebookLogin(CCObject* sender)
{
	delegate_->mainMenuFacebook();
}

void MainMenu::settings(CCObject* sender)
{
	SoundManager::sharedSoundManager()->playEffect("button_click");
	delegate_->mainMenuSettings();
}

void MainMenu::onEnter()
{
	CCScene::onEnter();
	
	w2f2::GoogleAnalytics::instance().trackScreenEnter("Main Menu");
	
	if (!loadingIndicatorLayer_->isVisible())
	{
//		btnFacebook_->setVisible(!Session::sharedSession().isFacebookLoggedIn());
		menu_->setVisible(true);
	}
	
	if (SoundManager::sharedSoundManager()->currentMusic()->compare("main_menu_bgmusic"))
	{
		SoundManager::sharedSoundManager()->stopBackgroundMusic();
		this->runAction(CCSequence::create(CCDelayTime::create(0.1),
			CCCallFunc::create(this, callfunc_selector(MainMenu::startBackgroundMusic)),
			0));
	}
}

void MainMenu::startBackgroundMusic()
{
	#ifndef MN_DISABLE_SOUND
	SoundManager::sharedSoundManager()->startBackgroundMusicAtPath("main_menu_bgmusic");
	#endif
}

void MainMenu::onExit()
{
	CCScene::onExit();
	
	//SoundManager::sharedSoundManager()->stopBackgroundMusic();
}

void MainMenu::hideFacebookButton(bool animated)
{
	btnFacebook_->setVisible(false);
	
//	if (animated)
//	{
//		btnFacebook_->runAction(CCFadeOut::create(0.25));
//	}
//	else
//	{
//		btnFacebook_->setOpacity(0);
//	}
}

void MainMenu::showFacebookButton()
{
	btnFacebook_->setVisible(true);
}

void MainMenu::showFacebookLoginLoadingIndicator()
{
	facebookLoginLoadingIndicatorLayer_->show();
}

void MainMenu::hideFacebookLoginLoadingIndicator()
{
	facebookLoginLoadingIndicatorLayer_->hide();
}

void MainMenu::showLoadingIndicator()
{
	facebookLoginLoadingIndicatorLayer_->hide();
	loadingIndicatorLayer_->show();
}

void MainMenu::hideLoadingIndicator()
{
	loadingIndicatorLayer_->hide();
}

void MainMenu::showSynchronizingIndicator()
{
	loadingIndicatorLayer_->hide();
	facebookLoginLoadingIndicatorLayer_->hide();
	synchronizingIndicatorLayer_->show();
}

void MainMenu::hideSynchronizingIndicator()
{
	synchronizingIndicatorLayer_->hide();
}

//void MainMenu::menuCloseCallback(CCObject* pSender)
//{
//    CCDirector::sharedDirector()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}

