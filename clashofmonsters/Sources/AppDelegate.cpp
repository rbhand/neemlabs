

//
//  ClashOfMonstersAppDelegate.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/26/12.
//  Copyright Factorial Complexity 2012. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "Session.h"
#include "ChartboostManager.h"
#include "GoogleAnalytics.h"
//#include "OnlineGame.h"
#include "CCAlertBox.h"

#include "Fixed.h"

USING_NS_CC;

using namespace monsters;

AppDelegate::AppDelegate() :
	mainMenu_(0), settingsScreen_(0), playScreen_(0)
{
	settings_ = Settings();
}

AppDelegate::~AppDelegate()
{
	if (mainMenu_)
		mainMenu_->release();
	if (settingsScreen_)
		settingsScreen_->release();
//	if (onlineGameScreen_)
//		onlineGameScreen_->release();
	if (gameConfigurationScreen_)
		gameConfigurationScreen_->release();
	if (playScreen_)
		playScreen_->release();
	if (gameOverScreen_)
		gameOverScreen_->release();
	if (tutorialScreen_)
		delete tutorialScreen_;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	
	using namespace w2f2;
	
	GoogleAnalytics::initialize("UA-42196849-1");
	
//	CCFileUtils::setiPhoneRetinaDisplaySuffix("");
//	CCFileUtils::setiPadSuffix("");
//	CCFileUtils::setiPadRetinaDisplaySuffix("");

	// enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//	pDirector->enableRetinaDisplay(true);

	if (cocos2d_utils::isIPhone5())
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 568, kResolutionNoBorder);
	else if (cocos2d_utils::isIPhone())
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionNoBorder);
	else
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(768, 1024, kResolutionNoBorder);

	if (cocos2d_utils::isRetina())
		pDirector->setContentScaleFactor(2);
	else
		pDirector->setContentScaleFactor(1);
	
	// turn on display FPS
	//pDirector->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);
	
	CCScene* splashScreen = CCScene::create();
	CCSprite* default_png = CCSprite::create(cocos2d_utils::defaultPNGFileName().c_str());
	default_png->setPosition(CCPoint(splashScreen->getContentSize().width * 0.5, splashScreen->getContentSize().height * 0.5));
	splashScreen->addChild(default_png);
	CCDirector::sharedDirector()->runWithScene(splashScreen);
	
	settings_.load();
	SoundManager::sharedSoundManager()->setIsSoundEnabled(settings_.isSoundEnabled());
	SoundManager::sharedSoundManager()->setIsMusicEnabled(settings_.isMusicEnabled());
	
	addTextureAsync("Textures/Menu-Background.jpg");
	addTextureAsync("Textures/Level_0-Background.jpg");
	addTextureAsync("Textures/Menu.png");
	addTextureAsync("Textures/Menu_Avatars.png");
	addTextureAsync("Textures/PlayScreen.png");
	addTextureAsync("Textures/Avatar_0.png");
	addTextureAsync("Textures/Avatar_1.png");
	addTextureAsync("Textures/Avatar_0_attack.png");
	addTextureAsync("Textures/Avatar_1_attack.png");
	
	Session::sharedSession();
	ChartboostManager::sharedManager();
	
	loadNextTexture();
	
	return true;
}

void AppDelegate::addTextureAsync(const char* path)
{
	std::string suffixed_path = cocos2d_utils::textureName(path);
	
	texturesToLoad_.push_back(suffixed_path);
	
	std::string plist_file_name = cocos2d_utils::fileNameWithoutExtension(suffixed_path.c_str()) +
		".plist";
	if (cocos2d_utils::isFileExists(CCFileUtils::sharedFileUtils()->fullPathForFilename(plist_file_name.c_str())))
	{
		plistsToLoad_.push_back(plist_file_name);
	}
}

void AppDelegate::loadNextTexture()
{
	if (texturesToLoad_.size())
	{
		std::string suffixed_path = texturesToLoad_.front();
		texturesToLoad_.erase(texturesToLoad_.begin());
		
		std::cout << "loadNextTexture: " << suffixed_path << std::endl;
		
		CCTextureCache::sharedTextureCache()->addImageAsync(suffixed_path.c_str(),
			this, callfuncO_selector(AppDelegate::textureDidLoad));
	}
	else
	{
		continueLoad();
	}
}

void AppDelegate::continueLoad()
{
	srand(time(0));

	for (std::list<std::string>::const_iterator i=plistsToLoad_.begin(), e=plistsToLoad_.end(); i!=e; ++i)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(i->c_str());
	}
	
	mainMenu_ = MainMenu::create();
	mainMenu_->retain();
	mainMenu_->setDelegate(this);
	
	settingsScreen_ = SettingsScreen::create();
	settingsScreen_->retain();
	settingsScreen_->setSettings(&settings_);
	settingsScreen_->setDelegate(this);
	
//	onlineGameScreen_ = OnlineGameScreen::create();
//	onlineGameScreen_->retain();
//	onlineGameScreen_->setDelegate(this);
	
	gameConfigurationScreen_ = GameConfigurationScreen::create();
	gameConfigurationScreen_->retain();
	gameConfigurationScreen_->setDelegate(this);
	gameConfigurationScreen_->setUnits(&Session::sharedSession().units());
	
	playScreen_ = PlayScreen::create();
	playScreen_->retain();
	playScreen_->setGame(&game_);
	playScreen_->setSettings(&settings_);
	playScreen_->setDelegate(this);
	
	gameOverScreen_ = GameOverScreen::create();
	gameOverScreen_->retain();
	gameOverScreen_->setGame(&game_);
	gameOverScreen_->setDelegate(this);
	
	tutorialScreen_ = new TutorialScreen();
	
	Session::sharedSession().setDelegate(this);
	
	//CCDirector::sharedDirector()->runWithScene(mainMenu_);
	transitionToScreen(mainMenu_);
	
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("showStartupAds", false))
		ChartboostManager::sharedManager().showInterstitial();
	else
		CCUserDefault::sharedUserDefault()->setBoolForKey("showStartupAds", true);
}

void AppDelegate::textureDidLoad(CCObject* tex)
{
	loadNextTexture();
}

//CCScene* AppDelegate::getRunningOrTransitingToScene()
//{
//	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
//	if (CCTransitionScene* t_scene = dynamic_cast<CCTransitionScene*>(scene))
//	{
//		return t_scene->getTargetScene();
//	}
//	
//	return scene;
//}

void AppDelegate::transitionToScreen(cocos2d::CCScene* scene, bool animated)
{
	if (CCDirector::sharedDirector()->getRunningScene() != scene)
	{
		if (animated)
		{
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3, scene, ccc3(0, 0, 0)));
		}
		else
		{
			CCDirector::sharedDirector()->replaceScene(scene);
		}
	}
	processNextNotification(scene);
}

void AppDelegate::mainMenuSinglePlayer()
{
/*	PlayerConfig p1;
	p1.avatar = units_.avatars().at(0);
	p1.largeUnits[0] = p1.avatar->largeUnits().at(rand() % p1.avatar->largeUnits().size());
	p1.largeUnits[1] = p1.avatar->largeUnits().at(rand() % p1.avatar->largeUnits().size());
	p1.normalUnits[0] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
	p1.normalUnits[1] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
	p1.normalUnits[2] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
	
	PlayerConfig p2;
	p2.avatar = units_.avatars().at(1);
	p2.largeUnits[0] = p2.avatar->largeUnits().at(rand() % p2.avatar->largeUnits().size());
	p2.largeUnits[1] = p2.avatar->largeUnits().at(rand() % p2.avatar->largeUnits().size());
	p2.normalUnits[0] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
	p2.normalUnits[1] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
	p2.normalUnits[2] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
	
	game_.startSinglePlayerGame(p1, p2);
	transitionToScreen(playScreen_);*/
	
	gameConfigurationScreen_->setType(GameConfigurationScreenType_SinglePlayer);
	transitionToScreen(gameConfigurationScreen_);
}

void AppDelegate::mainMenuTwoPlayers()
{
//	PlayerConfig p1;
//	p1.avatar = units_.avatars().at(0);
//	p1.largeUnits[0] = p1.avatar->largeUnits().at(rand() % p1.avatar->largeUnits().size());
//	p1.largeUnits[1] = p1.avatar->largeUnits().at(rand() % p1.avatar->largeUnits().size());
//	p1.normalUnits[0] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
//	p1.normalUnits[1] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
//	p1.normalUnits[2] = p1.avatar->normalUnits().at(rand() % p1.avatar->normalUnits().size());
//	
//	PlayerConfig p2;
//	p2.avatar = units_.avatars().at(1);
//	p2.largeUnits[0] = p2.avatar->largeUnits().at(rand() % p2.avatar->largeUnits().size());
//	p2.largeUnits[1] = p2.avatar->largeUnits().at(rand() % p2.avatar->largeUnits().size());
//	p2.normalUnits[0] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
//	p2.normalUnits[1] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
//	p2.normalUnits[2] = p2.avatar->normalUnits().at(rand() % p2.avatar->normalUnits().size());
//	
//	game_.startTwoPlayersGame(p1, p2);
//	transitionToScreen(playScreen_);
	
	gameConfigurationScreen_->setType(GameConfigurationScreenType_TwoPlayers);
	transitionToScreen(gameConfigurationScreen_);
}

void AppDelegate::gameConfigurationScreenBackToMenu()
{
	transitionToScreen(mainMenu_);
}

void AppDelegate::gameConfigurationScreenStartSinglePlayerGame(const PlayerConfig& player1Config, const PlayerConfig& aiConfig)
{
	game_.startSinglePlayerGame(player1Config, aiConfig);
	//playScreen_->setOnlineGame(boost::shared_ptr<OnlineGame>());
	transitionToScreen(playScreen_);
}

void AppDelegate::gameConfigurationScreenStartTwoPlayersGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config)
{
	game_.startTwoPlayersGame(player1Config, player2Config);
	//playScreen_->setOnlineGame(boost::shared_ptr<OnlineGame>());
	transitionToScreen(playScreen_);
}

void AppDelegate::gameConfigurationScreenTutorial()
{
	tutorialScreen_->show();
}

void AppDelegate::gameConfigurationScreenStartOnlineGame(monsters::OnlineGame *game)
{
	transitionToOnlineGame(game);
}

void AppDelegate::transitionToOnlineGame(OnlineGame* onlineGame)
{
//	if (onlineGame->state() == OnlineGame::State_Configuration)
//	{
//		gameConfigurationScreen_->setOnlineGame(onlineGame);
//		transitionToScreen(gameConfigurationScreen_);
//	}
//	else if (onlineGame->state() == OnlineGame::State_Playing)
//	{
//		pb::Message msg;
//		onlineGame->applyProcessedTurns();
//		onlineGame->data()->rewind();
//		msg.deserialize(*onlineGame->data());
//		playScreen_->setOnlineGame(Session::sharedSession().onlineGameSharedPtr(onlineGame));
//		game_.loadRemoteGame(msg, onlineGame->localPlayerState(), Session::sharedSession().units());
//		transitionToScreen(playScreen_);
//	}
}

void AppDelegate::sessionIsOutOfSync()
{
	if (isInsideOnlineGame())
	{
		transitionToScreen(mainMenu_);
		mainMenu_->showSynchronizingIndicator();
	}
}

void AppDelegate::sessionDidSyncronized()
{
	mainMenu_->hideSynchronizingIndicator();
}

bool AppDelegate::isInsideOnlineGame() const
{
	return false;
//	return ((CCDirector::sharedDirector()->getRunningScene() == playScreen_ &&
//			game_.type() == GameType_RemoteClient) ||
//		(CCDirector::sharedDirector()->getRunningScene() == gameConfigurationScreen_ &&
//			gameConfigurationScreen_->type() == GameConfigurationScreenType_OnlineGame) ||
//		CCDirector::sharedDirector()->getRunningScene() == onlineGameScreen_);
}

void AppDelegate::sessionDoubleLogin()
{
	if (isInsideOnlineGame())
	{
		transitionToScreen(mainMenu_);
	}
	
	CCAlertBox::show("Forced Logout", "Login from another device with the same account detected", "Dismiss");
	mainMenu_->showFacebookButton();
}

void AppDelegate::onGetActiveOnlineGame(OnlineGame* onlineGame, const Error& error)
{
//	mainMenu_->hideLoadingIndicator();
//	
//	if (!error.isOk())
//	{
//		CCAlertBox::show("Network Problems", error.description(), "Dismiss");
//	}
//	else
//	{
//		if (onlineGame)
//		{
//			transitionToOnlineGame(onlineGame);
//		}
//		else
//		{
//			transitionToScreen(onlineGameScreen_);
//		}
//	}
}

void AppDelegate::mainMenuOnlineGame()
{
//	mainMenu_->showLoadingIndicator();
//	Session::sharedSession().getActiveOnlineGame(Session::GetActiveOnlineGameCallback(this,
//		(Session::GetActiveOnlineGameCallbackSelector)&AppDelegate::onGetActiveOnlineGame));
}

void AppDelegate::onSessionLogin(const Error& error)
{
//	if (!error.isOk())
//	{
//		CCAlertBox::show("Login Failed", error.description(), "Dismiss");
//	}
//
//	if (Session::sharedSession().isFacebookLoggedIn())
//	{
//		mainMenu_->hideFacebookButton();
//		mainMenu_->hideFacebookLoginLoadingIndicator();
//	}
}

void AppDelegate::mainMenuFacebook()
{
//	if (!Session::sharedSession().isFacebookLoggedIn())
//	{
//		mainMenu_->showFacebookLoginLoadingIndicator();
//		Session::sharedSession().facebookLogin(Session::LoginCallback(this,
//			(Session::LoginCallbackSelector)&AppDelegate::onSessionLogin));
//	}
}

void AppDelegate::showFacebookLoginLoadingIndicator()
{
	mainMenu_->showFacebookLoginLoadingIndicator();
}

void AppDelegate::mainMenuSettings()
{
	settingsScreen_->setShouldReturnToMenu(true);
	transitionToScreen(settingsScreen_);
}

void AppDelegate::playScreenTutorial()
{
	tutorialScreen_->show();
}

void AppDelegate::settingsScreenBack()
{
	if (settingsScreen_->shouldReturnToMenu())
	{
		transitionToScreen(mainMenu_);
	}
	else
	{
		transitionToScreen(playScreen_);
	}
}

void AppDelegate::settingsScreenSoundOnOf()
{
	
}

void AppDelegate::settingsScreenMusicOnOf()
{
	
}

void AppDelegate::onlineGameScreenBackToMenu()
{
	transitionToScreen(mainMenu_);
}

void AppDelegate::onlineGameScreenGameConfigurationScreen()
{
	transitionToScreen(gameConfigurationScreen_);
}

void AppDelegate::onlineGameScreenGameProceedToGame(OnlineGame* game)
{
	transitionToOnlineGame(game);
}

void AppDelegate::playScreenMenu()
{
	transitionToScreen(mainMenu_);
	//transitionToScreen(gameOverScreen_);
}

void AppDelegate::playScreenGameOver()
{
//	gameOverScreen_->setOnlineGame(playScreen_->onlineGame().get());
//	Session::sharedSession().closeGame(playScreen_->onlineGame());
	transitionToScreen(gameOverScreen_);
}

void AppDelegate::playScreenSettings()
{
	settingsScreen_->setShouldReturnToMenu(false);
	transitionToScreen(settingsScreen_);
}

void AppDelegate::playScreenSoundOnOf()
{
	
}

void AppDelegate::playScreenMusicOnOf()
{
	
}

void AppDelegate::gameOverScreenBackToMenu()
{
	transitionToScreen(mainMenu_);
}

void AppDelegate::gameOverScreenReplay()
{
	game_.restartGame();
	transitionToScreen(playScreen_);
}

void AppDelegate::onProcessInviteCompleted(OnlineGame* onlineGame, const Error& error)
{
//	mainMenu_->hideLoadingIndicator();
//	
//	if (error.isOk())
//	{
//		Session::sharedSession().removeNotification(currentNotification_);
//		currentNotification_ = OnlineNotification();
//			
//		if (onlineGame)
//		{
//			transitionToOnlineGame(onlineGame);
//		}
//	}
//	else
//	{
//		alertNotificationError_.reset(new CCAlertBox("Network Problems", error.description(),
//			"Dismiss", "", this));
//		alertNotificationError_->show();
//	}
}

void AppDelegate::alertBoxDidDismiss(CCAlertBox* alertBox, int buttonIndex)
{
//	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
//	
//	if (alertBox == alertNotification_.get())
//	{
//		if (currentNotification_.type() == OnlineNotification::Type_InviteReceived)
//		{
//			if (buttonIndex != alertBox->cancelButtonIndex())
//			{
//				transitionToScreen(mainMenu_, false);
//				mainMenu_->showLoadingIndicator();
//			}
//			
//			Session::sharedSession().processInvite(currentNotification_.invite()->identifier(),
//				buttonIndex != alertBox->cancelButtonIndex(), Session::ProcessInviteCallback(this,
//				(Session::ProcessInviteCallbackSelector)&AppDelegate::onProcessInviteCompleted));
//		}
//		else if (currentNotification_.type() == OnlineNotification::Type_GameStarted ||
//			currentNotification_.type() == OnlineNotification::Type_GamePlaying ||
//			currentNotification_.type() == OnlineNotification::Type_GameOpponentTurn)
//		{
//			if (buttonIndex != alertBox->cancelButtonIndex() || currentScene == onlineGameScreen_)
//			{
//				OnlineGame* game = currentNotification_.game().get();
//				Session::sharedSession().removeNotification(currentNotification_);
//				currentNotification_ = OnlineNotification();
//				transitionToOnlineGame(game);
//			}
//			else
//			{
//				Session::sharedSession().removeNotification(currentNotification_);
//				currentNotification_ = OnlineNotification();
//			}
//		}
//		else
//		{
//			Session::sharedSession().removeNotification(currentNotification_);
//			currentNotification_ = OnlineNotification();
//			processNextNotification();
//		}
//	}
//	else if (alertBox == alertNotificationError_.get())
//	{
//		currentNotification_ = OnlineNotification();
//		processNextNotification();
//	}
}

void AppDelegate::processNextNotification(cocos2d::CCScene* currentScene)
{
//	if (!currentScene)
//		currentScene = CCDirector::sharedDirector()->getRunningScene();
//
//	Session& session = Session::sharedSession();
//	if (currentNotification_.type() == OnlineNotification::Type_None &&
//		session.hasNotifications() &&
//		(currentScene == mainMenu_ || currentScene == settingsScreen_ ||
//		(currentScene == onlineGameScreen_ && !Session::sharedSession().activeRandomGameInvite()) ||
//		((currentScene == playScreen_ || currentScene == gameConfigurationScreen_)
//			&& game_.type() == GameType_Local)))
//	{
//		OnlineNotification n = session.nextNotification();
//		currentNotification_ = n;
//		
//		switch (n.type())
//		{
//			case OnlineNotification::Type_GameStarted:
//			{
//				alertNotification_.reset(new CCAlertBox("Game Started", "Your game against " + n.user().name() + " has just started", "Close",
//					(currentScene == onlineGameScreen_) ? "" : "Play", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_GameResigned:
//			{
//				alertNotification_.reset(new CCAlertBox(n.user().name(), "Resigned from game", "Dismiss", "", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_GameExpired:
//			{
//				alertNotification_.reset(new CCAlertBox(n.user().name(), "Game expired", "Dismiss", "", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_InviteReceived:
//			{
//				alertNotification_.reset(new CCAlertBox("New Invite", n.user().name() + " invites you to play against him",
//					"Decline", "Accept", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_InviteDeclined:
//			{
//				onlineGameScreen_->reloadData();
//				
//				alertNotification_.reset(new CCAlertBox("Invitation Declined", n.user().name() + " has declined your invitation",
//					"Dismiss", "", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_FriendAdded:
//			{
//				alertNotification_.reset(new CCAlertBox("New Friend", "Your friend " + n.user().name() + " has just signed up",
//					"Dismiss", "", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_GamePlaying:
//			{
//				alertNotification_.reset(new CCAlertBox("Playing", n.user().name() + " has finished picking his units. You can play now.",
//					"Close", "Play", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_GameOpponentTurn:
//			{
//				alertNotification_.reset(new CCAlertBox("New Turn", "Your opponent " + n.user().name() + " has made his turn",
//					"Close", "View", this));
//				alertNotification_->show();
//			}
//			break;
//			
//			case OnlineNotification::Type_None:
//			break;
//		}
//	}
}

void AppDelegate::sessionHasNewNotifications()
{
	processNextNotification();
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();
	
	Session::sharedSession().save();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
	
	if (mainMenu_)
		mainMenu_->hideFacebookLoginLoadingIndicator();
    
	// if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("showStartupAds", false))
		ChartboostManager::sharedManager().showInterstitial();
	else
		CCUserDefault::sharedUserDefault()->setBoolForKey("showStartupAds", true);
}
