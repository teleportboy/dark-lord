#include "stdio.h"
#include "windows.h"

#include "ship.h"
#include "UserInterface.h"

//Отобразить поле игрока и поле противника.
void DisplayGameField(char** playerGameField, char** enemyGameField, Ship* playerShips)
{
	DisplayPlayerGameField(playerGameField, playerShips);
	DisplayEnemyGameField(enemyGameField);
}

//Отобразить поле игрока.
void DisplayPlayerGameField(char** playerGameField, Ship* playerShips)
{	
	int shipID;

	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			SetConsoleCursor(j * 2 + 30, i + 5);

			//Если по текущим смещениям в массиве хранится "часть" корабля, то менять цвет относительно типа корабля.
			if (playerGameField[i][j] >= '0' && playerGameField[i][j] <= '9')
			{
				shipID = playerGameField[i][j] - 48;

				//Менять цвет относительно типа корабля.
				switch (GetShipType(playerShips[shipID]))
				{
				case SingleDeck:
					SetColor(9);
					break;

				case DoubleDeck:
					SetColor(11);
					break;

				case TripleDeck:
					SetColor(13);
					break;

				case QuadrupleDeck:
					SetColor(14);
					break;
				}
				putchar('\xFE');
			}
			else //Иначе по текущим смещениям в массиве хранятся другие данные.
			{
				switch (playerGameField[i][j])
				{
				case MISS:
					SetColor(6);
					break;

				case HIT:
					SetColor(10);
					break;

				case FREE_SPACE_IN_SEA:
					SetColor(8);
					break;
				}
				printf("%c", playerGameField[i][j]);
			}
		}
	}
}

//Отобразить поле противника.
void DisplayEnemyGameField(char** enemyGameField)
{

	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			SetConsoleCursor(j * 2 + 5, i + 5);

			//Не показывать где расположены корабли противника.
			if (enemyGameField[i][j] >= '0' && enemyGameField[i][j] <= '9')
			{
				SetColor(8);
				printf("%c", FREE_SPACE_IN_SEA);
				continue;
			}

			switch (enemyGameField[i][j])
			{
			case MISS:
				SetColor(6);
				printf("%c", enemyGameField[i][j]);
				break;

			case HIT:
				SetColor(10);
				printf("%c", enemyGameField[i][j]);
				break;

			case FREE_SPACE_IN_SEA:
				SetColor(8);
				printf("%c", enemyGameField[i][j]);
				break;
			}
		}
	}
}

//Отобразить игровую систему координат.
void DisplayCoordinateSystem()
{
	unsigned char alpha = 'A';
	for (int i = 0; i < 10; i++)
	{
		SetConsoleCursor(i * 2 + 5, 4);
		printf("%c", alpha + i);
		SetConsoleCursor(3, i + 5);
		printf("%d", i + 1);
	}

	for (int i = 0; i < 10; i++)
	{
		SetConsoleCursor(i * 2 + 30, 4);
		printf("%c", alpha + i);
		SetConsoleCursor(28, i + 5);
		printf("%d", i + 1);
	}
}

//Отобразить результат игры.
void DisplayGameOutcome(Ship* playerShips, Ship* enemyShips)
{
	switch (IsVictory(playerShips, enemyShips))
	{
	case Win:
		SetColor(10);
		printf("\nYOU WON!");
		break;
	case Lose:
		SetColor(12);
		printf("\nY0U lOSE");
		break;
	}
}

//Отображать курсор при выборе точки для атаки.
void DisplaySelectionCursor(char** enemyGameField, Location coordinates)
{
	DisplayEnemyGameField(enemyGameField);
	SetConsoleCursor(coordinates.y * 2 + 5, coordinates.x + 5);
	SetColor(15);
	putchar('\xFE');
}

//Отобразить результат атаки.
void DisplayShotResult(int shotResult)
{
	switch (shotResult)
	{
	case True:
		SetConsoleCursor(5, 1);
		SetColor(10);
		printf("NICE SHOT!");
		SetColor(8);
		break;
	case False:
		SetConsoleCursor(5, 1);
		SetColor(12);
		printf("MISS!        ");
		SetColor(8);
		break;
	}
}

void DisplaySunkedResult()
{
	SetColor(10);
	SetConsoleCursor(5, 1);
	printf("SHIP SUNKED!");
}

void DisplayOtherInfo()
{
	SetConsoleCursor(5, 3);
	SetColor(12);
	printf("ENEMY GAME FIELD");

	SetConsoleCursor(30, 3);
	SetColor(9);
	printf("YOUR GAME FIELD");

	SetConsoleCursor(55, 5);
	SetColor(15);
	printf("Press ");
	SetColor(12);
	printf("\"UP\"    ");
	SetColor(15);
	printf("to move up the cursor.");

	SetConsoleCursor(55, 6);
	printf("Press ");
	SetColor(12);
	printf("\"DOWN\"  ");
	SetColor(15);
	printf("to move down the cursor.");

	SetConsoleCursor(55, 7);
	printf("Press ");
	SetColor(12);
	printf("\"LEFT\"  ");
	SetColor(15);
	printf("to move left the cursor.");

	SetConsoleCursor(55, 8);
	printf("Press ");
	SetColor(12);
	printf("\"RIGHT\" ");
	SetColor(15);
	printf("to move right the cursor.");

	SetConsoleCursor(55, 9);
	printf("Press ");
	SetColor(12);
	printf("\"ENTER\" ");
	SetColor(15);
	printf("to confirm the attack.");
}

//Установить цвет.
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Установить курсор.
void SetConsoleCursor(int x, int y)
{
	COORD cursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

//Спрятать курсор.
void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}