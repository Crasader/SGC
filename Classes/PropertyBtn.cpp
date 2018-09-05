
#include "PropertyBtn.h"
USING_NS_CC;


PropertyBtn::PropertyBtn()
{


}

PropertyBtn* PropertyBtn::create(const std::string& normalImage, const std::string& selectedImage)
{
	PropertyBtn *posBox = new PropertyBtn();
	if (posBox&&posBox->init(normalImage, selectedImage))
	{
		posBox->loadTextures(normalImage, selectedImage);
		posBox->autorelease();
		posBox->m_selectStatus = false;
		return posBox;
	}
	CC_SAFE_DELETE(posBox);
	return nullptr;
}


void PropertyBtn::setCurrentPropety(Property p)
{
	m_property = p;

	std::string text;
	std::string Propety_name = Value(p.PropertyType).asString();
	std::string Propety_cost = Value(p.PropetyCost).asString();
	text.append(Propety_name);
	text.append("    ");
	text.append(Propety_cost);                    //说明特性的名称和潜力消耗

	Label* btnText = Label::create();
	btnText->setString(text);
	this->addChild(btnText);
}
Property PropertyBtn::getCurrentPropety()
{
	return m_property;
}
void PropertyBtn::setSelectedStatus(bool status)
{
	m_selectStatus = status;
}
bool PropertyBtn::getSelectedStatus()
{
	return m_selectStatus;
}