#include "stdafx.h"
#include "VisibleGameObject.h"


VisibleGameObject::VisibleGameObject() :
isLoaded(false)
{
}


VisibleGameObject::~VisibleGameObject()
{
}

void VisibleGameObject::load(std::string filename)
{
	if (texture.loadFromFile(filename) == false)
	{
		this->filename = "";
		isLoaded = false;
	}
	else
	{
		this->filename = filename;
		sprite.setTexture(texture);
		isLoaded = true;
	}
}

void VisibleGameObject::draw(sf::RenderWindow& renderWindow)
{
	if (isLoaded)
	{
		renderWindow.draw(sprite);
	}
}

void VisibleGameObject::update(float elapsedTime)
{
}

void VisibleGameObject::setPosition(float x, float y)
{
	if (isLoaded)
	{
		sprite.setPosition(x, y);
	}
}

void VisibleGameObject::setPosition(sf::Vector2f pos)
{
	if (isLoaded)
	{
		sprite.setPosition(pos);
	}
}

sf::Vector2f VisibleGameObject::getPosition() const
{
	if (isLoaded)
	{
		return sprite.getPosition();
	}
	return sf::Vector2f();
}

sf::Sprite& VisibleGameObject::getSprite()
{
	return sprite;
}

bool VisibleGameObject::isObjectLoaded() const
{
	return isLoaded;
}

float VisibleGameObject::getHeight() const
{
	return sprite.getLocalBounds().height;
}

float VisibleGameObject::getWidth() const
{
	return sprite.getLocalBounds().width;
}

sf::Rect<float> VisibleGameObject::getBoundingRect() const
{
	return sprite.getLocalBounds();
}