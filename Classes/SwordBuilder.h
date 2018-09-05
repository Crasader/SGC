#ifndef __SWORD_BUILDER_H__
#define __SWORD_BUILDER_H__
#include "cocos2d.h"
USING_NS_CC;
class SwordBuilder
{
public:
	SwordBuilder();
	~SwordBuilder();
	void setName(std::string name);
	void setThres(int thres[4]);
	void setWeight(double weight[4]);
	void setRarity(double rarity);
	void setItems(std::map<std::string, double> m_items);
	void setBluePrints(std::map<std::string, float> m_blueprints);
	void setMaxRate(double maxRate);
	void setSucceed(double m_succeed);
	void setCurrentResource(int res[4]);

	std::string getName();

	double calcRate();

private:
	std::string m_swordName;
	int m_thresholds[4];
	double m_weight[4];
	double m_rarity;
	std::map<std::string, double> m_items;
	std::map<std::string, double> m_blueprints;
	double m_maxRate;
	double m_succeed;
	int m_currentRes[4];
	
};


#endif