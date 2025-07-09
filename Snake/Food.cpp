#include "Food.h"
#include "Config.h"
#include <random>



Food::Food()
{
	isEaten = false;

	foodBody.setRadius(Config::CELL_SIZE * 0.5f); // Imposta il raggio del cibo
	foodBody.setFillColor(sf::Color::Red);
	foodStarPos = sf::Vector2i(getRandomInt(0, Config::GRID_SIZE), getRandomInt(0, Config::GRID_SIZE));
	foodPosList.push_front(foodStarPos);
}

void Food::MyDrawFood(sf::RenderWindow& window)
{
	float pixelX = foodPosList[0].x * Config::CELL_SIZE;	// conversione in pixel
	float pixelY = foodPosList[0].y * Config::CELL_SIZE;
	foodBody.setPosition({pixelX, pixelY});

	window.draw(this->foodBody);
}

void Food::MyRedrawFood()
{
	float pixelX = foodPosList[0].x * Config::CELL_SIZE -1;	// conversione in pixel
	float pixelY = foodPosList[0].y * Config::CELL_SIZE - 1;
	foodBody.setPosition({ pixelX, pixelY });
}


int Food::getRandomInt(int min, int max)
{
	// Il motore è statico per mantenere lo stato fra le chiamate
	static std::mt19937 engine{ std::random_device{}() };

	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine);
	return 1;
}

