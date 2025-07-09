#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Config.h"

class TestClass
{
public:

	sf::Vector2f sPosition;
	sf::Vector2f lastPosition;	// ultima posizione del serpente per il movimento in update
	sf::Vector2f direction;	// direzione del serpente, inizialmente verso destra

	sf::RectangleShape body;
	std::deque<sf::Vector2i> testBodyPos;
	std::deque<sf::RectangleShape> testBodyShapes;

	sf::Clock deltaClock;
	float timeToCheckUpdate = 0.6f;
	bool isMoving = false;	// per controllare se il serpente si sta muovendo o meno

	bool shouldGrow = false;  // flag che indica “cresci al prossimo move”
	sf::Vector2i gridDirection = { 1, 0 }; // direzione di movimento in termini di celle, es. (1,0)


	TestClass();
	void ProcessInput(sf::RenderWindow& window);
	void Update();
	void MyDrawSnake(sf::RenderWindow& Window, const sf::Drawable& Drawable);
	void TestMove();
	void TestEat(sf::RenderWindow& Window);
	void AddBodyShape();
	void GrowBy(int count);


};

