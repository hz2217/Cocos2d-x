# include "HelloWorldScene.h"
# include "ui/CocosGUI.h"
//# include "Monster.h"
//# define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace cocos2d::ui;

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
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
}
