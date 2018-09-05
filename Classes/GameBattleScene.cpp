#include "GameBattleScene.h"
#include "GameData.h"
#include "MainScene2.h"
USING_NS_CC;

Scene *GameBattleScene::createScene(BattlePoint* p)
{
	auto scene = Scene::create();
	auto layer = GameBattleScene::create(p);
	layer->setColor(Color3B(0, 0, 0));
	scene->addChild(layer);
	return scene;

}
GameBattleScene* GameBattleScene::create(BattlePoint* p)
{
	auto battleLayer = new GameBattleScene();
	if (battleLayer->init(p))
	{
		return battleLayer;
	}

}
bool GameBattleScene::init(BattlePoint* battlepoint)
{

	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	this->setContentSize(winSize);

	m_battleEnemies = battlepoint->m_enemySwords;    //设置敌人列表
	m_battleBrides = GameData::shareGameData()->getCurrentSwordTeam();   
	//设置剑姬列表，从数据存储类中获取，当前可获取4队，只用1队进行战斗

	m_battleWeather = battlepoint->m_weather;  //设置天气

	m_battleTeam1.clear();

	for (int i = 0; i < 6; i++)
	{
		SwordBride* oneBride = m_battleBrides.at(i);

		oneBride->setBrideDef(200+i);
		oneBride->setBrideAvd(i);
		oneBride->setBrideDex(90 + i);
		oneBride->setBrideFortune(20+i);    //测试数据，应从系统中存储的编队列表中获取


		m_battleTeam1.push_back(oneBride);
		m_allSwords.push_back(oneBride);
	}
	for (int i = 0; i < m_battleEnemies.size(); i++)
	{
		SwordBride* oneEnemy = m_battleEnemies.at(i);
		oneEnemy->setBrideDef(200 + i);
		oneEnemy->setBrideAvd(i);
		oneEnemy->setBrideDex(90 + i);
		oneEnemy->setBrideFortune(20 + i);   //测试数据，应从json文件中获取

		m_allSwords.push_back(oneEnemy);
	}
	//m_allSwords为玩家剑姬加上敌人的列表，用于进行排列

	m_atkSequence = createBrideSequence(m_allSwords);

	//创建画面，左边为剑姬的显示，右边为敌人的显示

	 m_bridesSpriteVec.clear();
	m_enemiesSpriteVec.clear();

	for (int i = 0; i < m_battleTeam1.size(); i++)
	{
		Sprite* brideSp = Sprite::create("GameBattleScene/BattleKuang.png");  //根据剑姬可选择不同的图像，图像的素材待定 
																				//精灵的创建
		brideSp->setPosition(Point(100,700-i*50));
		this->addChild(brideSp,3,10+i);   //剑姬在layer中的ID从10开始

		LabelAtlas* valueLabel = LabelAtlas::create("0123456789", "GameBattle\number.png", 15, 25, '0');  //显示伤害数值的控件
		valueLabel->setPosition(Point(brideSp->getPosition().x + 20, brideSp->getPosition().y));   //放到我方的右边，参数待定
		valueLabel->setString("0");   //默认值为0？
		valueLabel->setVisible(false);       //默认不显示
		brideSp->addChild(valueLabel,1);


		m_bridesSpriteVec.push_back(brideSp);
	}
	for (int i = 0; i < m_battleEnemies.size(); i++)
	{
		Sprite* enemySp = Sprite::create("GameBattleScene/BattleKuang.png");    //根据敌人可选择不同的图像，图像素材待定
		enemySp->setPosition(Point(600,700-i*50));
		this->addChild(enemySp,3,20+i);   //敌人在layer中的ID从20开始


		LabelAtlas* valueLabel = LabelAtlas::create("0123456789", "GameBattle\number.png", 15, 25, '0');  //显示伤害数值的控件
		valueLabel->setPosition(Point(enemySp->getPosition().x - 20, enemySp->getPosition().y));   //放到敌方的右边，参数待定
		valueLabel->setString("0");   //默认值为0？
		valueLabel->setVisible(false);       //默认不显示
		enemySp->addChild(valueLabel, 1);

		m_enemiesSpriteVec.push_back(enemySp);
	}
	



	doAttackAction(m_atkSequence,m_battleTeam1,m_battleEnemies,m_bridesSpriteVec,m_enemiesSpriteVec,1);




	

	return true;
}


std::vector<SwordBride*> GameBattleScene::createBrideSequence(std::vector<SwordBride*>allBrides)   //按照敌我成员的速度进行排序
{
	std::vector<SwordBride*>attkSequence;
	attkSequence.clear();
	int max, count;
	while (allBrides.size() > 0)
	{
		max = -1;
		for (int i = 0; i < allBrides.size(); i++)
		{
			SwordBride* oneSword = allBrides.at(i);
			if (oneSword->getBrideSpd()>max)
				max = oneSword->getBrideSpd();
		}  //首先找到速度最大的值
		count = 0;
		for (std::vector<SwordBride*>::iterator it = allBrides.begin(); it != allBrides.end();)
		{
			//寻找是哪个剑姬的速度最大，依次放入攻击序列中
			SwordBride* oneBride = allBrides.at(count);
			if (oneBride->getBrideSpd() == max)
			{
				attkSequence.push_back(oneBride);
				allBrides.erase(it++);
				break;

			}
			else
			{
				it++;
				count++;
			}
		}
	}//创建攻击序列

	return attkSequence;
}

std::vector<AttackAction> GameBattleScene::createAttackSequence(std::vector<SwordBride*> bridesSequence, std::vector<SwordBride*>playerBrides, std::vector<SwordBride*>enemyBrides)
{
	std::vector<AttackAction> attackSequence;
	int playerSwordCount, enemySwordCount;
	playerSwordCount = playerBrides.size();
	enemySwordCount = enemyBrides.size();
	for (int i = 0; i < bridesSequence.size(); i++)
	{
		SwordBride* oneBride = bridesSequence.at(i);

		if (oneBride->getBrideHp()<=0)
			continue;   //如果攻击者的血量小于0，说明已经死亡，不进行攻击


		AttackAction oneAction;
 
		if (oneBride->isEnemy)
		{        //如果攻击者为敌人，则在敌人中找到他
			for (int j = 0; j < enemyBrides.size(); j++)
			{
				SwordBride* temp = enemyBrides.at(j);
				if (temp == oneBride)
				{
					oneAction.attackSwordID = j;
					break;
				}
			}

		}
		else
		{   //攻击者为玩家
			for (int j = 0; j < playerBrides.size(); j++)
			{
				SwordBride* temp = playerBrides.at(j);
				if (temp == oneBride)
				{
					oneAction.attackSwordID = j;
					break;
				}
			}

		}

		if (playerSwordCount == 0)
		{
			//玩家全部死亡,停止攻击
			break;
		}
		else if (enemySwordCount == 0)
		{
			//敌人全部死亡，也停止攻击 
			break;
		}
		else
		{
			if (oneBride->isEnemy == false)  //玩家剑姬进行攻击
			{
				struct timeval now;
				int randomNum;
				gettimeofday(&now, NULL);   //获取系统当前时间

				unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
				srand(rand_seed);
				randomNum = rand() % enemyBrides.size();
				while (enemyBrides.at(randomNum)->getBrideHp() <= 0)  //找到一个血量大于0的敌人进行攻击
				{
					gettimeofday(&now, NULL);
					rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
					srand(rand_seed);
					randomNum = rand() % enemyBrides.size();
				}
				SwordBride* attackedBride = enemyBrides.at(randomNum);   
				int  attackValue = getAttackValue(oneBride, attackedBride);  //获取攻击造成的伤害数值

				oneAction.attackedSwordID = randomNum;
				oneAction.attackValue = attackValue;   //记录攻击的数值，攻击对象和被攻击对象
				oneAction.isBrideAttack = true;


				if (attackValue == -1)
				{
					attackValue = 0;
					//miss时按0计算
				}

				if (attackedBride->getBrideHp() - attackValue <= 0)
				{
					enemySwordCount--;
					attackedBride->setBrideHP(0);    //如果血量为0，则被打死，剑姬数量减少
					oneAction.isKillSword = true;
				}
				else
				{
					attackedBride->setBrideHP(attackedBride->getBrideHp() - attackValue);   //设置血量减少
					oneAction.isKillSword = false;
				}

		
				

				//写到此处，完善攻击后的数据


			}
			else  //敌人进行进攻
			{
				struct timeval now;
				int randomNum;
				gettimeofday(&now, NULL);
				unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
				srand(rand_seed);
				randomNum = rand() % playerBrides.size();
				while (playerBrides.at(randomNum)->getBrideHp() <= 0)
				{
					gettimeofday(&now, NULL);
					rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
					srand(rand_seed);
					randomNum = rand() % playerBrides.size();
				}

				SwordBride* attackedBride = playerBrides.at(randomNum);
				int attackValue = getAttackValue(oneBride, attackedBride);

				oneAction.attackedSwordID = randomNum;
				oneAction.attackValue = attackValue;  //如果进行攻击的是敌人，则攻击剑姬
				oneAction.isBrideAttack = false;

				if (attackValue == -1)
				{
					attackValue = 0;
					//miss时按照0计算
				}


				
				if (attackedBride->getBrideHp() - attackValue <= 0)
				{
					playerSwordCount--;
					attackedBride->setBrideHP(0);   //如果血量为0，则被打死，剑姬数量减少
					oneAction.isKillSword = true;
				}
				else
				{
					attackedBride->setBrideHP(attackedBride->getBrideHp() - attackValue);
					oneAction.isKillSword = false;
				}
						
			}

			attackSequence.push_back(oneAction);   //将创建的攻击加入到序列中

		}


	}
	
	return attackSequence;
}


void GameBattleScene::doAttackAction(std::vector<SwordBride*> bridesSequence, std::vector<SwordBride*>playerBrides, std::vector<SwordBride*>enemyBrides,
	std::vector<Sprite*>brideSpriteVec,std::vector<Sprite*>enemySpriteVec,int count)
{
	std::vector<AttackAction>attackActionVec= createAttackSequence(bridesSequence, playerBrides, enemyBrides); //获取攻击序列
	

	for (int i = 0; i < attackActionVec.size(); i++)
	{
		AttackAction oneAction = attackActionVec.at(i);
		if (oneAction.isBrideAttack)
		{
			SwordBride* attackSword = playerBrides.at(oneAction.attackSwordID);
			Sprite* attackSwordSp = (Sprite*)brideSpriteVec.at(oneAction.attackSwordID);
			SwordBride* attackedSword = enemyBrides.at(oneAction.attackedSwordID);
			Sprite* attackedSwordSp = (Sprite*)enemySpriteVec.at(oneAction.attackedSwordID);

			Point originPosition = attackSwordSp->getPosition();
			ActionInterval* attackMove = MoveTo::create(1.0f, Point(attackedSwordSp->getPosition().x-50,attackedSwordSp->getPosition().y));

			LabelAtlas* valueLabel = (LabelAtlas*)attackedSwordSp->getChildByTag(1);
			
			char* mtime = new char[10];
			if (oneAction.attackValue != -1)
			{
				//没有miss,设置数值
				sprintf(mtime, "%d", oneAction.attackValue);
				valueLabel->setString(mtime);
			}
			else
			{
				//miss则显示miss
				valueLabel->setString("miss");
			}
			ActionInterval* labelFadeIn = FadeIn::create(0.1f);
			ActionInterval* labelFadeOut = FadeOut::create(0.1f);
			
			valueLabel->runAction(CCSequence::create(CCDelayTime::create((i + 1)*3.0+1),labelFadeIn, CCDelayTime::create(1.0f),labelFadeOut,NULL));   //显示数值
		
			
			ActionInterval* returnMove = MoveTo::create(1.0f, originPosition);


				attackSwordSp->runAction(CCSequence::create(CCDelayTime::create((i+1)*3.0), attackMove, CCDelayTime::create(1.0f), returnMove, NULL));           //执行一个组合，先移动，再攻击，再返回



				if (oneAction.isKillSword)
				{
					//被攻击者死亡
					ActionInterval* swordFade = FadeOut::create(0.2f);
					attackedSwordSp->runAction(CCSequence::create(CCDelayTime::create(6+i*3),swordFade,NULL));
					//如果死亡，直接淡出
				}
				else
				{
					//继续保留
				}

		}
		else  //敌人攻击
		{
			SwordBride* attackSword = enemyBrides.at(oneAction.attackSwordID);
			Sprite* attackSwordSp = enemySpriteVec.at(oneAction.attackSwordID);
			SwordBride* attackedSword = playerBrides.at(oneAction.attackedSwordID);
			Sprite* attackedSwordSp = brideSpriteVec.at(oneAction.attackedSwordID);

			Point originPosition = attackSwordSp->getPosition();
			ActionInterval* attackMove = MoveTo::create(1.0f, Point(attackedSwordSp->getPosition().x + 50, attackedSwordSp->getPosition().y));


			LabelAtlas* valueLabel = (LabelAtlas*)attackedSwordSp->getChildByTag(1);

			char* mtime = new char[10];
			if (oneAction.attackValue != -1)
			{
				//没有miss,设置数值
				sprintf(mtime, "%d", oneAction.attackValue);
				valueLabel->setString(mtime);
			}
			else
			{
				//miss则显示miss
				valueLabel->setString("miss");
			}
			ActionInterval* labelFadeIn = FadeIn::create(0.1f);
			ActionInterval* labelFadeOut = FadeOut::create(0.1f);

			valueLabel->runAction(CCSequence::create(CCDelayTime::create((i + 1)*3.0 + 1), labelFadeIn, CCDelayTime::create(1.0f), labelFadeOut, NULL));   //显示数值

			//存在显示问题



			ActionInterval* returnMove = MoveTo::create(1.0f, originPosition);

			attackSwordSp->runAction(CCSequence::create(CCDelayTime::create((i+1)*3.0), attackMove, CCDelayTime::create(1.0f), returnMove, NULL));
			if (oneAction.isKillSword)
			{
				//被攻击者死亡
				ActionInterval* swordFade = FadeOut::create(0.5f);
				attackedSwordSp->runAction(CCSequence::create(CCDelayTime::create(6 + i * 3), swordFade, NULL));
				//如果死亡，直接淡出
			}
			else
			{
				//继续保留
			}

		}

		//此处应延时2s后再执行

	}
	if (count == 1)
	{
		scheduleOnce(schedule_selector(GameBattleScene::showContinueDlg), attackActionVec.size() * 3 + 3);   //完成动画后调用函数显示对话框
	}
	else
	{
	}
}

void GameBattleScene::playAction(float delta)
{
	//单纯做延时
}


int GameBattleScene::getAttackValue(SwordBride* attackBride, SwordBride* attackedBride)
{
	double baseValue = attackBride->getBrideAtk() - attackedBride->getBrideDef();  //基础攻击为攻击力减敌方防御
	if (baseValue <= 0)
	{
		baseValue = 0;   //攻击力不能为负值
		return 0;
	}

	double hitRate = attackBride->getBrideDex()*1.0*(100 - attackedBride->getBrideAvd()) / 100.0;  //命中率
	double randomNum = CCRANDOM_0_1() * 100;
	if (randomNum <= hitRate)
	{
		//命中
		randomNum = CCRANDOM_0_1() * 40;
		baseValue = (20 - randomNum + 100) / 100.0*baseValue;     //攻击力上下浮动百分之20



		int crit = attackBride->getBrideFortune();
		 randomNum = CCRANDOM_0_1() * 100;
		if (randomNum <= crit)
		{
			baseValue = baseValue*1.50;    //如果产生了暴击
		}

		return baseValue;
	}
	else
	{
		return -1;   //未命中，发生了miss
	}
}

void GameBattleScene::showContinueDlg(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();
	LayerColor* layerDlg = LayerColor::create(Color4B(100,100,255,255));   //颜色待定
	layerDlg->setContentSize(Size(400,300));
	layerDlg->setAnchorPoint(ccp(0.5f,0.5f));
	layerDlg->setPosition(Point(winSize.width/2,winSize.height/2));

	Button* yesBtn = Button::create("GameBattleScene/yesBtn.png");
	Button* noBtn = Button::create("GameBattleScene/noBtn.png");
	yesBtn->addTouchEventListener(yesBtn, toucheventselector(GameBattleScene::onYesBtnClicked));
	noBtn->addTouchEventListener(noBtn,toucheventselector(GameBattleScene::onNoBtnClicked));
	yesBtn->setPosition(Point(50, 50));
	noBtn->setPosition(Point(150,50));

	layerDlg->addChild(yesBtn,1,1);
	layerDlg->addChild(noBtn,1,2);

	this->addChild(layerDlg, 1);
	
	

}
void GameBattleScene::onYesBtnClicked(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		//进入第二轮战斗，将对话框删除
		LayerColor* layerdlg = (LayerColor*)clickedButton->getParent();

		GameBattleScene* battleScene = (GameBattleScene*)layerdlg->getParent();
		layerdlg->removeFromParentAndCleanup(true);
		enterSecondBattle(battleScene->m_battleTeam1, battleScene->m_battleEnemies, battleScene->m_bridesSpriteVec, battleScene->m_enemiesSpriteVec);

		break;
	}
	default:
		break;
	}

}

void GameBattleScene::onNoBtnClicked(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		//不进入第二轮战斗，显示战斗总结后退出战斗界面？
		LayerColor* layerdlg = (LayerColor*)clickedButton->getParent();
		layerdlg->removeFromParentAndCleanup(true);
		ShowBattleResult();

		break;
	}
	default:
		break;
	}

}

void GameBattleScene::enterSecondBattle(std::vector<SwordBride*> playerBrides, std::vector<SwordBride*>enemyBrides, std::vector<Sprite*>bridesSpiteVec, std::vector<Sprite*>enemySpriteVec)
{
	std::vector<SwordBride*> allSwords;
	for (int i = 0; i < playerBrides.size(); i++)
	{
		allSwords.push_back(playerBrides.at(i));
	}
	for (int i = 0; i < enemyBrides.size(); i++)
	{
		allSwords.push_back(enemyBrides.at(i));
	}

	std::vector<SwordBride*>secondSequence = createBrideSequence(allSwords);
	doAttackAction(secondSequence, playerBrides, playerBrides, bridesSpiteVec,enemySpriteVec,2);   //在传参中加入sprite解决？
}


void GameBattleScene::ShowBattleResult()
{
	Director::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1.0f, MainScene2::createScene()));
	//直接退出到主界面中
}