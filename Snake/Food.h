#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

class Food
{
public:
	Food();
	void MyDrawFood(sf::RenderWindow& window);
	void MyRedrawFood();
	int getRandomInt(int min, int max);
	
	sf::CircleShape foodBody;
	std::deque<sf::Vector2i> foodPosList;
	sf::Vector2i foodStarPos;

	bool isEaten;
};

