//
//  Session.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/25/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "Callback.h"
#include "Error.h"
#include "User.h"
//#include "Update.h"
#include "GameUnits.h"
//#include "AvatarsCache.h"
//#include "OnlineNotification.h"

#include <vector>
#include <list>
#include <string>

#include "boost_essentials.h"

namespace pb { class Message; }
namespace w2f2 { class JSONVariant; }

namespace monsters
{
	class ISessionDelegate
	{
	public:
		virtual void sessionIsOutOfSync() = 0;
		virtual void sessionDidSyncronized() = 0;
		virtual void sessionDoubleLogin() = 0;
		
		virtual void sessionHasNewNotifications() = 0;
	};


	class OnlineGame;
	class OnlineInvite;
//	class SessionPrivate;
	class Session
	{
		friend class SessionPrivate;
		friend class OnlineGame;
		
	public:
		typedef void(cocos2d::CCObject::*OperationCallbackSelector)(const Error& error);
		typedef w2f2::Callback<OperationCallbackSelector> OperationCallback;
	
		typedef void(cocos2d::CCObject::*LoginCallbackSelector)(const Error& error);
		typedef w2f2::Callback<LoginCallbackSelector> LoginCallback;
		
		typedef void(cocos2d::CCObject::*GetUsersListCallbackSelector)(const std::vector<User>& friends, const Error& error);
		typedef w2f2::Callback<GetUsersListCallbackSelector> GetUsersListCallback;
		
		typedef void(cocos2d::CCObject::*GetActiveOnlineGameCallbackSelector)(OnlineGame* onlineGame, const Error& error);
		typedef w2f2::Callback<GetActiveOnlineGameCallbackSelector> GetActiveOnlineGameCallback;
		
		typedef void(cocos2d::CCObject::*StartRandomGameCallbackSelector)(OnlineGame* onlineGame, const Error& error);
		typedef w2f2::Callback<StartRandomGameCallbackSelector> StartRandomGameCallback;
		
		typedef void(cocos2d::CCObject::*ProcessInviteCallbackSelector)(OnlineGame* onlineGame, const Error& error);
		typedef w2f2::Callback<ProcessInviteCallbackSelector> ProcessInviteCallback;
		
	public:
		Session();
		~Session();
		
//		void facebookLogin(const LoginCallback& callback);
//		bool isFacebookLoggedIn() const { return user_.facebookId().size() > 0; }
//		const User& user() const { return user_; }
		
		int experiencePoints() const;
		void addExperiencePoints(int points);
		
//		void getActiveOnlineGame(const GetActiveOnlineGameCallback& callback);
//		void startRandomGame(const StartRandomGameCallback& callback);
//		void cancelRandomGame();
//		void inviteUserToGame(const User& user, const OperationCallback& callback);
		
//		void setActiveGame(OnlineGame* game, const OperationCallback& callback);
//		void processInvite(const std::string& inviteId, bool accept, const ProcessInviteCallback& callback);
//		void acceptInvite(const std::string& inviteId, const ProcessInviteCallback& callback);
//		void declineInvite(const std::string& inviteId, const ProcessInviteCallback& callback);
//		bool isUserInvited(const User& user) const;
		
//		boost::shared_ptr<OnlineGame> activeOnlineGame() const;
//		boost::shared_ptr<OnlineInvite> activeRandomGameInvite() const;
//		boost::shared_ptr<OnlineGame> onlineGameSharedPtr(OnlineGame* game);
//		void closeGame(boost::shared_ptr<OnlineGame> game);
		
//		void getFriendsList(const GetUsersListCallback& callback);
		
//		void findUsersByName(const std::string& query, const GetUsersListCallback& callback);
		const GameUnits& units() const { return units_; }
		
		void save();
		
//		bool hasNotifications() const;
//		void removeNotification(const OnlineNotification& n);
//		OnlineNotification nextNotification() const;
		
//		AvatarsCache& avatarsCache() { return avatarsCache_; }
		void setDelegate(ISessionDelegate* delegate) { delegate_ = delegate; }
		ISessionDelegate* delegate() const { return delegate_; }
		
		static Session& sharedSession();
		
	protected:
//		void configureOnlineGame(OnlineGame* game, const PlayerConfig& config);
//		void playOnlineGame(OnlineGame* game, const PlayerConfig& config);
//		w2f2::OperationHandle sendTurnOnlineGame(OnlineGame* game, const PlayerAction& turn);
//		void resignOnlineGame(OnlineGame* game);
		
	private:
		pb::Message* serialized() const;
		void load();
		
//		void connect();
//		void synchronize();
//		void loadSyncData(const w2f2::JSONVariant& state);
//		void processUpdate(const Update& update, bool fromLoadState);
		
//		void outOfSync();
//		void doubleLogin();
		
//		boost::shared_ptr<OnlineGame> onlineGameWithId(const std::string& gameId) const;
//		boost::shared_ptr<OnlineInvite> onlineInviteWithId(const std::string& inviteId);
//		void removeOnlineInviteWithId(const std::string& inviteId);
//		void removeOnlineGame(const boost::shared_ptr<OnlineGame>& onlineGame);
		
//		void startReceivingUpdates();
//		void stopReceivingUpdates();
//		void receiveUpdates();
		
//		void uploadExperiencePoints();
		
	private:
		User user_;
		int experiencePoints_;
		
		// if true session is in process of establishing connection
		// it is either logging in, synchronizing or linking Facebook account
		// all other rrequests should wait until operation is over
//		bool connecting_;
		std::string loginKey_;
		std::string sessionKey_;
		
//		std::list<boost::shared_ptr<OnlineGame> > onlineGames_;
//		std::list<boost::shared_ptr<OnlineInvite> > sentInvites_;
//		std::list<boost::shared_ptr<OnlineInvite> > receivedInvites_;
//		std::vector<User> friends_;
		
//		std::string lastActiveGameId_;
//		int lastActiveGameRound_;
//		uint64_t lastActiveGameRoundTime_;
		
//		StartRandomGameCallback startRandomGameCallback_;
//		boost::shared_ptr<OnlineGame> activeGame_;
		
		// notifications
//		std::list<User> notifyFriendsAdded_;
//		std::list<User> notifyInviteDeclined_;
//		std::list<std::string> notifyGameStarted_;
//		std::list<OnlineNotification> notifyGameResignedOrExpired_;
		
//		std::string updatesToken_;
		
		ISessionDelegate* delegate_;
//		AvatarsCache avatarsCache_;
		GameUnits units_;
//		SessionPrivate* d_;
	};
}
