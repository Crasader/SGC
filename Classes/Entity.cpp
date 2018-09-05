#include "Entity.h"
Entity::Entity()
{
	m_sprite = NULL;
//this is a test
}
Entity::~Entity()
{
}
Sprite* Entity::getsprite()
{
	return this->m_sprite;
}
void Entity::bindSprite(Sprite* sprite)
{
	this->m_sprite = sprite;
	this->addChild(m_sprite);
	Size size = m_sprite->getContentSize();
	m_sprite->setPosition(Point(size.width*0.5f, size.height*0.5f));

	this->setContentSize(size);
}
