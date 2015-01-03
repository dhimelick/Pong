#pragma once

class VisibleGameObject
{
public:
	VisibleGameObject();
	virtual ~VisibleGameObject();

	virtual void load(std::string filename);
	virtual void draw(sf::RenderWindow& window);
	virtual void update(float elapsedTime);

	virtual void setPosition(float x, float y);
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition() const;
	virtual bool isObjectLoaded() const;

	virtual float getWidth() const;
	virtual float getHeight() const;

	virtual sf::Rect<float> getBoundingRect() const;

protected:
	sf::Sprite& getSprite();

private:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string filename;
	bool isLoaded;
};

