#pragma once
#include "raylib.h"
#include "utility_file.h"


void buildRectEx(Vector2 cameraFocus, RectEx* rectsEx, unsigned int* rectsExlength , char* fileName , int shouldWrite);
int writeRectEx(RectEx rect, const char* fileName);
void readRectEx(const char* const fileName, RectEx* rectsEx, unsigned int* rectExlength);
void drawRectsEx(RectEx* rectsEx, unsigned int rectsExlength);