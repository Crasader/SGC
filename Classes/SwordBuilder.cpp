#include "SwordBuilder.h"
SwordBuilder::SwordBuilder()
{
	m_swordName = "";
	for (int i = 0; i < 4; i++)
	{
		m_thresholds[i] = 0;
		m_weight[i] = 0;
		m_currentRes[i] = 0;
	}
	m_rarity = 0;
	m_maxRate = 0;
	m_succeed = 0;
}
SwordBuilder::~SwordBuilder()
{
}
void SwordBuilder::setName(std::string name)
{
	//设置建造剑姬名称
	m_swordName = name;
}
void SwordBuilder::setThres(int thres[4])
{
	//设置建造剑姬4个资源阈值
	for (int i = 0; i < 4;i++)
		m_thresholds[i] = thres[i];
}
void SwordBuilder::setWeight(double weight[4])
{
	//设置建造剑姬4个资源的权重
	for (int i = 0; i < 4; i++)
		m_weight[i] = weight[i];
}
void SwordBuilder::setRarity(double rarity)
{
	//设置建造稀有度（成功率？）
	m_rarity = rarity;
}
void SwordBuilder::setItems(std::map<std::string, double> items)
{
	//设置对剑姬有影响的物品集合
	m_items = items;
}
void SwordBuilder::setBluePrints(std::map<std::string, float> m_blueprints)
{
}
void SwordBuilder::setMaxRate(double maxRate)
{
	//设置建造稀有度最大值
	m_maxRate = maxRate;
}
void SwordBuilder::setSucceed(double succeed)
{
	//设置建造成功率
	m_succeed = succeed;
}
void SwordBuilder::setCurrentResource(int res[4])
{
	//设置当前投入的资源数量
	for (int i = 0; i < 4; i++)
		m_currentRes[i] = res[i];

}

std::string SwordBuilder::getName()
{
	return m_swordName;
}


double SwordBuilder::calcRate()
{
	int resDiff[4];
	double sum = 0;
	bool buildFlag = true;
	for (int i = 0; i < 4; i++)
	{
		resDiff[i] = m_currentRes[i] - m_thresholds[i];
		if (resDiff[i] < 0)
		{
			resDiff[i] = 0;
			buildFlag = false;
		}
		sum = sum + resDiff[i] * m_weight[i];
	}
	if (buildFlag==false)
		return 0;
	else
	{
		sum = sum*m_succeed;
		if (sum > m_maxRate)
			sum = m_maxRate;
		return sum;
	}
}