#include "SwordBride.h"
USING_NS_CC;
SwordBride::SwordBride()
{
	 m_BrideID=0; 
	 m_level=0; 
	 m_hp=0;    
	 m_mp=0;    
	 m_lp=0;	
	 m_tp=0;	
	 m_atk=0;	
	 m_mag=0;	
	 m_def=0;	
	 m_spd=0;	
	 m_dex=0;	
	 m_avd = 0;
	 m_rng = 0;
	 m_fortune = 0;

	 isEnemy = false;
}
SwordBride::~SwordBride()
{

}
void SwordBride::setBrideID(int ID)
{
	m_BrideID = ID;
}
int SwordBride::getBrideID()
{
	return m_BrideID;
}
int SwordBride::getBrideLv()
{
	return m_level;
}
int SwordBride::getBrideHp()
{
	return m_hp;
}
int SwordBride::getBrideMp()
{
	return m_mp;
}
int SwordBride::getBrideLp()
{
	return m_lp;
}
int SwordBride::getBrideTp()
{
	return m_tp;
}
int SwordBride::getBrideAtk()
{
	return m_atk;
}
int SwordBride::getBrideMag()
{
	return m_mag;
}
int SwordBride::getBrideDef()
{
	return m_def;
}
int SwordBride::getBrideSpd()
{
	return m_spd;
}
int SwordBride::getBrideDex()
{
	return m_dex;
}
int SwordBride::getBrideAvd()
{
	return m_avd;
}
int SwordBride::getBrideRng()
{
	return m_rng;
}
int SwordBride::getBrideFortune()
{
	return m_fortune;
}

void SwordBride::setBrideAtk(int attack)
{
	m_atk = attack;
}
void SwordBride::setBrideDef(int defence)
{
	m_def = defence;
}
void SwordBride::setBrideHP(int hp)
{
	m_hp = hp;
}
void SwordBride::setBrideMP(int mp)
{
	m_mp = mp;
}

void SwordBride::setBrideLv(int lv)
{
	m_level = lv;
}
void SwordBride::setBrideSpd(int spd)
{
	m_spd = spd;
}

void SwordBride::setBrideFortune(int p)
{
	m_fortune = p;
}
void SwordBride::setBrideDex(int dex)
{
	m_dex = dex;
}
void SwordBride::setBrideAvd(int avd)
{
	m_avd = avd;
}