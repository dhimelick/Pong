#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"

GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
	std::for_each(gameObjects.begin(), gameObjects.end(), GameObjectDeallocator());
}

void GameObjectManager::add(std::string name, VisibleGameObject* gameObject)
{
	gameObjects.insert(std::pair<std::string, VisibleGameObject*>(name, gameObject));
}

void GameObjectManager::remove(std::string name)
{
	std::map<std::string, VisibleGameObject*>::iterator results = gameObjects.find(name);
	if (results != gameObjects.end())
	{
		delete results->second;
		gameObjects.erase(results);
	}
}

VisibleGameObject* GameObjectManager::get(std::string name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = gameObjects.find(name);
	if (results == gameObjects.end())
	{
		return NULL;
	}

	return results->second;
}

int GameObjectManager::getObjectCount() const
{
	return gameObjects.size();
}

void GameObjectManager::drawAll(sf::RenderWindow& renderWindow)
{
	std::map<std::string, VisibleGameObject*>::const_iterator it = gameObjects.begin();
	while (it != gameObjects.end())
	{
		it->second->draw(renderWindow);
		it++;
	}
}

void GameObjectManager::updateAll()
{
	std::map<std::string, VisibleGameObject*>::const_iterator it = gameObjects.begin();
	float timeDelta = Game::getElapsed();

	while (it != gameObjects.end())
	{
		it->second->update(timeDelta);
		it++;
	}
}