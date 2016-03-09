//
//  Utils.cpp
//  ClashOfMonsters
//
//  Created by Vitaliy Ivanov on 6/28/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#include "Utils.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#define W2IsMac (NO)

#ifdef __IPHONE_4_0

#define W2IsIPad   (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#define W2IsIPhone (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
#define W2IsIPadHD  (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && [[UIScreen mainScreen] scale] == 2)

#else

#define W2IsIPad   (NO)
#define W2IsIPhone (YES)

#endif

#define W2IsIPhoneRetina (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && \
	([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2))

#define W2IsIPhoneRetina4Inch (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && \
	([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2) && \
	CGRectGetHeight([UIScreen mainScreen].bounds) * [[UIScreen mainScreen] scale] == 1136.0f )

//#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//
//#define W2IsIPad   (NO)
//#define W2IsIPhone (NO)
//#define W2IsMac    (YES)
//#define W2IsIPhoneRetina (NO)
//#define W2IsIPadRetina (NO)

#define kDFFontHUDSmall (W2IsIPadHD ? @"badaboom-4.fnt" : \
	((W2IsIPad || W2IsIPhoneRetina || W2IsMac) ? @"badaboom-2.fnt" :\
	@"badaboom.fnt"))

#endif





using namespace cocos2d;
using namespace std;

cocos2d::CCMenuItemSprite* cocos2d_utils::menuItemSpriteHighlighted(const char* spriteFrameName,
	cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector)
{
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	selected_sprite->setColor(ccc3(120, 120, 120));
	return CCMenuItemSprite::create(normal_sprite, selected_sprite,
		target, selector);
}

cocos2d::CCMenuItemSprite* cocos2d_utils::menuItemSpriteHighlightedWithDisabledState(const char* spriteFrameName,
	cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector)
{
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	selected_sprite->setColor(ccc3(120, 120, 120));
	CCSprite* disabled_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	disabled_sprite->setOpacity(100);
	return CCMenuItemSprite::create(normal_sprite, selected_sprite, disabled_sprite,
		target, selector);
}

cocos2d::CCMenuItemSprite* cocos2d_utils::menuItemSpriteHighlightedAndFlipped(const char* spriteFrameName,
	cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector)
{
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	normal_sprite->setFlipX(true);
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	selected_sprite->setColor(ccc3(120, 120, 120));
	selected_sprite->setFlipX(true);
	return CCMenuItemSprite::create(normal_sprite, selected_sprite,
		target, selector);
}

void cocos2d_utils::menuItemSpriteHighlightedChangeSprite(cocos2d::CCMenuItemSprite* itemSprite, const char* spriteFrameName)
{
	CCSprite* normal_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	CCSprite* selected_sprite = CCSprite::createWithSpriteFrameName(spriteFrameName);
	selected_sprite->setColor(ccc3(120, 120, 120));
	itemSprite->setNormalImage(normal_sprite);
	itemSprite->setSelectedImage(selected_sprite);
}


std::string cocos2d_utils::fileNameWithoutExtension(const std::string& baseName)
{
	size_t i = baseName.find_last_of(".");
	if (i != string::npos)
		return baseName.substr(0, i);
		
	return baseName;
}

std::string cocos2d_utils::fileExtension(const std::string& baseName)
{
	size_t i = baseName.find_last_of(".");
	if (i != string::npos)
		return baseName.substr(i+1);
		
	return "";
}

std::string cocos2d_utils::textureName(const std::string& fileName)
{
	std::string base_name = fileNameWithoutExtension(fileName);
	std::string ext = fileExtension(fileName);
	std::string size;
	
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (W2IsIPad)
	{
		size = [[UIScreen mainScreen] scale] == 1 ? "2" : "4";
	}
	else
	{
		size = [[UIScreen mainScreen] scale] == 1 ? "1" : "2";
	}
	#else
	size = createWithFormat("%d", getNearestDeviceSize());
	#endif
	
	return base_name + "-" + size + "." + ext;
}


std::string cocos2d_utils::defaultPNGFileName()
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (W2IsIPad)
	{
		return [[UIScreen mainScreen] scale] == 1 ? "Default~ipad.png" : "Default@2x~ipad.png";
	}
	else if (W2IsIPhoneRetina4Inch)
	{
		return "Default-568h@2x.png";
	}
	else
	{
		return [[UIScreen mainScreen] scale] == 1 ? "Default.png" : "Default@2x.png";
	}
	#endif
	
	return "Default.png";
}


bool cocos2d_utils::isFileExists(const std::string& fileName)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	
	return ([[NSFileManager defaultManager] fileExistsAtPath:[NSString
		stringWithUTF8String:fileName.c_str()]] == YES);
	
	#endif

	return CCFileUtils::sharedFileUtils()->isFileExist(fileName.c_str());
}

float cocos2d_utils::ct(float coordinate)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return ((W2IsIPad) ? ((coordinate) * 2.0) : (coordinate));
	#else
	return coordinate;
	#endif
}

cocos2d::CCPoint cocos2d_utils::pt(const cocos2d::CCPoint& p)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return ((W2IsIPad) ? (ccpMult(p, 2.0)) : (p));
	#else
	return p;
	#endif
}

bool cocos2d_utils::isScreen43()
{
    CCSize raw_screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
    // normalized to portrait orientation size
    CCSize screen_size = CCSize(raw_screen_size.width < raw_screen_size.height ? raw_screen_size.width : raw_screen_size.height,
        raw_screen_size.width < raw_screen_size.height ? raw_screen_size.height : raw_screen_size.width);
    return screen_size.width / 3 == screen_size.height / 4;
}

int cocos2d_utils::getNearestDeviceSize()
{
	CCSize raw_screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
    // normalized to portrait orientation size
    CCSize screen_size = CCSize(raw_screen_size.width < raw_screen_size.height ? raw_screen_size.width : raw_screen_size.height,
        raw_screen_size.width < raw_screen_size.height ? raw_screen_size.height : raw_screen_size.width);

    if (!isScreen43())
    {
        float aspect = (float)screen_size.height / (float)screen_size.width;
        if (aspect >= 1.5)
        {
            // pick device size based on width
            if (screen_size.width <= (320 + 100))
                return 1;
            else if (screen_size.width <= (640 + 200))
                return 2;
            else
                return 4;
        }
        else
        {
            // pick device size based on height
            if (screen_size.height<= (480 + 100))
                return 1;
            else if (screen_size.height <= (960 + 200))
                return 2;
            else
                return 4;
        }
    }
    else
    {
        if (screen_size.width < (768 + 300))
            return 2;
        else
            return 4;
    }

	return 1;
}

float cocos2d_utils::getScreenAspectRatio()
{
    CCSize raw_screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
    // normalized to portrait orientation size
    CCSize screen_size = CCSize(raw_screen_size.width < raw_screen_size.height ? raw_screen_size.width : raw_screen_size.height,
        raw_screen_size.width < raw_screen_size.height ? raw_screen_size.height : raw_screen_size.width);
    return (float)screen_size.height / (float)screen_size.width;
}

bool cocos2d_utils::isIPhone()
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return W2IsIPhone ? true : false;
	#else
	return !isScreen43();
	#endif
}

bool cocos2d_utils::isIPhone5()
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return W2IsIPhoneRetina4Inch ? true : false;
	#else
	return false;
	#endif
}

bool cocos2d_utils::isRetina()
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return ([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2) ?
		true : false;
	#endif
	
	return getNearestDeviceSize() > 1;
}

void cocos2d_utils::showAlert(const char* title, const char* message, const char* cancelButton)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[[[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title]
		message:[NSString stringWithUTF8String:message] delegate:nil
		cancelButtonTitle:[NSString stringWithUTF8String:cancelButton] otherButtonTitles:nil]
			autorelease] show];
	#else
	CCMessageBox(title, message);
	#endif
}

std::string cocos2d_utils::createWithFormat(const char* format, ...)
{
	char* pBuf = (char*)malloc(1024*2);
	assert(pBuf);
	std::string str;
	if (pBuf != NULL)
	{
		va_list ap;
		va_start(ap, format);
		vsnprintf(pBuf, 1024*2, format, ap);
		va_end(ap);
		str = pBuf;
		free(pBuf);
	}
	
	return str;
}

CCRect cocos2d_utils::intersectRectangles(const CCRect& rect1, const CCRect& rect2)
{
	if (rect1.intersectsRect(rect2))
	{
		return CCRectMake(std::max(rect1.origin.x, rect2.origin.x), std::max(rect1.origin.y, rect2.origin.y),
			(std::min(rect1.origin.x + rect1.size.width, rect2.origin.x + rect2.size.width) - std::max(rect1.origin.x, rect2.origin.x)),
			(std::min(rect1.origin.y + rect1.size.height, rect2.origin.y + rect2.size.height)) - std::max(rect1.origin.y, rect2.origin.y));
	}
	else
		return CCRectMake(0,0,0,0);
}
