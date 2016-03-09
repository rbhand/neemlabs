//
//  LoadingIndicatorLayer.h
//  PiratesVSNinjas
//
//  Created by Sergey Gerasimov on 3/29/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "Utils.h"
#include <list>

using namespace cocos2d;
using namespace cocos2d_utils;

namespace monsters
{
	class LoadingIndicatorLayer : public CCLayerColor
	{
	public:
		bool init();
		
		CREATE_FUNC(LoadingIndicatorLayer);
		
		void setLinkedNodes(CCArray* linkedNodes);
		
		void setCaption(const std::string& caption);
		void show();
		void hide();
		
	private:
		void setLinkedNodesVisible(CCArray* linkedNodes, bool visible);
		
	private:
		CCLabelTTF* caption_;
		CCSprite* loadingIndicator_;
		CCArray* linkedNodes_;
		std::list<CCNode*> nodesHidden_;
	};
}