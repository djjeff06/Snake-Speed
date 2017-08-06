#pragma once

#include "cocos2d.h"

class EndWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(EndWorld);
	void EndWorld::GoToGameScene(cocos2d::Ref *sender);
};
