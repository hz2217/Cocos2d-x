#include "GameScene.h"
#include "stdlib.h"
USING_NS_CC;
auto stoneLayer = Layer::create();
auto mouseLayer = Layer::create();
auto stone = Sprite::create("stone.png");
auto stoneShoot = Sprite::create("stone.png");

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// background
	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	// layer for stone and mouse 
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));

	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("general-sheet.plist");
	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	mouse->setPosition(Vec2(origin.x + visibleSize.width / 2, 0));
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));

	stoneLayer = Layer::create();
	stoneLayer->setPosition(Vec2(0, 0));
	stoneLayer->setAnchorPoint(Vec2(0, 0));
	stoneLayer->ignoreAnchorPointForPosition(false);
	stoneLayer->addChild(stone);

	mouseLayer = Layer::create();
	mouseLayer->setPosition(Vec2(0, origin.y + visibleSize.height / 2));
	mouseLayer->setAnchorPoint(Vec2(0, 0));
	mouseLayer->ignoreAnchorPointForPosition(false);
	mouseLayer->addChild(mouse);
	
	this->addChild(stoneLayer);
	this->addChild(mouseLayer);

	// shoot
	auto shoot = Label::createWithSystemFont("Shoot", "Marker Felt", 80);
	auto menuItem = MenuItemLabel::create(shoot, CC_CALLBACK_1(GameSence::shootMenuCallback, this));
	menuItem->setPosition(Vec2(visibleSize.width + origin.x - 150, visibleSize.height + origin.y - 140));

	auto menu = Menu::create(menuItem, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {
	auto location = touch->getLocation();
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(Vec2(location));
	this->addChild(cheese);

	auto moveTo = MoveTo::create(1.0f, mouseLayer->convertToNodeSpace(location));
	mouse->runAction(moveTo);

	auto delayTime = DelayTime::create(2.0f);
	auto func = CallFunc::create([this, cheese]() {
		this->removeChild(cheese);
	});
	auto seq = Sequence::create(delayTime, func, NULL);
	this->runAction(seq);
	return true;
}

void GameSence::shootMenuCallback(cocos2d::Ref* pSender) {
	stoneShoot = Sprite::create("stone.png");
	stoneShoot->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stoneShoot);
	auto location = mouse->getPosition();
	location = mouseLayer->convertToWorldSpace(location);
	auto moveTo = MoveTo::create(1, stoneLayer->convertToNodeSpace(location));
	stoneShoot->runAction(moveTo);

	auto delayTime = DelayTime::create(1.0f);
	auto func = CallFunc::create([this, location]() {

		int x = random() % 1000;
		int y = random() % 1000;
		auto _location = Point(x, y);
		auto moveTo = MoveTo::create(1, mouseLayer->convertToNodeSpace(_location));
		mouse->runAction(moveTo);
		/*
		Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
		leg->runAction(RepeatForever::create(legAnimate));
		*/
		auto diamond = Sprite::create("diamond.png");
		Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
		diamond->runAction(RepeatForever::create(diamondAnimate));
		diamond->setPosition(this->convertToNodeSpace(location));
		this->addChild(diamond);

		auto _delayTime = DelayTime::create(0.5);
		auto _func = CallFunc::create([this]() {
			stoneLayer->removeChild(stoneShoot);
			stoneShoot = Sprite::create("stone.png");
			stoneShoot->setPosition(Vec2(560, 480));
			stoneLayer->addChild(stoneShoot);
		});
		auto _seq = Sequence::create(_delayTime, _func, NULL);
		this->runAction(_seq);
	});
	auto seq = Sequence::create(delayTime, func, NULL);
	this->runAction(seq);
}
