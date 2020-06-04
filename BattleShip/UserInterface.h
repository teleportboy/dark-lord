#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ship.h"

void DisplayGameField(char**, char**, Ship*);
void DisplayLocationInfo(Location);
void DisplayPlayerGameField(char**, Ship*);
void DisplayEnemyGameField(char**);
void DisplaySelectionCursor(char**, Location);
void DisplayShotResult(int);
void DisplaySunkedResult();
void DisplayCoordinateSystem();
void DisplayGameOutcome(Ship*, Ship*);
void DisplayOtherInfo();
void SetColor(int);
void SetConsoleCursor(int, int);
void HideCursor();


#endif // !USERINTERFACE_H
