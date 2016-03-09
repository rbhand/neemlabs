//
//  NextRoundLayer.h
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 2/26/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Settings.h"

namespace monsters
{
	
	class INextRoundLayerDelegate
	{
	public:
		//virtual void nextRoundLayerBack() = 0;
	};
	
	class NextRoundLayer : public cocos2d::CCLayerRGBA//, public cocos2d::CCRGBAProtocol
	{
	public:
		virtual bool init();
		
		void setDelegate(INextRoundLayerDelegate* delegate) { delegate_ = delegate; }
		INextRoundLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(NextRoundLayer);
		
		void setPlayerName(const char* name);
		
//		void setOpacity(GLubyte opacity);
//		GLubyte getOpacity(void) { return bg_->getOpacity(); }
//		void setColor(const cocos2d::ccColor3B& color);
//		const cocos2d::ccColor3B& getColor(void) { return bg_->getColor(); }
//		void setOpacityModifyRGB(bool bValue) { bg_->setOpacityModifyRGB(bValue); }
//		bool isOpacityModifyRGB(void) { return bg_->isOpacityModifyRGB(); }
		
	private:
		INextRoundLayerDelegate* delegate_;
		
		void show();
		void hide();
		
		cocos2d::CCSprite* bg_;
		cocos2d::CCLabelBMFont* label_;
		
	};
	
	
	
}
