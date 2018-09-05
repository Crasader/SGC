#ifndef __SWORD_BRIDE_H__
#define __SWORD_BRIDE_H__
#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;
class SwordBride:public Entity
{
public:
	SwordBride();

	void setBrideID(int ID);
	int getBrideID();
	int getBrideLv();
	int getBrideHp();
	int getBrideMp();
	int getBrideLp();
	int getBrideTp();
	int getBrideAtk();
	int getBrideMag();
	int getBrideDef();
	int getBrideSpd();
	int getBrideDex();
	int getBrideAvd();
	int getBrideRng();
	int getBrideFortune();
	void setBrideLv(int lv);
	void setBrideHP(int hp);
	void setBrideMP(int mp);
	void setBrideAtk(int attack);
	void setBrideDef(int defence);
	void setBrideSpd(int spd);
	void setBrideFortune(int fortune);
	void setBrideDex(int dex);
	void setBrideAvd(int avd);
	~SwordBride();

	bool isEnemy;

private:
	Sprite* m_CardView;  
	Sprite* m_BigView;
	int m_BrideID;  //����ID
	int m_level;  //�����ȼ�
	int m_hp;    //����Ѫ��
	int m_mp;    //����ħ�� 
	int m_lp;	//�����øж�
	int m_tp;	//����ƣ�Ͷ�
	int m_atk;	//����������
	int m_mag;	//����ħ��
	int m_def;	//��������
	int m_spd;	//�����ٶ�
	int m_dex;	//��������
	int m_avd;	//��������
	int m_rng;	//����������Χ
	int m_fortune; //�������ˣ�Ӱ�챩��

	
};


#endif