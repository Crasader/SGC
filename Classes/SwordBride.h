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
	int m_BrideID;  //½£¼§ID
	int m_level;  //½£¼§µÈ¼¶
	int m_hp;    //½£¼§ÑªÁ¿
	int m_mp;    //½£¼§Ä§Á¿ 
	int m_lp;	//½£¼§ºÃ¸Ð¶È
	int m_tp;	//½£¼§Æ£ÀÍ¶È
	int m_atk;	//½£¼§¹¥»÷Á¦
	int m_mag;	//½£¼§Ä§Á¦
	int m_def;	//½£¼§·ÀÓù
	int m_spd;	//½£¼§ËÙ¶È
	int m_dex;	//½£¼§ÃüÖÐ
	int m_avd;	//½£¼§ÉÁ±Ü
	int m_rng;	//½£¼§¹¥»÷·¶Î§
	int m_fortune; //½£¼§ÐÒÔË£¬Ó°Ïì±©»÷

	
};


#endif