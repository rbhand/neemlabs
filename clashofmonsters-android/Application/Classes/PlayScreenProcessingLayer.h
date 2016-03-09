//
//  PlayScreenProcessingLayer.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 5/17/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Settings.h"

namespace monsters
{
	class PlayScreenProcessingLayer : public cocos2d::CCLayerColor//, public cocos2d::CCRGBAProtocol
	{
	public:
		virtual bool init();
		
		CREATE_FUNC(PlayScreenProcessingLayer);
		
		void setTitle(const char* title);
		
//		void setOpacity(GLubyte opacity);
//		GLubyte getOpacity(void) { return bg_->getOpacity(); }
//		void setColor(const cocos2d::ccColor3B& color);
//		const cocos2d::ccColor3B& getColor(void) { return bg_->getColor(); }
//		void setOpacityModifyRGB(bool bValue) { bg_->setOpacityModifyRGB(bValue); }
//		bool isOpacityModifyRGB(void) { return bg_->isOpacityModifyRGB(); }
	
	private:
		bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		
	private:
		cocos2d::CCSprite* bg_;
		cocos2d::CCLabelBMFont* label_;
	};
	
}
