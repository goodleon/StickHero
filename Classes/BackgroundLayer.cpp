/*
 * BackgroundLayer.cpp
 *
 *  Created on: 2015年4月11日
 *      Author: Administrator
 */

#include "BackgroundLayer.h"
using namespace CocosDenshion;
bool BackgroundLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	MyWinSize = Director::getInstance()->getVisibleSize();
	/**
	 * 随机设置壁纸
	 */
	int RandomNumber = CCRANDOM_0_1()*10;
	int bgNumber = RandomNumber % 5;
	switch (bgNumber) {
	case 0:
		Image_One = Sprite::create("res/image/bg/bg1.jpg");
		Image_Two = Sprite::create("res/image/bg/bg1.jpg");
		break;
	case 1:
		Image_One = Sprite::create("res/image/bg/bg2.jpg");
		Image_Two = Sprite::create("res/image/bg/bg2.jpg");
		break;
	case 2:
		Image_One = Sprite::create("res/image/bg/bg3.jpg");
		Image_Two = Sprite::create("res/image/bg/bg3.jpg");
		break;
	case 3:
		Image_One = Sprite::create("res/image/bg/bg4.jpg");
		Image_Two = Sprite::create("res/image/bg/bg4.jpg");
		break;
	case 4:
		Image_One = Sprite::create("res/image/bg/bg5.jpg");
		Image_Two = Sprite::create("res/image/bg/bg5.jpg");
		break;
	default:
		break;
	}

	Image_One->setPosition(MyWinSize.width / 2, MyWinSize.height / 2);
	Image_Two->setPosition(
			MyWinSize.width / 2 + Image_Two->getContentSize().width,
			MyWinSize.height / 2);

	this->addChild(Image_One, 1);
	this->addChild(Image_Two, 1);

	GameName = Sprite::create("res/image/uires_1.png");
	GameName->setPosition(MyWinSize.width / 2, MyWinSize.height - GameName->getContentSize().height);
	this->addChild(GameName, 2);

	auto startBtn = MenuItemSprite::create(
			Sprite::create("res/image/uires_2.png"),
			Sprite::create("res/image/uires_2.png"), NULL,
			CC_CALLBACK_1(BackgroundLayer::Start,this));
	startBtn->setPosition(MyWinSize.width / 2, MyWinSize.height / 2 + 8);
	auto startBtnMoveDown = MoveTo::create(2,
			Vec2(MyWinSize.width / 2, MyWinSize.height / 2 - 8));
	auto startBtnMoveUp = MoveTo::create(2,
			Vec2(MyWinSize.width / 2, MyWinSize.height / 2 + 5));
	auto startMoveSeq = Sequence::create(startBtnMoveDown, startBtnMoveUp,
	NULL);
	auto startMoveRepeat = RepeatForever::create(startMoveSeq);
	menu = Menu::create(startBtn, NULL);
	menu->setPosition(0, 0);

	this->addChild(menu, 2);

	/*auto _testSprite = Sprite::create("CloseNormal.png");
	 _testSprite->setPosition(Point(100, 100));
	 this->addChild(_testSprite);*/


	log("width : %f , height : %f", MyWinSize.width, MyWinSize.height);

	/*
	 * 初始化三个stage
	 */
	for (int i = 0; i < 3; i++) {
		stageSprite[i] = Sprite::create("res/image/stage1.png");
	}
	stageSprite[0]->setScaleX(30);//设置固定宽度

	for (int i = 1; i < 3; i++) {
		stageSprite[i]->setPosition(
				Vec2(MyWinSize.width+ stageSprite[i]->getScaleX()* stageSprite[i]->getContentSize().width,
						stageSprite[i]->getContentSize().height / 2));
	}

	stageSprite[0]->setPosition(
			Vec2(MyWinSize.width / 2,
					stageSprite[0]->getContentSize().height / 4));

	for (int i = 0; i < 3; i++) {
		this->addChild(stageSprite[i], 3);
	}

	/**
	 * 添加监听器,并绑定监听事件
	 */
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(false);
	touchListener->onTouchBegan = CC_CALLBACK_2(BackgroundLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(BackgroundLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(BackgroundLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(BackgroundLayer::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	/**
	 * 添加英雄
	 */
	player = new Player();
	player->init();
 	player->getPlayer()->setPosition(Vec2(MyWinSize.width/2,stageSprite[0]->getContentSize().height/4*3));
 	this->addChild(player->getPlayer(),10);
 	player->Stay();

 	stick = Sprite::create("res/image/stick1.png");
 	stick->setAnchorPoint(Vec2(0.5,0));
 	stick->setPosition(-stick->getContentSize().width,-stick->getContentSize().height);
 	this->addChild(stick,5);

	return true;
}
void BackgroundLayer::Start(Ref* pSender) {

	log("%s","BackgroundLayer::Start()");
	auto MyWinSize = Director::getInstance()->getVisibleSize();
	log("width : %f , height : %f", MyWinSize.width, MyWinSize.height);
	isStart=true;//表示游戏开始

	this->removeChild(menu);
	this->removeChild(GameName);

	auto stageMove = MoveTo::create(0.2,Vec2(100, stageSprite[0]->getContentSize().height / 2));
	stageSprite[0]->runAction(stageMove);

	//bgMove(Vec2(-MyWinSize.width/2+100,0));

 	player->getPlayer()->runAction(MoveTo::create(0.2,Vec2(100, stageSprite[0]->getContentSize().height)));

 	StickPoint=Vec2(100+stageSprite[0]->getScaleX()* stageSprite[0]->getContentSize().width/2, stageSprite[0]->getContentSize().height);

	addStage();
//
}

void BackgroundLayer::addStage() {

	log("%s","BackgroundLayer::addStage()---->");

	stageSprite[stageNumber]->setScaleX(10 + CCRANDOM_0_1()*40);
	stageSprite[stageNumber]->setPosition(
			Vec2(MyWinSize.width+ stageSprite[stageNumber]->getScaleX()* stageSprite[stageNumber]->getContentSize().width,
					stageSprite[stageNumber]->getContentSize().height / 2));

	auto random = MyWinSize.width / 2 +CCRANDOM_0_1()*MyWinSize.width/3;
	auto stageMove = MoveTo::create(0.2,
			Vec2(random ,
			     stageSprite[stageNumber]->getContentSize().height/2));



	stageSprite[stageNumber]->runAction(stageMove);



	if (stageNumber + 1 <= 2) {
		stageNumber += 1;
	} else {
		stageNumber = 0;
	}
}
void BackgroundLayer::stageMove() {

	log("BackgroundLayer::stageMove()-->stageNumber:%d",stageNumber);

	/**
	 * 停止背景移动
	 */
	this->unschedule(schedule_selector(BackgroundLayer::bgMove));

	/*
	 * 停止播放走路音效（kick.ogg）
	 */
	SimpleAudioEngine::getInstance()->pauseEffect(kickId);
	NowStage = stageNumber == 0 ? 2 : stageNumber - 1;
	LastStage = NowStage == 0 ? 2 : (NowStage - 1);
	NextStage = NowStage == 2 ? 0 : (NowStage + 1);
	MoveTo* nowStageMove = MoveTo::create(0.2,Vec2(100, stageSprite[0]->getContentSize().height / 2));
	stageSprite[NowStage]->runAction(nowStageMove);
	MoveTo* lastStageMove = MoveTo::create(0.2,
			Vec2(-stageSprite[LastStage]->getContentSize().width * stageSprite[LastStage]->getScaleX(),
					stageSprite[0]->getContentSize().height / 2));
	stageSprite[LastStage]->runAction(lastStageMove);

	//player->getPlayer()->Walk(Vec2());

//	bgMove(Vec2(100-stageSprite[NowStage]->getPositionX(),0));


	StickPoint = Vec2(100+stageSprite[NowStage]->getScaleX()*stageSprite[NowStage]->getContentSize().width/2,
			stageSprite[0]->getContentSize().height);
	initStick();

	/*
	 * 跟随stage进行移动
	 */
	player->getPlayer()->runAction(MoveTo::create(0.2,Vec2(100, stageSprite[0]->getContentSize().height )));

	addStage();
	//addStick();

	successFlag = false;//重置successFlag
}

bool BackgroundLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	log("%s","BackgroundLayer::onTouchBegan()");
	if(isStart){
		//stageMove();
		addStick();
	}

    return true;
}
void BackgroundLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
}

void BackgroundLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	if(isStart){
		StopStick();
		RotateStickAndGo();
	}
}

void BackgroundLayer::addStick()
{
	log("%s","BackgroundLayer::addStick()");
	stick->setScaleY(1);//还原棍子
	stick->setRotation(0);
    stick->setPosition(StickPoint);
    this->schedule(schedule_selector(BackgroundLayer::StickLength));
}

void BackgroundLayer::StickLength(float)
{
	//log("%s","BackgroundLayer::StickLength()");
    stick->setScaleY(stick->getScaleY()+STICK_SPEED);
}
void BackgroundLayer::StopStick()
{
	log("%s","BackgroundLayer::StopStick()");
    TouchLength = stick->getContentSize().height*stick->getScaleY();
    this->unschedule(schedule_selector(BackgroundLayer::StickLength));
}

void BackgroundLayer::RotateStickAndGo(){
//	DestLengthMin = abs(stageSprite[LastStage]->getPositionX() - stageSprite[NowStage]->getPositionX()) - stageSprite[LastStage]->getContentSize().width*stageSprite[LastStage]->getScaleX()/2 - stageSprite[NowStage]->getContentSize().width*stageSprite[NowStage]->getScaleX()/2;
	DestLengthMin = abs(stageSprite[NextStage]->getPositionX() - stageSprite[NowStage]->getPositionX()) - stageSprite[NextStage]->getContentSize().width*stageSprite[NextStage]->getScaleX()/2 - stageSprite[NowStage]->getContentSize().width*stageSprite[NowStage]->getScaleX()/2;

	DestLengthMax = DestLengthMin + stageSprite[NextStage]->getContentSize().width*stageSprite[NextStage]->getScaleX();
	RotateTo* Ro_Stick = RotateTo::create(1, 90); //旋转90度
	RotateTo* RoDown_Stick = RotateTo::create(1,180);

	log("ToTouchLength:%f,DestLengthMin:%f,DestLengthMax:%f",TouchLength,DestLengthMin,DestLengthMax);

	Ro_Stick->setTag(1);
	if(TouchLength<DestLengthMin || TouchLength > DestLengthMax)
	{

		scoreCount++;//记录分数
		successFlag=false;
	    //stick->runAction(RoDown_Stick);
	}
	else if(TouchLength >= DestLengthMin && TouchLength <=DestLengthMax)
	{
		successFlag=true;
	   // stick->runAction(Ro_Stick);
	}

	log("BackgroundLayer::RotateStickAndGo()-->NextStage:%d",NextStage);
	log("BackgroundLayer::RotateStickAndGo()-->%d",successFlag);

	FiniteTimeAction *actionN ;
	if(successFlag==true){
		actionN= Sequence::create(Ro_Stick,
					 CallFuncN::create(CC_CALLBACK_0(BackgroundLayer::stickCallBack,this,successFlag)),
					 NULL);
	}else{
		actionN= Sequence::create(Ro_Stick,
			 CallFuncN::create(CC_CALLBACK_0(BackgroundLayer::stickCallBack,this,successFlag)),
			 NULL);
	}

	stick->runAction(actionN);
}

void BackgroundLayer::stickCallBack(bool successFlag){


	 kickId = SimpleAudioEngine::getInstance()->playEffect("res/sound/kick.ogg",false);

	if(successFlag==true){

	auto playMove = MoveTo::create(0.2,Vec2( stageSprite[NextStage]->getPositionX(),stageSprite[NextStage]->getContentSize().height));

	/*
	 * palyer走到stage之后，stage再进行移动。
	 */
	auto playSeq  =Sequence::create(playMove,
				CallFuncN::create(CC_CALLBACK_0(BackgroundLayer::stageMove,this)),
				NULL
				);

	player->getPlayer()->runAction(playSeq);
	/*
	 *背景进行移动
	 */
	this->schedule(schedule_selector(BackgroundLayer::bgMove));
		moveComplete = true;

	}else{

		auto playPoint = Vec2(
				  stageSprite[NowStage]->getPositionX()
				  + stageSprite[NowStage]->getContentSize().width*stageSprite[NowStage]->getScaleX()/2
				  + TouchLength
				  + player->getPlayer()->getContentSize().width/3,
				    stageSprite[NextStage]->getContentSize().height
			 );
		//player掉落之后的位置
		auto playerDownPoint = Vec2(
				  stageSprite[NowStage]->getPositionX()
				  + stageSprite[NowStage]->getContentSize().width*stageSprite[NowStage]->getScaleX()/2
				  + TouchLength
				  + player->getPlayer()->getContentSize().width/3,
				    -10);
		auto playerMove = MoveTo::create(0.2,playPoint);
		auto playerDown = MoveTo::create(0.2,playerDownPoint);
		auto playerSeq = Sequence::create(playerMove,
				                          playerDown,
										  NULL
				            );
		player->getPlayer()->runAction(playerSeq);
		RotateTo* RoDown_Stick = RotateTo::create(1,180);
		stick->runAction(RoDown_Stick);

		/*
		 * 游戏结束，显示从新开始界面
		 */
		gameOverLayer = GameOverLayer::create();

		this->addChild(gameOverLayer,8);

	}
}

void BackgroundLayer::initStick(){

	log("BackgroundLayer::hideStick()");

	stick->setScaleY(1);//还原棍子
	stick->setRotation(0);
	stick->setPosition(StickPoint);
}

void BackgroundLayer::stopAudio(int audioId){

}

void BackgroundLayer::bgMove(float){

	log("BackgroundLayer::bgMove()-->");


	int X1 = Image_One->getPositionX();
	int X2 = Image_Two->getPositionX();
	int speed = 50;
	X1 -= speed;
	X2 -= speed;
	Size size = Image_One->getContentSize();
	if (X1 < -size.width / 2) {
		X2 = size.width / 2;
		X1 = size.width / 2 + size.width;
	}
	if (X2 < -size.width / 2) {
		X1 = size.width / 2;
		X2 = size.width / 2 + size.width;
	}
	Image_One->setPositionX(X1);
	Image_Two->setPositionX(X2);

}



