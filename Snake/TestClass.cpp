#include "TestClass.h"
#include "Config.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

TestClass::TestClass()
{

	 // 1) Calcolo la cella centrale (in griglia) e la inserisco in testBodyPos
	int center = Config::GRID_SIZE / 2;
	testBodyPos.push_back({ center, center });

	// 2) Creo il primo rettangolo di dimensione CELL_SIZE×CELL_SIZE, colore verde
	sf::RectangleShape rect;
	rect.setSize({ static_cast<float>(Config::CELL_SIZE), static_cast<float>(Config::CELL_SIZE) });
	rect.setFillColor(sf::Color::Green);

	// 3) Posiziono il rettangolo in pixel: (center * CELL_SIZE, center * CELL_SIZE)
	rect.setPosition(
		{ static_cast<float>(center * Config::CELL_SIZE),
		static_cast<float>(center * Config::CELL_SIZE) });

	testBodyShapes.push_back(rect);
}

void TestClass::Update()
{
	float totalTime = deltaClock.getElapsedTime().asSeconds(); // 3) Ottengo il tempo totale trascorso dal lancio del clock (opzionale)

	//std::cout << "testBodyPos : " << testBodyPos.size() << std::endl;

	if (totalTime >= timeToCheckUpdate) {

		deltaClock.restart();	//faccio ripartire da 0 il timer 

		if (!isMoving)
		{
			TestMove();
		}
	}
}

void TestClass::ProcessInput(sf::RenderWindow& window)
{
	//Usiamo while (auto maybeEvent = window.pollEvent()) perch� in SFML 3.x pollEvent() restituisce un std::optional<sf::Event>.
	while (auto maybeEvent = window.pollEvent()) {

		const sf::Event& event = *maybeEvent;

		if (event.is<sf::Event::Closed>())
		{
			window.close();
			//isRunning = false;
		}
		else if (event.is<sf::Event::KeyPressed>())
		{
			auto keyEvent = event.getIf<sf::Event::KeyPressed>();
			using Key = sf::Keyboard::Key;

			switch (keyEvent->code)
			{
			case Key::Up:
				std::cout << "KEY UP" << std::endl;
				isMoving = true;
				gridDirection = { 0, -1 }; // Direzione in termini di celle

				break;
			case Key::Down:
				std::cout << "KEY Down" << std::endl;
				isMoving = true;
				gridDirection = { 0, 1 };

				break;
			case Key::Left:
				std::cout << "KEY Left" << std::endl;
				isMoving = true;
				gridDirection = { -1, 0 };

				break;
			case Key::Right:
				std::cout << "KEY Right" << std::endl;
				isMoving = true;
				gridDirection = { 1, 0 };

				break;

			case Key::Escape:
				window.close();
				//isRunning = false;
				isMoving = false;
				break;
			default:
				break;
			}

		}
		isMoving = false;

	}
}

void TestClass::MyDrawSnake(sf::RenderWindow& Window, const sf::Drawable& Drawable)
{
	for (const auto& shape : testBodyShapes)
	{
		Window.draw(shape);

	}
}

void TestClass::TestMove()
{
	sf::Vector2i oldHead = testBodyPos.front();	// 1) Estrai la posizione griglia attuale della testa
	sf::Vector2i newHead = { oldHead.x + gridDirection.x, oldHead.y + gridDirection.y };	// 2) Calcola la nuova testa in griglia

	testBodyPos.push_front(newHead);	// 3) Inserisci newHead in testa a testBodyPos

	// forse si puo saltare
	if (shouldGrow == false)
	{
		testBodyPos.pop_back();
	}
	else
	{
		shouldGrow = false;
	}

	// forse si puo saltare
	// 5) Se ho una cella in più in testBodyPos, aggiungo un nuovo RectangleShape in coda
	if (testBodyShapes.size() < testBodyPos.size())
	{
		AddBodyShape();
	}

	for (size_t i = 0; i < testBodyPos.size(); i++)
	{
		sf::Vector2i cell = testBodyPos[i];

		float pixelX = cell.x * Config::CELL_SIZE;	// converto in pixel
		float pixelY = cell.y * Config::CELL_SIZE;	// converto in pixel
		testBodyShapes[i].setPosition({ pixelX, pixelY });
	}
}

void TestClass::TestEat(sf::RenderWindow& Window)
{

	shouldGrow = true;  // Imposto il flag per crescere al prossimo movimento

}

void TestClass::AddBodyShape()
{
	// Creo e memorizzo un nuovo rettangolo verde di dimensione CELL_SIZE×CELL_SIZE
	sf::RectangleShape newShape;
	newShape.setSize({ static_cast<float>(Config::CELL_SIZE), static_cast<float>(Config::CELL_SIZE) });
	newShape.setFillColor(sf::Color::Green);

	// Lo posizionerò insieme agli altri nel prossimo ciclo di TestMove
	testBodyShapes.push_back(newShape);
}

void TestClass::GrowBy(int count)
{
	for (int i = 0; i < count; ++i)
	{
		// 1) Prendo la posizione griglia dell’ultimo segmento (coda)
		sf::Vector2i tailCell = testBodyPos.back();

		// 2) Aggiungo la nuova cella in fondo a testBodyPos
		testBodyPos.push_back(tailCell);

		// 3) Creo un nuovo RectangleShape per il segmento
		sf::RectangleShape newShape;
		newShape.setSize({ static_cast<float>(Config::CELL_SIZE),
							static_cast<float>(Config::CELL_SIZE) });
		newShape.setFillColor(sf::Color::Green);

		// 4) Lo posiziono esattamente dove si trova la coda in pixel
		float pixelX = static_cast<float>(tailCell.x * Config::CELL_SIZE);
		float pixelY = static_cast<float>(tailCell.y * Config::CELL_SIZE);
		newShape.setPosition({ pixelX, pixelY });

		// 5) Aggiungo il nuovo rettangolo in fondo a testBodyShapes
		testBodyShapes.push_back(newShape);
	}

	// NOTA: non serve ricampionare tutte le posizioni qui,
	// perché TestMove() e MyDrawSnake() si occupano di riallineare
	// in ogni tick. Se vuoi vedere subito i segmenti extra, richiamare dopo questa funzione:
	//
	//     for (size_t i = 0; i < testBodyPos.size(); ++i)
	//     {
	//         sf::Vector2i cell = testBodyPos[i];
	//         float x = static_cast<float>(cell.x * Config::CELL_SIZE);
	//         float y = static_cast<float>(cell.y * Config::CELL_SIZE);
	//         testBodyShapes[i].setPosition({ x, y });
	//     }
	//
	// ma non è strettamente necessario se poi, nel game‐loop, chiami TestMove().
}
