#pragma once
#include <iostream>
#include <SFML/Graphics.hpp> 
#include <deque>
#include "Config.h"
#include "Food.h"

class SnakeApp
{
public:

	SnakeApp(sf::RenderWindow& window, Food &food);	// Costruttore: riceve in ingresso un riferimento a Food già esistente
	void DrawGrid();	// disegna una griglia a schermo evidenziando le celle
	void CheckWindowLimits();
	void ProcessInput();
	void Update();
	void MyDrawSnake();
	void SnakeMove();
	void SnakeEat();
	bool CollisionBodyDetection();
	void GrowBy(int count);
	void GrowBy();
	void AddBodyShape();
	void DrawFinalPage();

	sf::RenderWindow &windRef;
	Food &foodRef;
	sf::Clock deltaClock;
	sf::Vector2f bodySize;
	sf::Vector2f lastPosition;	// ultima posizione del serpente per il movimento in update
	sf::Vector2i startPosition;
	sf::Vector2i direction;	// direzione del serpente, inizialmente verso destra
	sf::RectangleShape body;
	std::deque<sf::RectangleShape> snakeBodyShapes;
	std::deque<sf::Vector2i> snakeBodyPos;


	float timeToCheckUpdate;	//tempo di refresh per controlli in update
	bool isRunning;
	bool GameOver = false;
	bool isMoving = false;	// per controllare se il serpente si sta muovendo o meno
	bool shouldGrow = false;  // flag che indica “cresci al prossimo move”



	/*	NOTE 
		- 
		
	*/
};

