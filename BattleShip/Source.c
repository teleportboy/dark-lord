#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "ship.h"
#include "UserInterface.h"

int main()
{
	//Установка начала последовательности, генерируемой функ­цией rand() в последующих функциях.
	srand(time(NULL));

	//Выделение памяти под двумерные массивы типа char.
	char** playerGameField = InitGameField();
	char** enemyGameField = InitGameField();

	//Выделение памяти под одномерные массивы типа Ship.
	Ship* playerShips = calloc(10, sizeof(Ship));
	Ship* enemyShips = calloc(10, sizeof(Ship));

	//Установить корабли на игровом поле рандомно.
	SetShipsRandomly(playerShips, playerGameField);
	SetShipsRandomly(enemyShips, enemyGameField);

	HideCursor();                                                         //Отключить курсор в консоли.
	DisplayCoordinateSystem();                                            //Отобразить  игровую систему координат.
	DisplayOtherInfo();
	
	DisplayGameField(playerGameField, enemyGameField, playerShips);       //Отобразить игровые поля.
	
	while (IsVictory(playerShips, enemyShips) == GameOn)                  //Итерировать пока игра не закончена.
	{
		GetPlayerAttack(enemyShips, enemyGameField);                      //Ход игрока.
		GetComputerAttack(playerShips, playerGameField);                  //Ход компьютера.
		DisplayGameField(playerGameField, enemyGameField, playerShips);   //Отобразить игровые поля.
	}
	
	DisplayGameOutcome(playerShips, enemyShips);                          //Отобразить результаты игры.

	FreeMemory(playerShips, playerGameField, enemyShips, enemyGameField); //Освободить память.

	return 0;
}