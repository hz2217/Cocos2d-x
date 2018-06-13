# 实验报告（Cocos2d-x_Study note）

### 学号：16340216

### 姓名：王健豪

### 班级：软工五班

### 

[TOC]

## 2018.05.29 - week12 - hw11

参考资料：ppt 和 blog

```C++
// 创建倒计时
	time = Label::createWithTTF("120", "fonts/arial.ttf", 36);
	time->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-30));
	this->addChild(time);
	schedule(schedule_selector(HelloWorld::updateTime), 1);
	dtime = 120;
```

```C++
// 从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	// 使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height/2));
	addChild(player, 3);
```

```C++
// 死亡动画(帧数：22，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79*i, 0, 90, 79)));
		dead.pushBack(frame);
	}
	auto dead_animation = Animation::createWithSpriteFrames(dead, 0.1f);
	dead_animation->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(dead_animation, "deadAnimation");
```

```C++
// 方向键系列
	auto buttonW = Button::create("W.png", "W.png");
	buttonW->setPosition(Vec2(origin.x + 60, origin.y + 60));
// W键事件处理函数
	buttonW->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			schedule(schedule_selector(HelloWorld::moveW), 0.3f);
			break;
		case ui::Widget::TouchEventType::ENDED:
			unschedule(schedule_selector(HelloWorld::moveW));
			break;
		}
	});
// 技能键X的事件处理函数
	buttonX->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if (isDone == false) return; // 动画未完成，不能执行新动画
			else isDone = false; // 开始执行动画
			if (pT->getPercentage() == 0) return; // 进度条为0时不可再执行该动画
			auto callFunc = CallFunc::create([&] {isDone = true; }); // 定义动画执行完毕的回调函数
			auto seq = Sequence::createWithTwoActions(Animate::create(AnimationCache::getInstance()->getAnimation("deadAnimation")), callFunc);
			player->runAction(seq);
			if (pT->getPercentage() - 40 >= 0) { // 每次X操作减少进度条40
				pT->setPercentage(pT->getPercentage() - 40);
			}
			else {
				pT->setPercentage(0);
			}
			break;
		}
	});
```

```C++
// 方向键W的移动
void HelloWorld::moveW(float dt) {
	auto location = player->getPosition();
	if (location.y + 20 >= visibleSize.height) {
		auto seq = Sequence::createWithTwoActions(MoveBy::create(0.6f, Vec2(0, visibleSize.height - location.y - 10)),
			Animate::create(AnimationCache::getInstance()->getAnimation("runAnimation")));
		player->runAction(seq);
		return;
	}
	auto seq = Sequence::createWithTwoActions(MoveBy::create(0.6f, Vec2(0, 20)),
		Animate::create(AnimationCache::getInstance()->getAnimation("runAnimation")));
	player->runAction(seq);
}
```

## 2018.05.10 - week11 - hw10

1. 在menuscene中新建start跳转，跳转函数和图片层级 https://blog.csdn.net/gzy252050968/article/details/50524383
2. 在gamescene中新建背景，layer 和 锚点的设置定位坐标 https://blog.csdn.net/xuguangsoft/article/details/8425623
3. https://blog.csdn.net/u013517637/article/details/45483865 
4. 添加动画的基础设置 对照脚步跳动的动画
5. 晚上点击事件 放奶酪 老鼠来吃 奶酪消失 
6. 完善点击shoot函数，老鼠跑掉 放下钻石 http://www.cnblogs.com/slysky/p/3824773.html

## 2018.05.09 - week9 - hw9

<u>准备</u>

1. 安装 python2，官网下载

   - 安装时，选择最下面有红叉的搭建路径

2. 安装 Cocos2d-x

3. 用 命令行新建 cocos 项目

   ```powershell
   cocos new name -p com.sysu.edu -l cpp [-d filepath]
   -p 包名 -l 开发语言 -d 项目存放目录 new 项目名称

   Bug: ***utf-8 不对，无法运行某个 python 文件
   Debug: 问题在于，因为他有个从别的地方拷贝的操作，而我新建项目地方路径名有中文，导致 py 字符格式问题
   ```

4. 运行示例项目

   1. ```powershell
      Bug: 无法访问某个文件夹，及替换文件
      Debug: 因为我把程序装在 C 盘了，没有权限，于是放在 Usrs\zero 下

      Bug: cocos2dx运行 VS报错 找不到 windows sdk 版本8.1
      Debug: 项目->右键->重定项目目标->Windows SDK 版本->选择10.0.14393.0（当然是你当前拥有的Windows SDK版本就可以）->确定

      Bug: 无法查找或打开 PDB 文件解决办法
      Debug: 工具->选项->调试->常规->启用源服务器支持
      			        ->符号->Windows符号服务器

      Bug: 应用程序无法正常启动(0xc000007b)，请单击“确定”关闭应用程序/找不打 mvscr110.dll 等
      Debug: Microsoft DirectX  工具修复（关键）
      ```

<u>HelloWorld</u> [Reference](http://www.cnblogs.com/geore/p/5798176.html)

Cocos2d-x_Menu

1. Understand

   1. cocos2d-x 游戏引擎中的菜单是由菜单类 Menu 和菜单项 MenuItem 来实现的

   2. Menu 是 MenuItem 的一个容器，里面可以添加若干个菜单项 MenuItem

   3. 定义菜单项，然后用它们定义初始化菜单 Menu 实例，最后将Menu实例加入 Layer 中显示出来

      ![](https://images2015.cnblogs.com/blog/1012080/201608/1012080-20160823095453776-928633002.png)

   4. MenuItem 又有若干个子类，来实现不同样式的菜单，例如，文本菜单，字体菜单，图片菜单等

   ![](https://images2015.cnblogs.com/blog/1012080/201608/1012080-20160823095540245-607572292.png)

2. Function

   1. ```c++
      menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
      		origin.y + visibleSize.height - visibleSize.height / 2));
      // 这里的 x，y 是从左下角开始的，visibleSize.height 应该是整个屏幕的高
      ```

   2. ```c++
      menu->alignItemsVerticallyWithPadding(5);
      // 垂直方向以padding间隙排列
      ```

   3. ```c++
      CCMenu及CCMenuItem位置问题/https://blog.csdn.net/u013321328/article/details/21641519
      /** align items vertically */
      void alignItemsVertically(); //垂直方向默认间隙排列

      /** align items vertically with padding
      @since v0.7.2
      */
      void alignItemsVerticallyWithPadding(float padding);//垂直方向以padding间隙排列

      /** align items horizontally */
      void alignItemsHorizontally();//水平方向默认间隙排列

      /** align items horizontally with padding
      @since v0.7.2
      */
      void alignItemsHorizontallyWithPadding(float padding);//水平方向以padding间隙排列

      /** align items in rows of columns */
      void alignItemsInColumns(unsigned int columns, ...);//将items按照列排列
      void alignItemsInColumns(unsigned int columns, va_list args);

      /** align items in columns of rows */
      void alignItemsInRows(unsigned int rows, ...);//将items按照行排列
      void alignItemsInRows(unsigned int rows, va_list args);
      ```



<u>MenuItem</u>

1. MenuItemFont
2. MenuItemImage
3. MenuItemSprite



<u>Code</u>

```c++
MenuItemFont::setFontName("Comic Sans MS.ttf");
MenuItemFont::setFontSize(18);

auto nameItem = MenuItemFont::create("WangJianHao", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
auto idItem = MenuItemFont::create("16340216", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
auto imageItem = MenuItemImage::create(
    "ZhongShanNormal.png",
    "ZhongShanSelected.png",
    CC_CALLBACK_1(HelloWorld::displayMusic, this));

auto menu = Menu::create(nameItem, idItem, imageItem, NULL);
menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height - visibleSize.height / 2));

menu->alignItemsVerticallyWithPadding(5);
this->addChild(menu, 3);
```

```C++
// 音乐播放
void HelloWorld::displayMusic(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_FILE);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	if (count % 2 == 0)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FILE, true);
	else
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	count++;
}
```

