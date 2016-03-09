//
//  ClashOfMonstersAppDelegate.h
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/26/12.
//  Copyright Factorial Complexity 2012. All rights reserved.
//

#pragma once

#include "CCApplication.h"
//#include "CCAlertBox.h"

#include "MainMenu.h"
#include "PlayScreen.h"
#include "SettingsScreen.h"
//#include "OnlineGameScreen.h"
#include "GameConfigurationScreen.h"
#include "GameOverScreen.h"
#include "TutorialScreen.h"
#include "Game.h"
#include "Settings.h"
#include "Session.h"

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

namespace monsters
{
	class Error;

	/**
	@brief    The cocos2d Application.

	The reason for implement as private inheritance is to hide some interface call by CCDirector.
	*/
	class AppDelegate : public cocos2d::CCObject,
		private IMainMenuDelegate,
		private ISettingsScreenDelegate,
		//private IOnlineGameScreenDelegate,
		private IGameConfigurationScreenDelegate,
		private IPlayScreenDelegate,
		private IGameOverScreenDelegate,
		private ISessionDelegate,
		//private ICCAlertBoxDelegate,
		public cocos2d::CCApplication
	{
	public:
		AppDelegate();
		virtual ~AppDelegate();

		/**
		@brief    Implement CCDirector and CCScene init code here.
		@return true    Initialize success, app continue.
		@return false   Initialize failed, app terminate.
		*/
		virtual bool applicationDidFinishLaunching();

		/**
		@brief  The function be called when the application enter background
		@param  the pointer of the application
		*/
		virtual void applicationDidEnterBackground();

		/**
		@brief  The function be called when the application enter foreground
		@param  the pointer of the application
		*/
		virtual void applicationWillEnterForeground();
		
		void showFacebookLoginLoadingIndicator();
		
	private:
		void addTextureAsync(const char* path);
		void loadNextTexture();
		void textureDidLoad(cocos2d::CCObject* tex);
		void continueLoad();
		
		void transitionToScreen(cocos2d::CCScene* scene, bool animated = true);
		
		void mainMenuSinglePlayer();
		void mainMenuTwoPlayers();
		void mainMenuOnlineGame();
		void mainMenuFacebook();
		void mainMenuSettings();
		
		void settingsScreenBack();
		void settingsScreenSoundOnOf();
		void settingsScreenMusicOnOf();
		
		void onlineGameScreenBackToMenu();
		void onlineGameScreenGameConfigurationScreen();
		void onlineGameScreenGameProceedToGame(OnlineGame* game);
		
		void gameConfigurationScreenBackToMenu();
		void gameConfigurationScreenStartSinglePlayerGame(const PlayerConfig& player1Config, const PlayerConfig& aiConfig);
		void gameConfigurationScreenStartTwoPlayersGame(const PlayerConfig& player1Config, const PlayerConfig& player2Config);
		void gameConfigurationScreenStartOnlineGame(monsters::OnlineGame *game);
		void gameConfigurationScreenTutorial();
		
		void playScreenMenu();
		void playScreenTutorial();
		void playScreenGameOver();
		void playScreenSettings();
		void playScreenSoundOnOf();
		void playScreenMusicOnOf();
		
		void gameOverScreenBackToMenu();
		void gameOverScreenReplay();
		
		void onSessionLogin(const Error& error);
		void onGetActiveOnlineGame(OnlineGame* onlineGame, const Error& error);
		void onProcessInviteCompleted(OnlineGame* onlineGame, const Error& error);
		
		void processNextNotification(cocos2d::CCScene* currentScene=0);
		//void alertBoxDidDismiss(CCAlertBox* alertBox, int buttonIndex);
		
		void sessionHasNewNotifications();
		void sessionIsOutOfSync();
		void sessionDoubleLogin();
		void sessionDidSyncronized();

		void transitionToOnlineGame(OnlineGame* onlineGame);
		//cocos2d::CCScene* sceneCCScene* getRunningOrTransitingToScene();
		
		bool isInsideOnlineGame() const;
		
	private:
		Game game_;
		Settings settings_;
		
		MainMenu* mainMenu_;
		SettingsScreen* settingsScreen_;
		//OnlineGameScreen* onlineGameScreen_;
		GameConfigurationScreen* gameConfigurationScreen_;
		GameOverScreen* gameOverScreen_;
		PlayScreen* playScreen_;
		TutorialScreen* tutorialScreen_;
		
//		OnlineNotification currentNotification_;
//		boost::shared_ptr<cocos2d::CCAlertBox> alertNotification_;
//		boost::shared_ptr<cocos2d::CCAlertBox> alertNotificationError_;
		
		//int texturesLeft_;
		std::list<std::string> texturesToLoad_;
		std::list<std::string> plistsToLoad_;
	};
}
