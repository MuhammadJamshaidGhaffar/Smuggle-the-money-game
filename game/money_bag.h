#pragma once
#include "raylib.h"

//----------   STRUCTURES   -----------------
typedef struct MoneyBag {
	Texture texture;
	Vector2 position;
} MoneyBag;

//----------   FUNCTIONS    -----------------
Vector2 GetRandomMoneyBagCoord(char* fileName);
void savePoint(Vector2 point, char* fileName);
void loadAllMoneyBagPoints(const char* const fileName, Rectangle* moneyBag_points, unsigned int* moneyBags_length, int width, int height);
void drawAllMoneyBags(Texture moneyBag_texture, Rectangle* moneyBag_points, unsigned int moneyBags_length);

