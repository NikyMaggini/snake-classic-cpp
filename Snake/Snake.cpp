// Snake.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.

#include <iostream>
#include "SnakeApp.h"
#include "Config.h"
#include "Food.h"
#include <SFML/Graphics.hpp>
#include "TestClass.h"

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode({ Config::WINDOW_SIZE, Config::WINDOW_SIZE }), "Snake Nicola!");

	Food Food;
	SnakeApp SnakeApp(window, Food);
	TestClass TestClass;

	//TestClass.GrowBy(0);
	//SnakeApp.GrowBy(2);

	while (window.isOpen() && !SnakeApp.GameOver)
	{

		//SnakeApp.CheckWindowLimits();	//controlla se il serpente esce dai limiti della finestra
		//TestClass.ProcessInput(window);
		//TestClass.Update();				//aggiorna il tempo per il movimento del serpente
		SnakeApp.Update();
		SnakeApp.ProcessInput();			//metodo che controlla gli eventi della finestra
		window.clear();
		//SnakeApp.DrawGrid();
		Food.MyDrawFood(window);
		//TestClass.MyDrawSnake(window, TestClass.body);
		SnakeApp.MyDrawSnake();
		window.display();
	}
}

/* README
*	- 
* 
*/

