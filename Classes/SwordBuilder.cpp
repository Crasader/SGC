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
	//���ý��콣������
	m_swordName = name;
}
void SwordBuilder::setThres(int thres[4])
{
	//���ý��콣��4����Դ��ֵ
	for (int i = 0; i < 4;i++)
		m_thresholds[i] = thres[i];
}
void SwordBuilder::setWeight(double weight[4])
{
	//���ý��콣��4����Դ��Ȩ��
	for (int i = 0; i < 4; i++)
		m_weight[i] = weight[i];
}
void SwordBuilder::setRarity(double rarity)
{
	//���ý���ϡ�жȣ��ɹ��ʣ���
	m_rarity = rarity;
}
void SwordBuilder::setItems(std::map<std::string, double> items)
{
	//���öԽ�����Ӱ�����Ʒ����
	m_items = items;
}
void SwordBuilder::setBluePrints(std::map<std::string, float> m_blueprints)
{
}
void SwordBuilder::setMaxRate(double maxRate)
{
	//���ý���ϡ�ж����ֵ
	m_maxRate = maxRate;
}
void SwordBuilder::setSucceed(double succeed)
{
	//���ý���ɹ���
	m_succeed = succeed;
}
void SwordBuilder::setCurrentResource(int res[4])
{
	//���õ�ǰͶ�����Դ����
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