//
//  WaitingForOpponentLayer.h
//  PiratesVSNinjas
//
//  Created by Vitaliy Ivanov on 4/19/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "User.h"
#include "NavigationMenuLayer.h"

namespace monsters
{
	class IWaitingForOpponentLayerDelegate
	{
	public:
		virtual void waitingForOpponentCancel() = 0;
	};

	class WaitingForOpponentLayer : public NavigationMenuLayer
	{
	public:
		bool init();
		
		void setDelegate(IWaitingForOpponentLayerDelegate* delegate) { delegate_ = delegate; }
		IWaitingForOpponentLayerDelegate* delegate() const { return delegate_; }
		
		CREATE_FUNC(WaitingForOpponentLayer);
		
		void setOpponent(const User& opponent);
		
		void onShow();
		void onHide();
		
	private:
		void cancelButtonPressed();
		
	private:
		cocos2d::CCLabelTTF* outOpponent_;
		IWaitingForOpponentLayerDelegate* delegate_;
	};
}
