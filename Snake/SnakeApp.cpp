#include "SnakeApp.h"
#include "Config.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>   


SnakeApp::SnakeApp(sf::RenderWindow& window, Food& food)
	: windRef(window), foodRef(food)
{
	isRunning = true;
	GameOver = false;
	timeToCheckUpdate = 0.6f;
	bodySize = sf::Vector2f({ static_cast<float>(Config::CELL_SIZE), static_cast<float>(Config::CELL_SIZE) });
	startPosition = sf::Vector2i(Config::GRID_SIZE / 2, Config::GRID_SIZE / 2);
	direction = sf::Vector2i(1, 0);
	snakeBodyPos.push_back(startPosition);

	body.setSize({ static_cast<float>(Config::CELL_SIZE), static_cast<float>(Config::CELL_SIZE) });
	body.setFillColor(sf::Color::Green);
	body.setPosition({ static_cast<float>(snakeBodyPos.front().x * Config::CELL_SIZE),
					   static_cast<float>(snakeBodyPos.front().y * Config::CELL_SIZE) }); // Posizione iniziale del serpente

	snakeBodyShapes.push_back(body); // Inizializzo il deque per le forme del corpo del serpente
}

void SnakeApp::Update()
{
	float totalTime = deltaClock.getElapsedTime().asSeconds(); // 3) Ottengo il tempo totale trascorso dal lancio del clock (opzionale)

	if (GameOver) {
		isRunning = false;
	}
	else if (totalTime >= timeToCheckUpdate) {

		deltaClock.restart();	//faccio ripartire da 0 il timer 

		if (!isMoving)
		{
			SnakeMove();
			CheckWindowLimits();
			SnakeEat();
			CollisionBodyDetection();
		}
	}
}

void SnakeApp::ProcessInput()
{
	//Usiamo while (auto maybeEvent = window.pollEvent()) perch� in SFML 3.x pollEvent() restituisce un std::optional<sf::Event>.
	while (auto maybeEvent = windRef.pollEvent()) {

		const sf::Event& event = *maybeEvent;

		if (event.is<sf::Event::Closed>())
		{
			windRef.close();
			isRunning = false;
		}
		else if (isRunning && event.is<sf::Event::KeyPressed>())
		{
			auto keyEvent = event.getIf<sf::Event::KeyPressed>();
			using Key = sf::Keyboard::Key;

			switch (keyEvent->code)
			{
			case Key::Up:
				std::cout << "KEY UP" << std::endl;
				isMoving = true;
				direction = { 0, -1 };


				break;
			case Key::Down:
				std::cout << "KEY Down" << std::endl;
				isMoving = true;
				direction = { 0, 1 };


				break;
			case Key::Left:
				std::cout << "KEY Left" << std::endl;
				isMoving = true;
				direction = { -1, 0 };

				break;
			case Key::Right:
				std::cout << "KEY Right" << std::endl;
				isMoving = true;
				direction = { 1, 0 };


				break;
			case Key::LShift:
				std::cout << "KEY LShift" << std::endl;
				timeToCheckUpdate = 0.1f;
				break;
			case Key::Escape:
				windRef.close();
				isRunning = false;
				isMoving = false;
				break;
			default:
				break;
			}

			isMoving = false;
		}

	}
}


void SnakeApp::SnakeMove()
{
	/*creo sempre una posizione nuova per far muovere il corpo*/
	sf::Vector2i oldPosHead = snakeBodyPos.front();
	sf::Vector2i newPosHead = sf::Vector2i({ oldPosHead.x + direction.x, oldPosHead.y + direction.y });
	snakeBodyPos.push_front(newPosHead);

	/*finche non mangio 'shouldGrow' sarà sempre false*/
	if (shouldGrow == false)
	{
		snakeBodyPos.pop_back();
	}
	else
	{
		shouldGrow = false;
	}

	for (size_t i = 0; i < snakeBodyPos.size(); i++)
	{
		float pixelX = snakeBodyPos[i].x * Config::CELL_SIZE;
		float pixelY = snakeBodyPos[i].y * Config::CELL_SIZE;
		snakeBodyShapes[i].setPosition({ pixelX, pixelY });
	}
}

void SnakeApp::MyDrawSnake()
{
	for (const auto& shape : snakeBodyShapes)
	{
		windRef.draw(shape);
	}
}


void SnakeApp::SnakeEat()
{
	if (snakeBodyPos.front() == foodRef.foodPosList.front())
	{
		std::cout << "Food mangiato" << std::endl;

		sf::Vector2i newFoodPos(foodRef.getRandomInt(0, Config::GRID_SIZE - 1), foodRef.getRandomInt(0, Config::GRID_SIZE - 1));
		foodRef.isEaten = true;
		foodRef.foodPosList.push_back(newFoodPos);
		foodRef.foodPosList.pop_front();

		GrowBy();
		foodRef.MyRedrawFood();
	}
}

void SnakeApp::DrawGrid()
{
	// 5.1: Creo due shape da riusare per tutte le linee
	sf::RectangleShape lineV(sf::Vector2f(1.f, static_cast<float>(Config::WINDOW_SIZE)));
	lineV.setFillColor(sf::Color(1000, 1000, 1000)); // grigio chiaro per le linee verticali
	lineV.setSize({ 1,Config::WINDOW_SIZE });

	sf::RectangleShape lineH(sf::Vector2f(static_cast<float>(Config::WINDOW_SIZE), 1.f));
	lineH.setFillColor(sf::Color(1000, 1000, 1000)); // stesso colore per le linee orizzontali
	lineH.setSize({ Config::WINDOW_SIZE,1 });


	// 5.2: Disegno le linee verticali a ogni multiplo di CELL_SIZE
	for (int i = 0; i <= Config::GRID_SIZE; ++i)
	{
		float x = static_cast<float>(i * Config::CELL_SIZE);
		lineV.setPosition({ x, 0.f });
		windRef.draw(lineV);
	}

	// 5.3: Disegno le linee orizzontali a ogni multiplo di CELL_SIZE
	for (int j = 0; j <= Config::GRID_SIZE; ++j)
	{
		float y = static_cast<float>(j * Config::CELL_SIZE);
		lineH.setPosition({ 0.f, y });
		windRef.draw(lineH);
	}
}

void SnakeApp::CheckWindowLimits()	//vedere se spostarlo in config.h
{
	if (snakeBodyPos.front().x < 0 || snakeBodyPos.front().x >= Config::GRID_SIZE ||
		snakeBodyPos.front().y < 0 || snakeBodyPos.front().y >= Config::GRID_SIZE)
	{
		GameOver = true;
	}
}

void SnakeApp::GrowBy(int count)
{
	for (int i = 0; i < count; ++i)
	{
		// 1) Prendo la posizione griglia dell’ultimo segmento (coda)
		sf::Vector2i tailCell = snakeBodyPos.back();

		// 2) Aggiungo la nuova cella in fondo a testBodyPos
		snakeBodyPos.push_back(tailCell);

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
		snakeBodyShapes.push_back(newShape);
	}

	// NOTA: non serve ricampionare tutte le posizioni qui,
	// perché TestMove() e MyDrawSnake() si occupano di riallineare
	// in ogni tick. Se vuoi vedere subito i segmenti extra, puoi
	// comunque richiamare dopo questa funzione:
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

void SnakeApp::GrowBy()
{
	// 1) Prendo la posizione griglia dell’ultimo segmento (coda)
	sf::Vector2i tailCell = snakeBodyPos.back();

	// 2) Aggiungo la nuova cella in fondo a testBodyPos
	snakeBodyPos.push_back(tailCell);

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
	snakeBodyShapes.push_back(newShape);


	// NOTA: non serve ricampionare tutte le posizioni qui,
	// perché TestMove() e MyDrawSnake() si occupano di riallineare
	// in ogni tick. Se vuoi vedere subito i segmenti extra, puoi
	// comunque richiamare dopo questa funzione:
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

void SnakeApp::AddBodyShape()
{
	sf::RectangleShape newShape;
	newShape.setSize(bodySize);

	snakeBodyShapes.push_back(newShape);
}

bool SnakeApp::CollisionBodyDetection()
{
	if (snakeBodyPos.size() > 2)
	{
		sf::Vector2i headPos = snakeBodyPos[0];

		for (size_t i = 0; i < snakeBodyPos.size()-1; i++)
		{
			sf::Vector2i bodyPosCheck = snakeBodyPos[i+1];

			if (headPos == bodyPosCheck)
			{
				windRef.clear();
				//windRef.draw

				GameOver = true;
				return true;
			}
		}
	}

	return false;
}

void SnakeApp::DrawFinalPage()
{
}