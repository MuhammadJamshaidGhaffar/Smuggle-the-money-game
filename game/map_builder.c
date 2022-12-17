#include "map_builder.h"
#include "raylib.h"
#include <stdio.h>

void buildRectEx(Vector2 cameraFocus, RectEx* rectsEx,unsigned int* rectsExlength , char* fileName, int shouldWrite) {
	static Vector2 initialPos = { 0,0 };
	static Vector2 finalPos = { 0,0 };
	static int startDrawing = 0;
	static RectEx rectEx;
	rectEx.color = MAGENTA;

	if (!startDrawing)
		initialPos = cameraFocus;
	// when left ctrl is pressed then start drawing the rectangle
	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		startDrawing = 1;
		finalPos = cameraFocus;
		float x_start = initialPos.x < finalPos.x ? initialPos.x : finalPos.x;
		float y_start = initialPos.y < finalPos.y ? initialPos.y : finalPos.y;
		float x_end = initialPos.x > finalPos.x ? initialPos.x : finalPos.x;
		float y_end = initialPos.y > finalPos.y ? initialPos.y : finalPos.y;
		rectEx.rect = (Rectangle){ x_start , y_start , x_end - x_start , y_end - y_start };
		DrawRectangleRec(rectEx.rect, rectEx.color);
	}
	// when left ctrl is released then write it to file
	if (IsKeyReleased(KEY_LEFT_CONTROL))
	{
		startDrawing = 0;
		if(shouldWrite)
			writeRectEx(rectEx, fileName);	
		// also append rectEx in rectEx array
		rectsEx[*rectsExlength] = rectEx;
		*rectsExlength += 1;
	}
}

int writeRectEx(RectEx rect , const char* fileName ) {
	FILE* file;
	file = fopen(fileName, "ab");
	if (file == NULL)
	{
		printf("\n%s failed to open." , fileName);
		return 0;
	}
	printf("\n%s is now opened." , fileName);
	fwrite(&rect, sizeof(rect), 1, file);
	printf("\nRectEx successfully written in file  %s" , fileName);
	fclose(file);
	printf("\nThe file is now closed.");
}

void readRectEx(const char* const fileName , RectEx* rectsEx, unsigned int* rectExlength) {
	FILE* file;
	file = fopen(fileName, "rb");
	if (file == NULL)
	{
		printf("\n%s failed to open.", fileName);
		return 0;
	}
	printf("\n%s is now opened.", fileName);
	RectEx rectEx;
	while (fread(&rectEx, sizeof(rectEx), 1, file) && file != EOF)
	{
		rectsEx[*rectExlength] = rectEx;
		*rectExlength += 1;
	}
	printf("\nRectEx successfully read from file  %s", fileName);
	fclose(file);
	printf("\nThe file is now closed.");

}


void drawRectsEx(RectEx* rectsEx, unsigned int rectsExlength) {
	for (int i = 0; i < rectsExlength; i++) {
		DrawRectangleRec(rectsEx[i].rect, rectsEx[i].color);
		
	}
}

