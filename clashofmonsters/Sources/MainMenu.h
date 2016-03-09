#pragma once

#include "cocos2d.h"
#include "LoadingIndicatorLayer.h"

namespace monsters
{
	class IMainMenuDelegate
	{
	public:
		virtual void mainMenuSinglePlayer() = 0;
		virtual void mainMenuTwoPlayers() = 0;
		virtual void mainMenuOnlineGame() = 0;
		virtual void mainMenuFacebook() = 0;
		virtual void mainMenuSettings() = 0;
	};

	class MainMenu : public cocos2d::CCScene
	{
	public:
		virtual bool init();
		~MainMenu();
		
		void setDelegate(IMainMenuDelegate* delegate) { delegate_ = delegate; }
		IMainMenuDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(MainMenu);
		
		void showFacebookButton();
		void hideFacebookButton(bool animated = true);
		
		void showFacebookLoginLoadingIndicator();
		void hideFacebookLoginLoadingIndicator();
		
		void showLoadingIndicator();
		void hideLoadingIndicator();
		
		void showSynchronizingIndicator();
		void hideSynchronizingIndicator();
		
	private:
		void singlePlayer(CCObject* sender);
		void twoPlayers(CCObject* sender);
		void onlineGame(CCObject* sender);
		void facebookLogin(CCObject* sender);
		void settings(CCObject* sender);
		void tutorial(CCObject* sender);
		
		void onEnter();
		void onExit();
		void startBackgroundMusic();
		
	private:
		IMainMenuDelegate* delegate_;
		
		CCMenu* menu_;
		cocos2d::CCMenuItemSprite* btnFacebook_;
		
		LoadingIndicatorLayer* facebookLoginLoadingIndicatorLayer_;
		LoadingIndicatorLayer* loadingIndicatorLayer_;
		LoadingIndicatorLayer* synchronizingIndicatorLayer_;
	};
}
