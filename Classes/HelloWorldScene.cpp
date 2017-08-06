#include "HelloWorldScene.h"
#include "EndWorld.h"
#include <cstdlib>
#include <string>

; USING_NS_CC;

static int score;


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	static int direction = 0;

	score = 0;

	static int mov = 10;

	auto label = Label::createWithSystemFont(std::to_string(score), "arial", 32);
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setPosition(580, 420);
	this->addChild(label, 2);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	auto snake = Sprite::create("snakeblock.png");
	snake->setScale(0.5);

	snake->setAnchorPoint(Vec2(0.5, 0.5));
	snake->setPosition(Vec2(300, 300));
	this->addChild(snake, 1);

	auto moveBy = MoveBy::create(0.2, Point(10, 0));
	auto repeat = cocos2d::Repeat::create(moveBy, 100);
	snake->runAction(repeat);


	//background
	auto bg = Sprite::create("bg_grass.png");
	bg->setScale(100.0);
	this->addChild(bg, 0);

	//food
	auto food = Sprite::create("burger.png");

	static int randx = random() % 640;
	static int randy = random() % 480;
	food->setPosition(Vec2(randx, randy));
	food->setScale(0.5);
	this->addChild(food, 1);

	auto eventListener = EventListenerKeyboard::create();


	auto eatFood = CallFunc::create([this, snake, food,label]() {

		float distance = powf(food->getPositionX() - snake->getPositionX(), 2) + powf(food->getPositionY() - snake->getPositionY(), 2);
		distance = sqrt(distance);

		if (distance <= 25) {

			randx = random() % 640;
			randy = random() % 480;
			food->setPosition(Point(randx, randy));
			
			score++;
			this->removeChild(label);
			auto label = Label::createWithSystemFont(std::to_string(score), "arial", 32);
			label->setAnchorPoint(Vec2(0.5, 0.5));
			label->setPosition(580, 420);
			this->addChild(label, 2);
			mov += 10;

		}

	});

	auto gameover = CallFunc::create([snake, visibleSize]() {

		bool check = false;
		// check left
		if (snake->getPositionX()< 0)
			check = true;
		//check right
		else if (snake->getPositionX() > visibleSize.width)
			check = true;
		//check bot
		else if (snake->getPositionY() < 0)
			check = true;
		//check top
		else if (snake->getPositionY() > visibleSize.height)
			check = true;

		if (check == true) {

			auto scene = EndWorld::createScene();

			Director::getInstance()->replaceScene(scene);

		}

	});

	/* Changing direction faced by the snake
	0 : RIGHT
	1 : LEFT
	2 : UP
	3 : DOWN
	*/
	auto setRotate = CallFunc::create([snake]() {

		switch (direction) {

		case 0: snake->setRotation(0);
			break;
		case 1: snake->setRotation(+180.0f);
			break;
		case 2: snake->setRotation(-90.0f);
			break;
		case 3: snake->setRotation(+90.0f);
			break;

		}

	});


	auto seq = Sequence::create(setRotate, eatFood, gameover, nullptr);
	auto rep = cocos2d::RepeatForever::create(seq);
	snake->runAction(rep);

	eventListener->onKeyPressed = [snake, repeat](EventKeyboard::KeyCode keyCode, Event* event) {

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			//event->getCurrentTarget()->setPosition(loc.x-4, loc.y);
			if (direction != 0) {

				event->getCurrentTarget()->stopAction(repeat);
				auto moveBy = MoveBy::create(0.2, Point(-mov, 0));
				auto repeat = cocos2d::Repeat::create(moveBy, 100);
				snake->runAction(repeat);
				direction = 1;

			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			//event->getCurrentTarget()->setPosition(loc.x+4, loc.y);
			if (direction != 1) {

				event->getCurrentTarget()->stopAction(repeat);
				auto moveBy = MoveBy::create(0.2, Point(mov, 0));
				auto repeat = cocos2d::Repeat::create(moveBy, 100);
				snake->runAction(repeat);
				direction = 0;

			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			//event->getCurrentTarget()->setPosition(loc.x, loc.y+4);
			if (direction != 3) {

				event->getCurrentTarget()->stopAction(repeat);
				auto moveBy = MoveBy::create(0.2, Point(0, mov));
				auto repeat = cocos2d::Repeat::create(moveBy, 100);
				snake->runAction(repeat);
				direction = 2;

			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			//event->getCurrentTarget()->setPosition(loc.x, loc.y-4);
			if (direction != 2) {

				event->getCurrentTarget()->stopAction(repeat);
				auto moveBy = MoveBy::create(0.2, Point(0, -mov));
				auto repeat = cocos2d::Repeat::create(moveBy, 100);
				snake->runAction(repeat);
				direction = 3;

			}
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, snake);

	return true;
}