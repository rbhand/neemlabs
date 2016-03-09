//
//  Session.cpp
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/25/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#include "Session.h"
//#include "JSONVariant.h"
//#include "OnlineGame.h"
//#include "OnlineInvite.h"

//#import "W2Client.h"
//#import "W2ClientImageCache.h"

#include "Message.h"
#include <memory>

#include "cocos2d.h"

//#import <FacebookSDK/FacebookSDK.h>

using namespace monsters;
using namespace pb;
using namespace w2f2;

using namespace std;
using namespace boost;

//NSString* W2CreateUUID()
//{
//	CFUUIDRef theUUID = CFUUIDCreate(NULL);
//	CFStringRef string = CFUUIDCreateString(NULL, theUUID);
//	CFRelease(theUUID);
//	return (__bridge_transfer NSString *)string;
//}


//typedef void(^PVNRequireActiveConnectionCompletion)(Error error);

//@interface monsters_objc_SessionPrivate : NSObject
//{
//	SessionPrivate* d_;
//	
//	NSMutableSet* requireConnectionCompletions_;
//}
//
//- (id)initWithSessionPrivate:(SessionPrivate*)d;
//
//- (void)requireActiveConnection:(PVNRequireActiveConnectionCompletion)completion;
//
//@property (nonatomic) NSMutableSet* requireConnectionCompletions;
//
//@end

//namespace monsters
//{
//	class SessionPrivate
//	{
//	public:
//		SessionPrivate(Session* session) :
//			session_(session)
//		{
//			objc_ = [[monsters_objc_SessionPrivate alloc] initWithSessionPrivate:this];
//		}
//		
//		~SessionPrivate()
//		{
//			objc_ = nil;
//		}
//	
//		void outOfSync()
//		{
//			session_->outOfSync();
//		}
//		
//		void doubleLogin()
//		{
//			session_->doubleLogin();
//		}
//		
//		void requireActiveConnection(PVNRequireActiveConnectionCompletion completion)
//		{
//			if (!session_->connecting_)
//				completion(Error());
//			else
//				[objc_ requireActiveConnection:completion];
//		}
//		
//		void afterConnection(const Error& error)
//		{
//			NSMutableSet* completions = objc_.requireConnectionCompletions;
//			objc_.requireConnectionCompletions = [NSMutableSet set];
//			for (PVNRequireActiveConnectionCompletion c in completions)
//			{
//				c(error);
//			}
//		}
//		
//		Error parseError(NSError* objcError) const
//		{
//			if (!objcError)
//				return Error();
//			else
//				return Error([[objcError localizedDescription] UTF8String]);
//		}
//		
//	public:
//		Session* session_;
//		
//		W2Client* client_;
//		W2ClientRequest* updatesRequest_;
//		NSMutableDictionary* config_;
//		monsters_objc_SessionPrivate* objc_;
//	};
//}

//@implementation monsters_objc_SessionPrivate
//
//@synthesize requireConnectionCompletions = requireConnectionCompletions_;
//
//- (id)initWithSessionPrivate:(SessionPrivate*)d
//{
//	if ((self = [super init]))
//	{
//		d_ = d;
//		requireConnectionCompletions_ = [[NSMutableSet alloc] init];
//	}
//	
//	return self;
//}
//
//- (void)outOfSync:(NSNotification*)notification
//{
//	d_->outOfSync();
//}
//
//- (void)doubleLogin:(NSNotification*)notification
//{
//	d_->doubleLogin();
//}
//
//- (void)requireActiveConnection:(PVNRequireActiveConnectionCompletion)completion
//{
//	[requireConnectionCompletions_ addObject:[completion copy]];
//}
//
//+ (NSString*)archivePath
//{
//	return [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)
//		objectAtIndex:0] stringByAppendingPathComponent:[NSString
//		stringWithFormat:@"PiratesVSNinjas_Session-%@-1.0.pb",
//		
//		#ifdef DEBUG
//			@"debug"
//		#else
//			@"release"
//		#endif
//		
//		]];
//}
//
//@end




#ifndef PVN_ENVIRONMENT
	#define PVN_ENVIRONMENT @"devel"
#endif

static Session* g_sharedSession = 0x0;

Session::Session() :
	delegate_(0),
	units_(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("Pirates.csv"),
		cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("Ninja.csv"))
//	units_([[[NSBundle mainBundle] bundlePath] UTF8String]),
//	d_(new SessionPrivate(this))
{
	experiencePoints_ = 0;
	
//	[[NSNotificationCenter defaultCenter] addObserver:d_->objc_ selector:@selector(outOfSync:)
//		name:kW2ClientRequestOutOfSyncNotification object:nil];
//	[[NSNotificationCenter defaultCenter] addObserver:d_->objc_ selector:@selector(doubleLogin:)
//		name:kW2ClientRequestDoubleLoginNotification object:nil];

//	NSDictionary* config = [NSDictionary dictionaryWithContentsOfFile:[[[NSBundle mainBundle] bundlePath]
//			stringByAppendingPathComponent:@"Config.plist"]];
//	
//	d_->config_ = [NSMutableDictionary dictionaryWithDictionary:config[@"Environments"][@"_ALL"]];
//	
//	NSMutableDictionary* env = [[config objectForKey:@"Environments"] objectForKey:PVN_ENVIRONMENT];
//	assert(env);
//	
//	for (NSString* key in env)
//		d_->config_[key] = env[key];
//	
//	d_->client_ = [[W2Client alloc] initWithHost:[d_->config_ objectForKey:@"host"] pathPrefix:@"api/1.0"];
//	d_->client_.port = [[d_->config_ objectForKey:@"port"] intValue];
//	d_->client_.securePort = [[d_->config_ objectForKey:@"secure_port"] intValue];
//	
//	if (d_->client_.port == d_->client_.securePort);
//		d_->client_.overrideSecure = YES;
//		
//	avatarsCache_.setClient((__bridge void*)(d_->client_));
	
	load();
	
//	if (!user_.isValid())
//	{
//		// we don't have a valid user
//		connect();
//	}
//	else
//	{
//		// synchronize
//		synchronize();
//	}
}

pb::Message* Session::serialized() const
{
	using namespace pb;
	
	Message* m = new Message();
	
	*m
		<< msg(1, user_.serialized())
		
		<< bytes(2, loginKey_)
		<< bytes(3, sessionKey_)
		
		<< varint::i(201, experiencePoints_);
		
//	for (list<string>::const_iterator i=notifyGameStarted_.begin(),
//		e=notifyGameStarted_.end(); i!=e; ++i)
//	{
//		*m << bytes(5, (*i));
//	}
//	
//	for (list<OnlineNotification>::const_iterator i=notifyGameResignedOrExpired_.begin(),
//		e=notifyGameResignedOrExpired_.end(); i!=e; ++i)
//	{
//		*m << msg(6, OnlineNotification::serialized(i->type(), i->user()));
//	}
//	
//	shared_ptr<OnlineGame> active_game = activeOnlineGame();
//	
//	if (active_game)
//	{
//		*m
//			<< bytes(101, active_game->identifier())
//			<< varint::i(102, active_game->gameRound())
//			<< varint::ui(103, active_game->localPlayerState().roundTime);
//	}
	
	return m;
}

void Session::save()
{
	auto_ptr<Message> m(serialized());
	auto_ptr<Buffer> buf(m->serialized());
	
	cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("experience_points", experiencePoints_);
	
	// TODO
	//buf->writeToFile([[monsters_objc_SessionPrivate archivePath] UTF8String]);
}

void Session::load()
{
	experiencePoints_ = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("experience_points", 0);
	if (experiencePoints_ == 0)
	{
		NSString* archive_path = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)
			objectAtIndex:0] stringByAppendingPathComponent:[NSString
			stringWithFormat:@"PiratesVSNinjas_Session-%@-1.0.pb",
			
			#ifdef DEBUG
				@"debug"
			#else
				@"release"
			#endif
			
			]];
	
		Buffer buf([archive_path UTF8String]);
		if (buf.size())
		{
			Message m;
			if (m.deserialize(buf))
			{
				user_ = User(m.getFieldValueMessage(1));
				
				loginKey_ = m.getFieldValueString(2);
				sessionKey_ = m.getFieldValueString(3);
				
	//			for (int i=0; i<m.countValues(5); ++i)
	//			{
	//				notifyGameStarted_.push_back(m.getFieldValueStringAtIndex(5, i));
	//			}
	//			
	//			for (int i=0; i<m.countValues(6); ++i)
	//			{
	//				notifyGameResignedOrExpired_.push_back(OnlineNotification::deserialized(*m.getFieldValueMessageAtIndex(6, i)));
	//			}
				
	//			lastActiveGameId_ = m.getFieldValueString(101);
	//			lastActiveGameRound_ = m.getFieldValueInt32(102);
	//			lastActiveGameRoundTime_ = m.getFieldValueUInt64(103);
				
				experiencePoints_ = m.getFieldValueInt32(201);
				experiencePoints_ += m.getFieldValueInt32(202);
			}
			else
			{
	//			NSLog(@"Couldn't deserialize %@", [monsters_objc_SessionPrivate archivePath]);
			}
		}
	}
}

Session::~Session()
{
//	d_->client_ = nil;
//	d_->config_ = nil;
//	delete d_;
}

//void Session::processUpdate(const Update& update, bool fromLoadState)
//{
//	// TODO
//	switch (update.type())
//	{
//		case Update::Type_GameStarted:
//		{
//			shared_ptr<OnlineInvite> invite = onlineInviteWithId(update.inviteId());
//			shared_ptr<OnlineInvite> active_random_invite = activeRandomGameInvite();
//			removeOnlineInviteWithId(update.inviteId());
//			
//			if (!fromLoadState)
//			{
//				onlineGames_.push_back(update.game());
//			}
//			
//			if (invite == active_random_invite)
//			{
//				if (!startRandomGameCallback_.isNull())
//				{
//					startRandomGameCallback_.call(update.game().get(), Error());
//				}
//				else
//				{
//					notifyGameStarted_.push_back(update.gameId());
//					if (delegate_)
//						delegate_->sessionHasNewNotifications();
//				}
//			}
//			else
//			{
//				notifyGameStarted_.push_back(update.game()->identifier());
//				if (delegate_)
//					delegate_->sessionHasNewNotifications();
//			}
//		}
//		break;
//		
//		case Update::Type_GameConfiguration:
//		{
//			Message msg;
//			if (msg.deserialize(update.data()))
//			{
//				PlayerConfig config(msg, units_);
//				shared_ptr<OnlineGame> game = onlineGameWithId(update.gameId());
//				game->opponentConfigurationUpdated(config);
//			}
//		}
//		break;
//		
//		case Update::Type_GamePlaying:
//		{
//			if (!fromLoadState)
//			{
//				assert(!update.playData().isNull());
//				
//				shared_ptr<OnlineGame> game = onlineGameWithId(update.gameId());
//				if (game)
//				{
//					game->setNotifyPlay(true);
//					game->onPlayCompleted(Error(), update.playData());
//					if (game->isNotifyPlay() && delegate_)
//						delegate_->sessionHasNewNotifications();
//				}
//			}
//		}
//		break;
//		
//		case Update::Type_GameTurn:
//		case Update::Type_GameOver:
//		{
//			if (!fromLoadState)
//			{
//				shared_ptr<OnlineGame> game = onlineGameWithId(update.gameId());
//				if (game)
//				{
//					Message msg;
//					if (msg.deserialize(update.data()))
//					{
//						PlayerAction turn;
//						if (turn.deserialize(msg))
//						{
//							game->setNotifyOpponentTurn(true);
//							game->opponentMadeTurn(turn);
//							if (game->isNotifyOpponentTurn() && delegate_)
//								delegate_->sessionHasNewNotifications();
//						}
//					}
//				}
//			}
//		}
//		break;
//		
//		case Update::Type_GameResigned:
//		{
//			shared_ptr<OnlineGame> game = onlineGameWithId(update.gameId());
//			if (game)
//			{
//				OnlineNotification n(OnlineNotification::Type_GameResigned,
//					game->opponent());
//				
//				game->opponentResigned();
//				
//				if (game->delegate())
//				{
//					// game delegate is set, no need to repeat this as notification
//				}
//				else
//				{
//					notifyGameResignedOrExpired_.push_back(n);
//					if (delegate_)
//						delegate_->sessionHasNewNotifications();
//				}
//			}
//		}
//		break;
//		
//		case Update::Type_InviteReceived:
//		{
//			shared_ptr<OnlineInvite> invite(new OnlineInvite(update.user(), user_, update.inviteId()));
//			receivedInvites_.push_back(invite);
//			
//			if (delegate_)
//				delegate_->sessionHasNewNotifications();
//		}
//		break;
//		
//		case Update::Type_InviteDeclined:
//		{
//			shared_ptr<OnlineInvite> invite = onlineInviteWithId(update.inviteId());
//			if (invite)
//			{
//				notifyInviteDeclined_.push_back(invite->opponent());
//				removeOnlineInviteWithId(update.inviteId());
//			}
//			else
//			{
//				notifyInviteDeclined_.push_back(update.user());
//			}
//			
//			if (delegate_)
//				delegate_->sessionHasNewNotifications();
//		}
//		break;
//		
//		case Update::Type_FriendAdded:
//		{
//			notifyFriendsAdded_.push_back(update.user());
//			friends_.push_back(update.user());
//			
//			if (delegate_)
//				delegate_->sessionHasNewNotifications();
//		}
//		break;
//		
//		case Update::Type_None:
//		case Update::Type_ChatMessage:
//		break;
//		
//		// will eventually be used
//		case Update::Type_InviteWithdrawn:
//		case Update::Type_FriendRequest:
//		case Update::Type_GameWillExpire:
//		case Update::Type_GameExpired:
//		break;
//	}
//}
//
//bool Session::hasNotifications() const
//{
//	for (list<boost::shared_ptr<OnlineGame> >::const_iterator i=onlineGames_.begin(), e=onlineGames_.end();
//		i!=e; ++i)
//	{
//		if ((*i)->isNotifyPlay() || (*i)->isNotifyOpponentTurn())
//			return true;
//	}
//	
//	return notifyFriendsAdded_.size() > 0 ||
//		notifyInviteDeclined_.size() > 0 ||
//		notifyGameStarted_.size() > 0 ||
//		notifyGameResignedOrExpired_.size() > 0 ||
//		receivedInvites_.size() > 0;
//}
//
//void Session::removeNotification(const OnlineNotification& n)
//{
//	if (n.type() == OnlineNotification::Type_GameResigned ||
//		n.type() == OnlineNotification::Type_GameExpired)
//	{
//		for (list<OnlineNotification>::iterator i=notifyGameResignedOrExpired_.begin(),
//			e=notifyGameResignedOrExpired_.end(); i!=e; ++i)
//		{
//			if (i->user() == n.user())
//			{
//				notifyGameResignedOrExpired_.erase(i);
//				return;
//			}
//		}
//	}
//	else if (n.type() == OnlineNotification::Type_GameStarted)
//	{
//		for (list<string>::iterator i=notifyGameStarted_.begin(),
//			e=notifyGameStarted_.end(); i!=e; ++i)
//		{
//			if ((*i) == n.game()->identifier())
//			{
//				notifyGameStarted_.erase(i);
//				return;
//			}
//		}
//	}
//	else if (n.type() == OnlineNotification::Type_FriendAdded)
//	{
//		list<User>::iterator it = find(notifyFriendsAdded_.begin(), notifyFriendsAdded_.end(),
//			n.user());
//		if (it != notifyFriendsAdded_.end())
//		{
//			notifyFriendsAdded_.erase(it);
//		}
//	}
//	else if (n.type() == OnlineNotification::Type_InviteDeclined)
//	{
//		list<User>::iterator it = find(notifyInviteDeclined_.begin(), notifyInviteDeclined_.end(),
//			n.user());
//		if (it != notifyInviteDeclined_.end())
//		{
//			notifyInviteDeclined_.erase(it);
//		}
//	}
//	else if (n.type() == OnlineNotification::Type_GamePlaying)
//	{
//		n.game()->setNotifyPlay(false);
//	}
//	else if (n.type() == OnlineNotification::Type_GameOpponentTurn)
//	{
//		n.game()->setNotifyOpponentTurn(false);
//	}
//}
//
//OnlineNotification Session::nextNotification() const
//{
//	if (notifyFriendsAdded_.size())
//	{
//		return OnlineNotification(OnlineNotification::Type_FriendAdded,
//			notifyFriendsAdded_.front());
//	}
//	
//	if (notifyInviteDeclined_.size())
//	{
//		return OnlineNotification(OnlineNotification::Type_InviteDeclined,
//			notifyInviteDeclined_.front());
//	}
//	
//	if (notifyGameResignedOrExpired_.size())
//	{
//		return notifyGameResignedOrExpired_.front();
//	}
//	
//	if (notifyGameStarted_.size())
//	{
//		shared_ptr<OnlineGame> game = onlineGameWithId(notifyGameStarted_.front());
//		return OnlineNotification(OnlineNotification::Type_GameStarted, game);
//	}
//	
//	if (receivedInvites_.size())
//	{
//		shared_ptr<OnlineInvite> invite = receivedInvites_.front();
//		return OnlineNotification(OnlineNotification::Type_InviteReceived, invite);
//	}
//	
//	for (list<boost::shared_ptr<OnlineGame> >::const_iterator i=onlineGames_.begin(), e=onlineGames_.end();
//		i!=e; ++i)
//	{
//		if ((*i)->isNotifyPlay())
//			return OnlineNotification(OnlineNotification::Type_GamePlaying, *i);
//		
//		if ((*i)->isNotifyOpponentTurn())
//			return OnlineNotification(OnlineNotification::Type_GameOpponentTurn, *i);
//	}
//	
//	return OnlineNotification();
//}
//
//void Session::loadSyncData(const JSONVariant& data)
//{
//	JSONVariant session = data.objectForKey("session");
//	loginKey_ = session.objectForKey("login_key").asString();
//	sessionKey_ = session.objectForKey("session_key").asString();
//	
//	user_ = User(data.objectForKey("user"));
//	
//	experiencePoints_ = data.objectForKey("user").objectForKey("xp").asInteger();
//	addExperiencePoints_ = 0;
//	addingExperiencePoints_ = 0;
//	
//	[d_->client_.signatureParameters setParameterValue:[NSString stringWithUTF8String:user_.identifier().c_str()]
//		forKey:@"user_id"];
//	[d_->client_ setSignatureKey:[NSString stringWithUTF8String:sessionKey_.c_str()]
//		type:W2ClientSignatureType_MD5_HMAC];
//	
//	JSONVariant state = data.objectForKey("state");
//	
//	JSONVariant json_friends = data.objectForKey("friends");
//	friends_.clear();
//	for (int i=0; i<json_friends.objectsCount(); ++i)
//	{
//		friends_.push_back(User(json_friends.objectAtIndex(i)));
//	}
//	
//	map<string, User> users;
//	JSONVariant json_opponents = state.objectForKey("opponents");
//	for (int i=0; i<json_opponents.objectsCount(); ++i)
//	{
//		User user(json_opponents.objectAtIndex(i));
//		users.insert(pair<string, User>(user.identifier(), user));
//	}
//	users.insert(pair<string, User>(user_.identifier(), user_));
//	
//	JSONVariant json_invites = state.objectForKey("invites");
//	sentInvites_.clear();
//	receivedInvites_.clear();
//	for (int i=0; i<json_invites.objectsCount(); ++i)
//	{
//		shared_ptr<OnlineInvite> invite(new OnlineInvite(json_invites.objectAtIndex(i), users));
//		if (invite->sender() == user_)
//			sentInvites_.push_back(invite);
//		else
//			receivedInvites_.push_back(shared_ptr<OnlineInvite>(invite));
//	}
//	
//	JSONVariant json_games = state.objectForKey("games");
//	onlineGames_.clear();
//	for (int i=0; i<json_games.objectsCount(); ++i)
//	{
//		shared_ptr<OnlineGame> online_game(new OnlineGame(json_games.objectAtIndex(i), user_, users, units_));
//		if (online_game->identifier() == lastActiveGameId_)
//		{
//			if (online_game->gameRound() == lastActiveGameRound_)
//				online_game->setLocalPlayerStateRoundTime(lastActiveGameRoundTime_);
//		}
//		
//		onlineGames_.push_back(online_game);
//	}
//	
//	string active_game_id = state.objectForKey("active_game").asString();
//	if (active_game_id.size())
//		activeGame_ = onlineGameWithId(active_game_id);
//	else
//		activeGame_.reset();
//		
//	notifyFriendsAdded_.clear();
//	notifyInviteDeclined_.clear();
//	notifyGameStarted_.clear();
//	notifyGameResignedOrExpired_.clear();
//	
//	JSONVariant json_updates = state.objectForKey("updates");
//	for (int i=0; i<json_updates.objectsCount(); ++i)
//	{
//		processUpdate(Update(json_updates.objectAtIndex(i), user_, users, units_), true);
//	}
//	
//	updatesToken_ = state.objectForKey("updates_token").asString();
//	
//	if (hasNotifications() && delegate_)
//		delegate_->sessionHasNewNotifications();
//	
//	save();
//}

//void Session::connect()
//{
//	connecting_ = true;
//	stopReceivingUpdates();
//		
//	NSString* device_id = nil;
//	if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)])
//		device_id = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
////	else if ([[UIDevice currentDevice] respondsToSelector:@selector(uniqueIdentifier)])
////		device_id = [[UIDevice currentDevice] performSelector:@selector(uniqueIdentifier)];
//		
//	NSString* name = [UIDevice currentDevice].name;
//	if ([[name lowercaseString] hasSuffix:@"'s iphone"])
//		name = [name substringToIndex:[name length] - [@"'s iphone" length]];
//	else if ([[name lowercaseString] hasSuffix:@"'s ipad"])
//		name = [name substringToIndex:[name length] - [@"'s ipad" length]];
//	else if ([[name lowercaseString] hasSuffix:@"'s ipod"])
//		name = [name substringToIndex:[name length] - [@"'s ipod" length]];
//	
//	if (!device_id)
//	{
//		device_id = [[NSUserDefaults standardUserDefaults] objectForKey:@"generatedDeviceId"];
//		if (!device_id)
//		{
//			device_id = W2CreateUUID();
//			[[NSUserDefaults standardUserDefaults] setObject:device_id forKey:@"generatedDeviceId"];
//		}
//	}
//	
//	[d_->client_ postSecure:@"login" sign:NO
//		params:@{
//			@"platform": @"ios",
//			@"device_id": device_id,
//			@"name": name,
//			@"language": @"en"
//		}
//		completion:^(W2ClientRequest *request) {
//			Error err = d_->parseError(request.error);
//			
//			if (err.isOk())
//			{
//				loadSyncData(JSONVariant((__bridge void*)request.response));
//				connecting_ = false;
//				d_->afterConnection(err);
//				startReceivingUpdates();
//				uploadExperiencePoints();
//			}
//			else
//			{
//				d_->afterConnection(err);
//				
//				// connect again in 5 secs
//				dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^(void){
//					connect();
//				});
//			}
//		}];
//}
//
//void Session::startReceivingUpdates()
//{
//	d_->updatesRequest_ = nil;
//	receiveUpdates();
//}
//
//void Session::stopReceivingUpdates()
//{
//	[d_->updatesRequest_ cancel];
//	d_->updatesRequest_ = nil;
//}
//
//void Session::receiveUpdates()
//{
//	W2ClientRequestCompletionBlock completion = ^(W2ClientRequest *request)
//	{
//		if (request == d_->updatesRequest_)
//		{
//			if (request.error)
//			{
//				dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)),
//					dispatch_get_main_queue(), ^(void){
//						receiveUpdates();
//					});
//			}
//			else
//			{
//				JSONVariant res((__bridge void*)request.response);
//			
//				updatesToken_ = res.objectForKey("updates_token").asString();
//				
//				map<string, User> opponents;
//				JSONVariant json_opponents = res.objectForKey("opponents");
//				for (int i=0; i<json_opponents.objectsCount(); ++i)
//				{
//					User user(json_opponents.objectAtIndex(i));
//					opponents.insert(pair<string, User>(user.identifier(), user));
//				}
//				
//				JSONVariant updates = res.objectForKey("updates");
//				for (int i=0; i<updates.objectsCount(); ++i)
//				{
//					processUpdate(Update(updates.objectAtIndex(i), user_, opponents, units_), false);
//				}
//				
//				receiveUpdates();
//			}
//		}
//	};
//	
//	if (updatesToken_.size())
//		d_->updatesRequest_ = [d_->client_ post:@"updates/remove-and-retrieve"
//			params:@{ @"updates_token": [NSString stringWithUTF8String:updatesToken_.c_str()] }
//			completion:completion];
//	else
//		d_->updatesRequest_ = [d_->client_ get:@"updates" completion:completion];
//}
//
//void Session::synchronize()
//{
//	connecting_ = true;
//	stopReceivingUpdates();
//	
//	[d_->client_ postSecure:@"login" sign:NO
//		params:@{
//			@"login_key": [NSString stringWithUTF8String:loginKey_.c_str()],
//			@"user_id": [NSString stringWithUTF8String:user_.identifier().c_str()]
//		}
//		completion:^(W2ClientRequest *request) {
//			Error err = d_->parseError(request.error);
//			
//			if (err.isOk())
//			{
//				loadSyncData(JSONVariant((__bridge void*)request.response));
//				connecting_ = false;
//				d_->afterConnection(err);
//				startReceivingUpdates();
//				uploadExperiencePoints();
//				
//				if (delegate_)
//					delegate_->sessionDidSyncronized();
//			}
//			else
//			{
//				// TODO: check for permanently fail condition
//				// synchronize again in 5 secs
//				dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^(void){
//					synchronize();
//				});
//			}
//		}];
//}
//
//void Session::outOfSync()
//{
//	if (delegate_)
//		delegate_->sessionIsOutOfSync();
//	synchronize();
//}
//
//void Session::doubleLogin()
//{
//	if (delegate_)
//		delegate_->sessionDoubleLogin();
//	
//	user_ = User();
//	loginKey_ = "";
//	sessionKey_ = "";
//	connect();
//}
//
//void Session::facebookLogin(const LoginCallback& callback)
//{
//	LoginCallback the_callback = callback;
//	
//	[FBSession openActiveSessionWithReadPermissions:nil allowLoginUI:YES
//		completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
//			switch (status)
//			{
//				case FBSessionStateOpen:
//				{
//					[[FBRequest requestForMe] startWithCompletionHandler:^(FBRequestConnection *connection,
//						NSDictionary<FBGraphUser> *user, NSError *error)
//					{
//						if (!error)
//						{
//							d_->requireActiveConnection(^(monsters::Error error) {
//								[d_->client_ postSecure:@"link/facebook"
//									params:[NSDictionary dictionaryWithObjectsAndKeys:
//										user.id, @"fb_id",
//										session.accessTokenData.accessToken, @"fb_access_token",
//										nil
//									]
//									completion:^(W2ClientRequest *request) {
//										
//										Error err = d_->parseError(request.error);
//										
//										if (err.isOk())
//										{
//											loadSyncData(JSONVariant((__bridge void*)request.response));
//											startReceivingUpdates();
//											uploadExperiencePoints();
//										}
//										
//										the_callback.call(err);
//									}
//								];
//							});
//						}
//						else
//						{
//							the_callback.call(d_->parseError(error));
//						}
//					}];
//					
//					
//				}
//				break;
//				
//				case FBSessionStateClosedLoginFailed:
//				{
//					[FBSession.activeSession closeAndClearTokenInformation];
//					the_callback.call(d_->parseError(error));
//				}
//				break;
//				
//				default:
//				{
//					the_callback.call(d_->parseError(error));
//				}
//				break;
//			}
//		}];
//}
//
//boost::shared_ptr<OnlineGame> Session::activeOnlineGame() const
//{
//	if (activeGame_ && activeGame_->isActive())
//		return activeGame_;
//	
//	for (list<shared_ptr<OnlineGame> >::const_iterator i=onlineGames_.begin(), e=onlineGames_.end(); i!=e; ++i)
//	{
//		if ((*i)->isActive())
//		{
//			return *i;
//		}
//	}
//	
//	return shared_ptr<OnlineGame>();
//}
//
//boost::shared_ptr<OnlineInvite> Session::activeRandomGameInvite() const
//{
//	for (list<shared_ptr<OnlineInvite> >::const_iterator i=sentInvites_.begin(), e=sentInvites_.end(); i!=e; ++i)
//	{
//		if (!(*i)->receiver().isValid() && (*i)->sender() == user_)
//		{
//			return *i;
//		}
//	}
//	
//	return shared_ptr<OnlineInvite>();
//}
//
//boost::shared_ptr<OnlineGame> Session::onlineGameWithId(const std::string& gameId) const
//{
//	for (list<shared_ptr<OnlineGame> >::const_iterator i=onlineGames_.begin(),
//		e=onlineGames_.end(); i!=e; ++i)
//	{
//		if ((*i)->identifier() == gameId)
//		{
//			return *i;
//		}
//	}
//	
//	return shared_ptr<OnlineGame>();
//}
//
//boost::shared_ptr<OnlineGame> Session::onlineGameSharedPtr(OnlineGame* game)
//{
//	for (list<shared_ptr<OnlineGame> >::const_iterator i=onlineGames_.begin(),
//		e=onlineGames_.end(); i!=e; ++i)
//	{
//		if (i->get() == game)
//		{
//			return *i;
//		}
//	}
//	
//	return shared_ptr<OnlineGame>();
//}
//
//boost::shared_ptr<OnlineInvite> Session::onlineInviteWithId(const std::string& inviteId)
//{
//	for (list<shared_ptr<OnlineInvite> >::const_iterator i=sentInvites_.begin(),
//		e=sentInvites_.end(); i!=e; ++i)
//	{
//		if ((*i)->identifier() == inviteId)
//			return *i;
//	}
//	
//	for (list<shared_ptr<OnlineInvite> >::const_iterator i=receivedInvites_.begin(),
//		e=receivedInvites_.end(); i!=e; ++i)
//	{
//		if ((*i)->identifier() == inviteId)
//			return *i;
//	}
//	
//	return shared_ptr<OnlineInvite>();
//}
//
//
//void Session::removeOnlineInviteWithId(const std::string& inviteId)
//{
//	for (list<shared_ptr<OnlineInvite> >::iterator i=sentInvites_.begin(),
//		e=sentInvites_.end(); i!=e; ++i)
//	{
//		if ((*i)->identifier() == inviteId)
//		{
//			sentInvites_.erase(i);
//			return;
//		}
//	}
//	
//	for (list<shared_ptr<OnlineInvite> >::iterator i=receivedInvites_.begin(),
//		e=receivedInvites_.end(); i!=e; ++i)
//	{
//		if ((*i)->identifier() == inviteId)
//		{
//			receivedInvites_.erase(i);
//			return;
//		}
//	}
//}
//
//void Session::removeOnlineGame(const boost::shared_ptr<OnlineGame>& onlineGame)
//{
//	onlineGames_.remove(onlineGame);
//	if (activeGame_ == onlineGame)
//		activeGame_.reset();
//	notifyGameStarted_.remove(onlineGame->identifier());
//	//notifyGameStarted_.erase(find(notifyGameStarted_.begin(), notifyGameStarted_.end(), onlineGame->identifier()));
//}
//
//void Session::getActiveOnlineGame(const GetActiveOnlineGameCallback& callback)
//{
//	GetActiveOnlineGameCallback the_callback = callback;
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			the_callback.call(activeOnlineGame().get(), error);
//		}
//		else
//		{
//			the_callback.call((OnlineGame*)0, error);
//		}
//	});
//}
//
//void Session::startRandomGame(const Session::StartRandomGameCallback& callback)
//{
//	startRandomGameCallback_ = callback;
//	
//	if (activeRandomGameInvite()) // already started
//		return;
//	
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			[d_->client_ post:@"games/new/random" params:@{}
//				completion:^(W2ClientRequest *request) {
//					Error error = d_->parseError(request.error);
//					if (error.isOk())
//					{
//						JSONVariant res((__bridge void*)request.response);
//						string invite_id = res.objectForKey("invite_id").asString();
//						sentInvites_.push_back(shared_ptr<OnlineInvite>(new OnlineInvite(user_, User(), invite_id)));
//					}
//					else
//					{
//						startRandomGameCallback_.call((OnlineGame*)0, error);
//					}
//				}];
//		}
//		else
//		{
//			startRandomGameCallback_.call((OnlineGame*)0, error);
//		}
//	});
//}
//
//void Session::cancelRandomGame()
//{
//	startRandomGameCallback_ = StartRandomGameCallback();
//	
//	shared_ptr<OnlineInvite> invite = activeRandomGameInvite();
//	if (invite)
//	{
//		d_->requireActiveConnection(^(monsters::Error error)
//		{
//			if (error.isOk())
//			{
//				[d_->client_ post:[NSString stringWithFormat:@"invites/%s/withdraw", invite->identifier().c_str()]
//					params:@{ }
//					completion:^(W2ClientRequest *request) {
//						Error error = d_->parseError(request.error);
//						if (error.isOk())
//						{
//							sentInvites_.remove(invite);
//						}
//					}];
//			}
//		});
//	}
//}
//
//void Session::inviteUserToGame(const User& user, const OperationCallback& callback)
//{
//	OperationCallback the_callback = callback;
//	User opponent = user;
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			[d_->client_ post:[NSString stringWithFormat:@"games/new/%s", opponent.identifier().c_str()]
//				params:@{ }
//				completion:^(W2ClientRequest *request) {
//					Error error = d_->parseError(request.error);
//					if (error.isOk())
//					{
//						JSONVariant res((__bridge void*)request.response);
//						string invite_id = res.objectForKey("invite_id").asString();
//						sentInvites_.push_back(shared_ptr<OnlineInvite>(new OnlineInvite(user_, opponent, invite_id)));
//					}
//					
//					the_callback.call(error);
//				}];
//		}
//		else
//		{
//			the_callback.call(error);
//		}
//	});
//}
//
//bool Session::isUserInvited(const User& user) const
//{
//	for (list<boost::shared_ptr<OnlineInvite> >::const_iterator i=sentInvites_.begin(),
//		e=sentInvites_.end(); i!=e; ++i)
//	{
//		if ((*i)->receiver() == user)
//		{
//			return true;
//		}
//	}
//	
//	return false;
//}
//
//void Session::acceptInvite(const std::string& inviteId, const ProcessInviteCallback& callback)
//{
//	processInvite(inviteId, true, callback);
//}
//
//void Session::declineInvite(const std::string& inviteId, const ProcessInviteCallback& callback)
//{
//	processInvite(inviteId, false, callback);
//}
//
//void Session::processInvite(const std::string& inviteId, bool accept, const ProcessInviteCallback& callback)
//{
//	ProcessInviteCallback the_callback = callback;
//	std::string invite_id = inviteId;
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			[d_->client_ post:[NSString stringWithFormat:@"invites/%s/%@", invite_id.c_str(),
//					accept ? @"accept" : @"decline"]
//				params:@{ }
//				completion:^(W2ClientRequest *request) {
//					Error error = d_->parseError(request.error);
//					if (error.isOk())
//					{
//						if (accept)
//						{
//							JSONVariant res((__bridge void*)request.response);
//							shared_ptr<OnlineInvite> invite = onlineInviteWithId(invite_id);
//							
//							map<string, User> opponents;
//							opponents.insert(pair<string, User>(invite->opponent().identifier(), invite->opponent()));
//							shared_ptr<OnlineGame> online_game(new OnlineGame(res, user_, opponents, units_));
//							onlineGames_.push_back(online_game);
//							
//							removeOnlineInviteWithId(invite_id);
//							the_callback.call(online_game.get(), error);
//						}
//						else
//						{
//							removeOnlineInviteWithId(invite_id);
//							the_callback.call((OnlineGame*)0, error);
//						}
//					}
//					else
//					{
//						the_callback.call((OnlineGame*)0, error);
//					}
//				}];
//		}
//		else
//		{
//			the_callback.call((OnlineGame*)0, error);
//		}
//	});
//}
//
//void Session::getFriendsList(const GetUsersListCallback& callback)
//{
//	GetUsersListCallback block_callback = callback;
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			block_callback.call(friends_, error);
//		}
//		else
//		{
//			block_callback.call(vector<User>(), error);
//		}
//    });
//}
//
//void Session::findUsersByName(const std::string& query, const GetUsersListCallback& callback)
//{
//	GetUsersListCallback block_callback = callback;
//	d_->requireActiveConnection(^(monsters::Error error)
//	{
//		if (error.isOk())
//		{
//			[d_->client_ get:[NSString stringWithFormat:@"search"]
//				params:@{
//					@"name": [NSString stringWithFormat:@"%s", query.c_str()]
//				}
//				completion:^(W2ClientRequest *request) {
//					
//					vector<User> users;
//					
//					if (!request.error)
//					{
//						JSONVariant response = JSONVariant((__bridge void*)request.response);
//						
//						JSONVariant json_users = response.objectForKey("users");
//						for (int i =0 ; i < json_users.objectsCount(); ++i)
//						{
//							users.push_back(User(json_users.objectAtIndex(i)));
//						}
//					}
//					
//					Error error = d_->parseError(request.error);
//					block_callback.call(users, error);
//				}];
//		}
//		else
//		{
//			block_callback.call(vector<User>(), error);
//		}
//	});
//}
//
//void Session::configureOnlineGame(OnlineGame* game, const PlayerConfig& config)
//{
//	auto_ptr<Message> msg(config.serialized());
//	auto_ptr<Buffer> buf(msg->serialized());
//	shared_ptr<OnlineGame> game_ptr = onlineGameSharedPtr(game);
//	
//	[d_->client_ post:[NSString stringWithFormat:@"games/%s/configure", game->identifier().c_str()]
//		params:@{
//			@"config": [NSString stringWithUTF8String:buf->toBase64().data()]
//		}
//		completion:^(W2ClientRequest *request) {
//			Error error = d_->parseError(request.error);
//			game_ptr->onConfigureCompleted(error);
//		}];
//}
//
//void Session::playOnlineGame(OnlineGame* game, const PlayerConfig& config)
//{
//	auto_ptr<Message> msg(config.serialized());
//	auto_ptr<Buffer> buf(msg->serialized());
//	shared_ptr<OnlineGame> game_ptr = onlineGameSharedPtr(game);
//	
//	[d_->client_ post:[NSString stringWithFormat:@"games/%s/play", game->identifier().c_str()]
//		params:@{
//			@"config": [NSString stringWithUTF8String:buf->toBase64().data()]
//		}
//		completion:^(W2ClientRequest *request) {
//			Error error = d_->parseError(request.error);
//			if (error.isOk())
//			{
//				JSONVariant res((__bridge void*)request.response);
//				if (res.objectForKey("status").asString() == "playing")
//				{
//					game->onPlayCompleted(error, res.objectForKey("data"));
//				}
//			}
//			else
//			{
//				game->onPlayCompleted(error, JSONVariant());
//			}
//			
//			//game_ptr->onConfigureCompleted(error);
//		}];
//}
//
//OperationHandle Session::sendTurnOnlineGame(OnlineGame* game, const PlayerAction& turn)
//{
//	auto_ptr<Message> msg(turn.serialized());
//	auto_ptr<Buffer> buf(msg->serialized());
//	shared_ptr<OnlineGame> game_ptr = onlineGameSharedPtr(game);
//	
//	__block OperationHandle h = (__bridge OperationHandle)[d_->client_
//		post:[NSString stringWithFormat:@"games/%s/turn", game->identifier().c_str()]
//		params:@{
//			@"turn": [NSString stringWithUTF8String:buf->toBase64().data()]
//		}
//		completion:^(W2ClientRequest *request)
//		{
//			Error error = d_->parseError(request.error);
//			game->onSendTurnCompleted(h, error);
//		}];
//	
//	return h;
//}
//
//void Session::setActiveGame(OnlineGame* game, const OperationCallback& callback)
//{
//	if (game != activeGame_.get())
//	{
//		OperationCallback the_callback = callback;
//		[d_->client_ post:[NSString stringWithFormat:@"games/%s/activate", game->identifier().c_str()]
//			params:@{ }
//			completion:^(W2ClientRequest *request) {
//				Error error = d_->parseError(request.error);
//				if (error.isOk())
//				{
//					activeGame_ = onlineGameSharedPtr(game);
//					the_callback.call(Error());
//				}
//				else
//				{
//					the_callback.call(error);
//				}
//			}];
//	}
//	else
//	{
//		callback.call(Error());
//	}
//}
//
//void Session::resignOnlineGame(OnlineGame* game)
//{
//	[d_->client_ post:[NSString stringWithFormat:@"games/%s/resign", game->identifier().c_str()]
//		params:@{ }
//		completion:^(W2ClientRequest *request) {
//			Error error = d_->parseError(request.error);
//			if (error.isOk())
//			{
//				shared_ptr<OnlineGame> game_ptr = onlineGameSharedPtr(game);
//				removeOnlineGame(game_ptr);
//				game->onResignCompleted(error);
//			}
//			else
//			{
//				game->onResignCompleted(error);
//			}
//		}];
//}
//
//void Session::uploadExperiencePoints()
//{
//	User xp_user = user_;
//	if (addExperiencePoints_)
//	{
//		d_->requireActiveConnection(^(monsters::Error error)
//		{
//			if (xp_user == user_)
//			{
//				addingExperiencePoints_ = addExperiencePoints_;
//				addExperiencePoints_ = 0;
//				
//				[d_->client_ post:@"profile/xp/add"
//					params:@{ @"points": @(addingExperiencePoints_) }
//					completion:^(W2ClientRequest *request)
//					{
//						if (xp_user == user_)
//						{
//							Error error = d_->parseError(request.error);
//							if (error.isOk())
//							{
//								JSONVariant res((__bridge void*)request.response);
//								if (!res.objectForKey("xp").isNull())
//									experiencePoints_ = res.objectForKey("xp").asInteger();
//								addingExperiencePoints_ = 0;
//							}
//							else
//							{
//								addExperiencePoints_ += addingExperiencePoints_;
//								addingExperiencePoints_ = 0;
//								
//								// retry
//								dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
//									(int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^(void)
//								{
//									uploadExperiencePoints();
//								});
//							}
//						}
//					}];
//			}
//		});
//	}
//}

int Session::experiencePoints() const
{
	return experiencePoints_;
	//isFacebookLoggedIn() ? (experiencePoints_ + addingExperiencePoints_ + addExperiencePoints_) : 0;
}

void Session::addExperiencePoints(int points)
{
	experiencePoints_ += points;
//	uploadExperiencePoints();
}

Session& Session::sharedSession()
{
	if (!g_sharedSession)
	{
		g_sharedSession = new Session();
	}
	
	return *g_sharedSession;
}

//void Session::closeGame(boost::shared_ptr<OnlineGame> game)
//{
//	onlineGames_.remove(game);
//	if (activeGame_ == game)
//		activeGame_.reset();
//}
