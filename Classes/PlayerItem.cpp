//道具具体数据类，包含了道具类型，数量和属性，通过道具ID可以获取各个道具的数据

#include "PlayerItem.h"

PlayerItem::PlayerItem()
{
}
PlayerItem::PlayerItem(int id)
{
	m_ItemID = id;
}
PlayerItem::PlayerItem(int id,int num)
{
	m_ItemID = id;
	m_ItemNum = num;


	m_Quality = 0;
	 m_Rarity=0;
	 m_Potential=0;
	 m_property.clear();
	if (m_ItemID == 1)
	{
		//m_types.push_back(ITEM_TYPE_WEAPON);

		Property p1;
		p1.PropertyType = 1;
		p1.PropertyValue = 3;
		m_property.push_back(p1);

		Property p2;
		p2.PropertyType = 2;
		p2.PropertyValue = -1;
		m_property.push_back(p2);
	}
	if (m_ItemID == 2)
	{
	//	m_types.push_back(ITEM_TYPE_ORNAMENT);
		Property p1;
		p1.PropertyType = 3;
		p1.PropertyValue = 2;
		m_property.push_back(p1);
	}
	if (m_ItemID == 3)
	{
	//	m_types.push_back(ITEM_TYPE_ASSIST);
	//	m_types.push_back(ITEM_TYPE_METAL);
		Property p1;
		p1.PropertyType = 4;
		p1.PropertyValue = 1;
		m_property.push_back(p1);
	}

}
PlayerItem::~PlayerItem()
{
}
void PlayerItem::setItemNum(int num)
{
	m_ItemNum = num;
}
int PlayerItem::getItemNum()
{
	return m_ItemNum;
}
int PlayerItem::getItemID()
{
	return m_ItemID;
}


void PlayerItem::setItemName(std::string name)
{
	m_name = name;
}
void PlayerItem::setItemQuality(int quality)
{
	m_Quality = quality;
}
void PlayerItem::setItemRarity(int rarity)
{
	m_Rarity = rarity;
}
void PlayerItem::setItemPotential(int potential)
{
	m_Potential = potential;
}
void PlayerItem::setItemTypes(std::vector<ItemType>types)
{
	m_types = types;
}
void PlayerItem::setItemPropeties(std::vector<Property>propeties)
{
	m_property = propeties;
}
void PlayerItem::setItemEffect(Property p)
{
	m_Effect = p;
}
std::string PlayerItem::getItemName()
{
	return m_name;
}
std::vector<ItemType> PlayerItem::getItemTypes()
{
	return m_types;
}
int PlayerItem::getItemQuality()
{
	return m_Quality;
}
int PlayerItem::getItemRarity()
{
	return m_Rarity;
}
int PlayerItem::getItemPotential()
{
	return m_Potential;
}
Property PlayerItem::getItemEffect()
{
	return m_Effect;
}
std::vector<Property> PlayerItem::getItemPropeties()
{
	return m_property;
}

void PlayerItem::setMetalValue(int value)
{
	m_metalValue = value;
}
void PlayerItem::setWoodValue(int value)
{
	m_woodValue = value;
}

void PlayerItem::setWaterValue(int value)
{
	m_waterValue = value;
}

void PlayerItem::setFireValue(int value)
{
	m_fireValue = value;
}

void PlayerItem::setEarthValue(int value)
{
	m_earthValue = value;
}



