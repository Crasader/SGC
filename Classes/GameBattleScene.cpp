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

	m_battleEnemies = battlepoint->m_enemySwords;    //���õ����б�
	m_battleBrides = GameData::shareGameData()->getCurrentSwordTeam();   
	//���ý����б������ݴ洢���л�ȡ����ǰ�ɻ�ȡ4�ӣ�ֻ��1�ӽ���ս��

	m_battleWeather = battlepoint->m_weather;  //��������

	m_battleTeam1.clear();

	for (int i = 0; i < 6; i++)
	{
		SwordBride* oneBride = m_battleBrides.at(i);

		oneBride->setBrideDef(200+i);
		oneBride->setBrideAvd(i);
		oneBride->setBrideDex(90 + i);
		oneBride->setBrideFortune(20+i);    //�������ݣ�Ӧ��ϵͳ�д洢�ı���б��л�ȡ


		m_battleTeam1.push_back(oneBride);
		m_allSwords.push_back(oneBride);
	}
	for (int i = 0; i < m_battleEnemies.size(); i++)
	{
		SwordBride* oneEnemy = m_battleEnemies.at(i);
		oneEnemy->setBrideDef(200 + i);
		oneEnemy->setBrideAvd(i);
		oneEnemy->setBrideDex(90 + i);
		oneEnemy->setBrideFortune(20 + i);   //�������ݣ�Ӧ��json�ļ��л�ȡ

		m_allSwords.push_back(oneEnemy);
	}
	//m_allSwordsΪ��ҽ������ϵ��˵��б����ڽ�������

	m_atkSequence = createBrideSequence(m_allSwords);

	//�������棬���Ϊ��������ʾ���ұ�Ϊ���˵���ʾ

	 m_bridesSpriteVec.clear();
	m_enemiesSpriteVec.clear();

	for (int i = 0; i < m_battleTeam1.size(); i++)
	{
		Sprite* brideSp = Sprite::create("GameBattleScene/BattleKuang.png");  //���ݽ�����ѡ��ͬ��ͼ��ͼ����زĴ��� 
																				//����Ĵ���
		brideSp->setPosition(Point(100,700-i*50));
		this->addChild(brideSp,3,10+i);   //������layer�е�ID��10��ʼ

		LabelAtlas* valueLabel = LabelAtlas::create("0123456789", "GameBattle\number.png", 15, 25, '0');  //��ʾ�˺���ֵ�Ŀؼ�
		valueLabel->setPosition(Point(brideSp->getPosition().x + 20, brideSp->getPosition().y));   //�ŵ��ҷ����ұߣ���������
		valueLabel->setString("0");   //Ĭ��ֵΪ0��
		valueLabel->setVisible(false);       //Ĭ�ϲ���ʾ
		brideSp->addChild(valueLabel,1);


		m_bridesSpriteVec.push_back(brideSp);
	}
	for (int i = 0; i < m_battleEnemies.size(); i++)
	{
		Sprite* enemySp = Sprite::create("GameBattleScene/BattleKuang.png");    //���ݵ��˿�ѡ��ͬ��ͼ��ͼ���زĴ���
		enemySp->setPosition(Point(600,700-i*50));
		this->addChild(enemySp,3,20+i);   //������layer�е�ID��20��ʼ


		LabelAtlas* valueLabel = LabelAtlas::create("0123456789", "GameBattle\number.png", 15, 25, '0');  //��ʾ�˺���ֵ�Ŀؼ�
		valueLabel->setPosition(Point(enemySp->getPosition().x - 20, enemySp->getPosition().y));   //�ŵ��з����ұߣ���������
		valueLabel->setString("0");   //Ĭ��ֵΪ0��
		valueLabel->setVisible(false);       //Ĭ�ϲ���ʾ
		enemySp->addChild(valueLabel, 1);

		m_enemiesSpriteVec.push_back(enemySp);
	}
	



	doAttackAction(m_atkSequence,m_battleTeam1,m_battleEnemies,m_bridesSpriteVec,m_enemiesSpriteVec,1);




	

	return true;
}


std::vector<SwordBride*> GameBattleScene::createBrideSequence(std::vector<SwordBride*>allBrides)   //���յ��ҳ�Ա���ٶȽ�������
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
		}  //�����ҵ��ٶ�����ֵ
		count = 0;
		for (std::vector<SwordBride*>::iterator it = allBrides.begin(); it != allBrides.end();)
		{
			//Ѱ�����ĸ��������ٶ�������η��빥��������
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
	}//������������

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
			continue;   //��������ߵ�Ѫ��С��0��˵���Ѿ������������й���


		AttackAction oneAction;
 
		if (oneBride->isEnemy)
		{        //���������Ϊ���ˣ����ڵ������ҵ���
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
		{   //������Ϊ���
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
			//���ȫ������,ֹͣ����
			break;
		}
		else if (enemySwordCount == 0)
		{
			//����ȫ��������Ҳֹͣ���� 
			break;
		}
		else
		{
			if (oneBride->isEnemy == false)  //��ҽ������й���
			{
				struct timeval now;
				int randomNum;
				gettimeofday(&now, NULL);   //��ȡϵͳ��ǰʱ��

				unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
				srand(rand_seed);
				randomNum = rand() % enemyBrides.size();
				while (enemyBrides.at(randomNum)->getBrideHp() <= 0)  //�ҵ�һ��Ѫ������0�ĵ��˽��й���
				{
					gettimeofday(&now, NULL);
					rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);
					srand(rand_seed);
					randomNum = rand() % enemyBrides.size();
				}
				SwordBride* attackedBride = enemyBrides.at(randomNum);   
				int  attackValue = getAttackValue(oneBride, attackedBride);  //��ȡ������ɵ��˺���ֵ

				oneAction.attackedSwordID = randomNum;
				oneAction.attackValue = attackValue;   //��¼��������ֵ����������ͱ���������
				oneAction.isBrideAttack = true;


				if (attackValue == -1)
				{
					attackValue = 0;
					//missʱ��0����
				}

				if (attackedBride->getBrideHp() - attackValue <= 0)
				{
					enemySwordCount--;
					attackedBride->setBrideHP(0);    //���Ѫ��Ϊ0���򱻴�����������������
					oneAction.isKillSword = true;
				}
				else
				{
					attackedBride->setBrideHP(attackedBride->getBrideHp() - attackValue);   //����Ѫ������
					oneAction.isKillSword = false;
				}

		
				

				//д���˴������ƹ����������


			}
			else  //���˽��н���
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
				oneAction.attackValue = attackValue;  //������й������ǵ��ˣ��򹥻�����
				oneAction.isBrideAttack = false;

				if (attackValue == -1)
				{
					attackValue = 0;
					//missʱ����0����
				}


				
				if (attackedBride->getBrideHp() - attackValue <= 0)
				{
					playerSwordCount--;
					attackedBride->setBrideHP(0);   //���Ѫ��Ϊ0���򱻴�����������������
					oneAction.isKillSword = true;
				}
				else
				{
					attackedBride->setBrideHP(attackedBride->getBrideHp() - attackValue);
					oneAction.isKillSword = false;
				}
						
			}

			attackSequence.push_back(oneAction);   //�������Ĺ������뵽������

		}


	}
	
	return attackSequence;
}


void GameBattleScene::doAttackAction(std::vector<SwordBride*> bridesSequence, std::vector<SwordBride*>playerBrides, std::vector<SwordBride*>enemyBrides,
	std::vector<Sprite*>brideSpriteVec,std::vector<Sprite*>enemySpriteVec,int count)
{
	std::vector<AttackAction>attackActionVec= createAttackSequence(bridesSequence, playerBrides, enemyBrides); //��ȡ��������
	

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
				//û��miss,������ֵ
				sprintf(mtime, "%d", oneAction.attackValue);
				valueLabel->setString(mtime);
			}
			else
			{
				//miss����ʾmiss
				valueLabel->setString("miss");
			}
			ActionInterval* labelFadeIn = FadeIn::create(0.1f);
			ActionInterval* labelFadeOut = FadeOut::create(0.1f);
			
			valueLabel->runAction(CCSequence::create(CCDelayTime::create((i + 1)*3.0+1),labelFadeIn, CCDelayTime::create(1.0f),labelFadeOut,NULL));   //��ʾ��ֵ
		
			
			ActionInterval* returnMove = MoveTo::create(1.0f, originPosition);


				attackSwordSp->runAction(CCSequence::create(CCDelayTime::create((i+1)*3.0), attackMove, CCDelayTime::create(1.0f), returnMove, NULL));           //ִ��һ����ϣ����ƶ����ٹ������ٷ���



				if (oneAction.isKillSword)
				{
					//������������
					ActionInterval* swordFade = FadeOut::create(0.2f);
					attackedSwordSp->runAction(CCSequence::create(CCDelayTime::create(6+i*3),swordFade,NULL));
					//���������ֱ�ӵ���
				}
				else
				{
					//��������
				}

		}
		else  //���˹���
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
				//û��miss,������ֵ
				sprintf(mtime, "%d", oneAction.attackValue);
				valueLabel->setString(mtime);
			}
			else
			{
				//miss����ʾmiss
				valueLabel->setString("miss");
			}
			ActionInterval* labelFadeIn = FadeIn::create(0.1f);
			ActionInterval* labelFadeOut = FadeOut::create(0.1f);

			valueLabel->runAction(CCSequence::create(CCDelayTime::create((i + 1)*3.0 + 1), labelFadeIn, CCDelayTime::create(1.0f), labelFadeOut, NULL));   //��ʾ��ֵ

			//������ʾ����



			ActionInterval* returnMove = MoveTo::create(1.0f, originPosition);

			attackSwordSp->runAction(CCSequence::create(CCDelayTime::create((i+1)*3.0), attackMove, CCDelayTime::create(1.0f), returnMove, NULL));
			if (oneAction.isKillSword)
			{
				//������������
				ActionInterval* swordFade = FadeOut::create(0.5f);
				attackedSwordSp->runAction(CCSequence::create(CCDelayTime::create(6 + i * 3), swordFade, NULL));
				//���������ֱ�ӵ���
			}
			else
			{
				//��������
			}

		}

		//�˴�Ӧ��ʱ2s����ִ��

	}
	if (count == 1)
	{
		scheduleOnce(schedule_selector(GameBattleScene::showContinueDlg), attackActionVec.size() * 3 + 3);   //��ɶ�������ú�����ʾ�Ի���
	}
	else
	{
	}
}

void GameBattleScene::playAction(float delta)
{
	//��������ʱ
}


int GameBattleScene::getAttackValue(SwordBride* attackBride, SwordBride* attackedBride)
{
	double baseValue = attackBride->getBrideAtk() - attackedBride->getBrideDef();  //��������Ϊ���������з�����
	if (baseValue <= 0)
	{
		baseValue = 0;   //����������Ϊ��ֵ
		return 0;
	}

	double hitRate = attackBride->getBrideDex()*1.0*(100 - attackedBride->getBrideAvd()) / 100.0;  //������
	double randomNum = CCRANDOM_0_1() * 100;
	if (randomNum <= hitRate)
	{
		//����
		randomNum = CCRANDOM_0_1() * 40;
		baseValue = (20 - randomNum + 100) / 100.0*baseValue;     //���������¸����ٷ�֮20



		int crit = attackBride->getBrideFortune();
		 randomNum = CCRANDOM_0_1() * 100;
		if (randomNum <= crit)
		{
			baseValue = baseValue*1.50;    //��������˱���
		}

		return baseValue;
	}
	else
	{
		return -1;   //δ���У�������miss
	}
}

void GameBattleScene::showContinueDlg(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();
	LayerColor* layerDlg = LayerColor::create(Color4B(100,100,255,255));   //��ɫ����
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
		//����ڶ���ս�������Ի���ɾ��
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
		//������ڶ���ս������ʾս���ܽ���˳�ս�����棿
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
	doAttackAction(secondSequence, playerBrides, playerBrides, bridesSpiteVec,enemySpriteVec,2);   //�ڴ����м���sprite�����
}


void GameBattleScene::ShowBattleResult()
{
	Director::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1.0f, MainScene2::createScene()));
	//ֱ���˳�����������
}