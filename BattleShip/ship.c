#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"

#include "ship.h"
#include "UserInterface.h"

//Ход игрока.
void GetPlayerAttack(Ship* ships, char** enemyGameField)
{
	Location coordinates; 
	char target;
	int attackResult;

	do
	{
		coordinates = GetAttackCoordinates(enemyGameField);      //Получить координаты по которым совершить атаку.
		target = enemyGameField[coordinates.x][coordinates.y];   //Получить то что хранится по раннее полученным координатам.

		//Атаковать.
		attackResult = Attack(ships, enemyGameField, coordinates, target);

		//Отобразить вражеское поле.
		DisplayEnemyGameField(enemyGameField);
		
		switch (attackResult)
		{
		case True:                                               //Если атака была успешной отобразить результат атаки.
			DisplayShotResult(True);
			if (IsSunk(ships, enemyGameField, target) == True)   //Если в результате был потоплен корабль отобразить это.
			{
				DisplaySunkedResult();
			}
			continue;                                            //Атаковать еще раз.
			break;

		case False:                                              //При неудаче прекратить атаку.
			DisplayShotResult(False);
			return;
		}

	} while (True);
}

//Атака копмпьютера. Атакует в рандомные точки.
void GetComputerAttack(Ship* playerShips, char** playerGameField)
{
	Location coordinates;
	char target;
	int attackResult;

	do
	{
		do
		{
			coordinates.x = rand() % 10;
			coordinates.y = rand() % 10;
			target = playerGameField[coordinates.x][coordinates.y];
			
			if (target != HIT && target != MISS)
			{
				break;
			}
		} while (1);

		attackResult = Attack(playerShips, playerGameField, coordinates, target);

		switch (attackResult)
		{
		case True:
			continue;

		case False:
			return;
		}

	} while (True);
}

int GetAfloatShips(Ship* ships)
{
	int afloatShipsCounter = 0;
	
	//Посчитать количество кораблей на плаву.
	for (size_t i = 0; i < 10; i++)
	{
		if (ships[i].shipStatus.status == Afloat)
		{
			afloatShipsCounter++;
		}
	}

	return afloatShipsCounter;
}

//Проверка на победу.
int IsVictory(Ship* playerShips, Ship* enemyShips)
{
	if (GetAfloatShips(playerShips) == 0)   //Если "живых" кораблей у игрока не осталось - поражение.
	{
		return Lose;
	}
	if (GetAfloatShips(enemyShips) == 0)   //Если "живых" кораблей у противника не осталось - победа.
	{
		return Win;
	}
	return GameOn;                        //Иначе, игра продолжается.
}

//Выбрать координаты для атаки.
Location GetAttackCoordinates(char** enemyGameField)
{
	int key;
	char target;
	static Location coordinates = { 0, 0 };

	DisplaySelectionCursor(enemyGameField, coordinates);

	do
	{
		//Считать управляющую клавишу.
		key = getch();
		if (key == 0xE0)
		{
			key = key << 8;
			key = key | getch();
		}

		//Взависимости от нажатой клавиши менять координаты соответствующим образом.
		switch (key)
		{
		case UP:
			coordinates.x--;
			if (coordinates.x < 0)
			{
				coordinates.x = 0;
			}
			break;

		case DOWN:
			coordinates.x++;
			if (coordinates.x > 9)
			{
				coordinates.x = 9;
			}
			break;

		case LEFT:
			coordinates.y--;
			if (coordinates.y < 0)
			{
				coordinates.y = 0;
			}
			break;

		case RIGHT:
			coordinates.y++;
			if (coordinates.y > 9)
			{
				coordinates.y = 9;
			}
			break;

			//Выбрать точку для атаки.
		case ENTER:
			target = enemyGameField[coordinates.x][coordinates.y];
			if (target == HIT || target == MISS)
			{
				continue;
			}
			return coordinates;
		}

		//Отображать курсор.
		DisplaySelectionCursor(enemyGameField, coordinates);

	} while (True);
}

//Атаковать.
int Attack(Ship* ships, char** gameField, Location coordinates, char target)
{
	//Если в target хранятся цифры значит попадение по кораблю.
	if (target >= '0' && target <= '9')
	{
		gameField[coordinates.x][coordinates.y] = HIT;
		ships[target - 48].shipStatus.hitPoints--;

		if (ships[target - 48].shipStatus.hitPoints == 0)
		{
			ships[target - 48].shipStatus.status = Sunk;
			return True;
		}

		return True;
	}
	
	//Иначе - промах.
	gameField[coordinates.x][coordinates.y] = MISS;

	return False;
}

//Проверить потоплен ли корабль или нет.
int IsSunk(Ship* ships, char** gameField, char target)
{
	if (target >= '0' && target <= '9')
	{
		if (ships[target - 48].shipStatus.status == Sunk)
		{
			return True;
		}
	}
	return False;
	
}

//Установить рандомным путём корабли.
void SetShipsRandomly(Ship* ships, char** gameField)
{	
	SetQuadrupleDeckRandomly(ships, gameField);   //Установить четырёхпалубный корабль.
	SetTripleDeckRandomly(ships, gameField);      //Установить трёхпалубные корабли.
	SetDoubleDeckRandomly(ships, gameField);      //Установить двухпалубные корабли.
	SetSingleDeckRandomly(ships, gameField);      //Установить однопалубные корабли.
}

//Установить четырёхпалубный корабль.
void SetQuadrupleDeckRandomly(Ship* ships, char** gameField)
{
	int position;
	short x, y;
	Location coordinates;
	
	do
	{
		//Рандомить положение корабля.
		position = rand() % 2;

		//Рандомить стартовое положение корабля в массиве взависимости от его положения.
		if (position == Horizontal)
		{
			x = rand() % 10;
			y = rand() % 6;
		}
		else
		{
			x = rand() % 6;
			y = rand() % 10;
		}
		
		//Установить координаты корабля.
		SetXY(&coordinates, x, y);

		//Установить корабль. Итерировать снова если корабль не удалось установить.
	} while (SetShip(&ships[0], gameField, coordinates, position, QuadrupleDeck) == False);
}

//Установить трёхпалубный корабль.
void SetTripleDeckRandomly(Ship* ships, char** gameField)
{
	int position;
	short x, y;
	Location coordinates;

	for (size_t i = 1; i < 3; i++)
	{
		do
		{
			position = rand() % 2;
			if (position == Horizontal)
			{
				x = rand() % 10;
				y = rand() % 7;
			}
			else
			{
				x = rand() % 7;
				y = rand() % 10;
			}

			SetXY(&coordinates, x, y);

		} while (SetShip(&ships[i], gameField, coordinates, position, TripleDeck) == False);
	}	
}

//Установить двухпалубный корабль.
void SetDoubleDeckRandomly(Ship* ships, char** gameField)
{
	int position;
	short x, y;
	Location coordinates;

	for (size_t i = 3; i < 6; i++)
	{
		do
		{
			position = rand() % 2;
			if (position == Horizontal)
			{
				x = rand() % 10;
				y = rand() % 8;
			}
			else
			{
				x = rand() % 8;
				y = rand() % 10;
			}

			SetXY(&coordinates, x, y);

		} while (SetShip(&ships[i], gameField, coordinates, position, DoubleDeck) == False);
	}
}

//Установить однопалубный корабль.
void SetSingleDeckRandomly(Ship* ships, char** gameField)
{
	int position;
	short x, y;
	Location coordinates;

	for (size_t i = 6; i < 10; i++)
	{
		do
		{
			position = rand() % 2;

			x = rand() % 10;
			y = rand() % 10;

			SetXY(&coordinates, x, y);

		} while (SetShip(&ships[i], gameField, coordinates, position, SingleDeck) == False);
	}
}

//Инициализировать игровое поле.
char** InitGameField()
{
	//Выделить память.
	char** gameField = (char**)calloc(10, sizeof(char*));
	for (size_t i = 0; i < 10; i++)
	{
		gameField[i] = (char*)calloc(10, sizeof(char));
	}

	//Инициализировать массив.
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			gameField[i][j] = FREE_SPACE_IN_SEA;
		}
	}

	//Вернуть указатель на массив.
	return gameField;
}

//Установить корабль.
int SetShip(Ship* ship, char** gameField, Location startIndex, enum Position position, enum Type shipType)
{
	static char shipID = '0';
	if (shipID > '9')
	{
		shipID = '0';
	}

	if (CheckSpace(gameField, startIndex, position, shipType) == True)
	{
		SetLocation(ship, shipType);

		for (short i = 0; i < shipType; i++)
		{
			switch (position)
			{
			case Horizontal:
				SetX(ship, i, startIndex.x);
				SetY(ship, i, startIndex.y);

				gameField[startIndex.x][startIndex.y] = shipID;

				startIndex.y += 1;
				break;

			case Vertical:
				SetX(ship, i, startIndex.x);
				SetY(ship, i, startIndex.y);

				gameField[startIndex.x][startIndex.y] = shipID;

				startIndex.x += 1;
				break;
			}
		}

		SetHitPoints(ship, shipType);
		SetShipStatus(ship, Afloat);
		SetShipType(ship, shipType);

		shipID += 1;

		return True;
	}

	return False;
}

//Проверить можно ли ставить корабль по указанным координатам.
int CheckSpace(char** gameField, Location coordinates, enum Position position, enum Type shipType)
{
	int counter = 0;

	switch (position)
	{
	case Horizontal:
		for (int i = 0; i < shipType; i++)
		{
			if (gameField[coordinates.x][coordinates.y] == FREE_SPACE_IN_SEA)
			{
				counter++;
				coordinates.y += 1;
			}
		}
		break;

	case Vertical:
		for (int i = 0; i < shipType; i++)
		{
			if (gameField[coordinates.x][coordinates.y] == FREE_SPACE_IN_SEA)
			{
				counter++;
				coordinates.x += 1;
			}
		}
		break;
	}

	if (counter == shipType)
	{
		return True;
	}
	return False;
}

//Очистить память.
void FreeMemory(Ship* playerShips, char** playerGameField, Ship* enemyShips, char** enemyGameField)
{
	for (size_t i = 0; i < 10; i++)
	{
		free(playerShips[i].Location);
		playerShips[i].Location = NULL;
		free(enemyShips[i].Location);
		enemyShips[i].Location = NULL;
	}

	free(playerShips);
	playerShips = NULL;
	free(enemyShips);
	enemyShips = NULL;

	for (size_t i = 0; i < 10; i++)
	{
		free(playerGameField[i]);
		playerGameField[i] = NULL;
		free(enemyGameField[i]);
		enemyGameField[i] = NULL;
	}

	free(playerGameField);
	playerGameField = NULL;
	free(enemyGameField);
	enemyGameField = NULL;
}

//Сеттеры.
void SetShipType(Ship* ship, enum Type shipType)
{
	ship->shipType.type = shipType;
}

void SetHitPoints(Ship* ship, short hitPoints)
{
	ship->shipStatus.hitPoints = hitPoints;
}

void SetShipStatus(Ship* ship, enum Type shipStatus)
{
	ship->shipStatus.status = shipStatus;
}

void SetX(Ship* ship, short index, short coordByX)
{
	ship->Location[index].x = coordByX;
}

void SetY(Ship* ship, short index, short coordByY)
{
	ship->Location[index].y = coordByY;
}

void SetLocation(Ship* ship, enum Type shipType)
{
	ship->Location = calloc(shipType, sizeof(Location));
}

void SetXY(Location* location, short x, short y)
{
	location->x = x;
	location->y = y;
}

//Геттеры.
short GetShipType(Ship ship)
{
	return ship.shipType.type;
}

short GetHitPoints(Ship ship)
{
	return ship.shipStatus.hitPoints;
}

short GetShipStatus(Ship ship)
{
	return ship.shipStatus.status;
}

short GetX(Ship ship, short index)
{
	return ship.Location[index].x;
}

short GetY(Ship ship, short index)
{
	return ship.Location[index].y;
}