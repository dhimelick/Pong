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
	virtual sf::Vector2f getPosition() const;
	virtual bool isObjectLoaded() const;

protected:
	sf::Sprite& getSprite();

private:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string filename;
	bool isLoaded;
};

