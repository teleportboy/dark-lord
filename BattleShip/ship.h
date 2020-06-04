#ifndef SHIP_H
#define SHIP_H

//Макроподстановки.
#define FREE_SPACE_IN_SEA   '~'
#define HIT                 'X'
#define MISS                '#'

#define UP                  0xE048
#define DOWN                0xE050
#define LEFT                0xE04B
#define RIGHT               0xE04D
#define ESC                 0x1B
#define ENTER               0xD
#define SPACE               0x20

//Координаты.
typedef struct
{
	short x;
	short y;
}Location;

//Состояние корабля.
typedef struct
{
	short hitPoints;
	short status;
}ShipStatus;

//Тип корабля.
typedef struct
{
	short type;
}ShipType;

//Корабль.
typedef struct
{
	ShipType shipType;
	ShipStatus shipStatus;
	Location* Location;
}Ship;

//Тип корабля.
typedef enum
{
	SingleDeck = 1,
	DoubleDeck,
	TripleDeck,
	QuadrupleDeck,
}Type;

//Статус корабля.
typedef enum
{
	Sunk,
	Afloat,
}Status;

//Положение корабля.
typedef enum
{
	Horizontal,
	Vertical,
}Position;

//Статус игры.
typedef enum
{
	Win,
	Lose,
	GameOn,
}GameStatus;

//bool
typedef enum
{
	False,
	True,
}Boolean;

//Управление игрой.
char** InitGameField();
int Attack(Ship*, char**, Location, char);
int IsSunk(Ship*, char**, char);
Location GetAttackCoordinates(char**);
void GetPlayerAttack(Ship*, char**);
void GetComputerAttack(Ship*, char**);
int GetAfloatShips(Ship*);
int IsVictory(Ship*, Ship*);

//Установка корабля.
int SetShip(Ship*, char**, Location, enum Position, enum Type);
void SetShipsRandomly(Ship*, char**);
void SetQuadrupleDeckRandomly(Ship*, char**);
void SetTripleDeckRandomly(Ship*, char**);
void SetDoubleDeckRandomly(Ship*, char**);
void SetSingleDeckRandomly(Ship*, char**);
int CheckSpace(char**, Location, enum Position, enum Type);

//Сеттеры.
void SetXY(Location*, short, short);
void SetShipType(Ship*, enum Type);
void SetHitPoints(Ship*, short);
void SetShipStatus(Ship*, enum Type);
void SetX(Ship*, short, short);
void SetY(Ship*, short, short);
void SetLocation(Ship*, enum Type);

//Геттеры.
short GetShipType(Ship);
short GetHitPoints(Ship);
short GetShipStatus(Ship);
short GetX(Ship, short);
short GetY(Ship, short);

//Освобождение памяти.
void FreeMemory(Ship*, char**, Ship*, char**);

#endif // !SHIP_H
