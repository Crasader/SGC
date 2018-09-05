#ifndef __PLAYER_ITEM_H__
#define __PLAYER_ITEM_H__
#include "cocos2d.h"
USING_NS_CC;


struct ValueInfluence
{
	int InfluenceType;
	int InfluenceValue;
};
struct Property
{
	int PropertyType;
	int PropertyValue;
	int PropetyCost;
	std::vector<ValueInfluence>PropetyInfluence;

};



struct  ItemType
{
	std::string typeName;
	std::string typeCode;

};
class PlayerItem
{
public:
	PlayerItem();
	PlayerItem(int id);
	PlayerItem(int id,int num);
	int getItemNum();
	int getItemID();
	void setItemNum(int num);
	~PlayerItem();

	void setItemName(std::string name);
	void setItemTypes(std::vector<ItemType>types);
	void setItemQuality(int quality);
	void setItemRarity(int rarity);
	void setItemPotential(int potential);
	void setItemEffect(Property p);
	void setItemPropeties(std::vector<Property>propeties);
	void setMetalValue(int value);
	void setWoodValue(int value);
	void setWaterValue(int value);
	void setFireValue(int value);
	void setEarthValue(int value);


	std::string getItemName();
	std::vector<ItemType> getItemTypes();
	int getItemQuality();
	int getItemRarity();
	int getItemPotential();
	Property getItemEffect();
	std::vector<Property> getItemPropeties();



private:
	int m_ItemNum;	//道具数量  ，当前不需要？ 
	int m_ItemID;   //道具ID
	int m_metalValue;
	int m_woodValue;
	int m_waterValue;
	int m_fireValue;
	int m_earthValue;

	std::string m_name;
	std::vector<ItemType>m_types;   //道具类型：服装，武器，符文等

	int m_Quality;   //道具品质
	int m_Rarity;
	int m_Potential;
	Property m_Effect;   //效果，与物品本身有关

	std::vector<Property>m_property;   //道具包含的特性，特性有特性的类别和特性数据
	

};



#endif