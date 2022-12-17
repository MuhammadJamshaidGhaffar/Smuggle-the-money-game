#include "money_bag.h"
#include <stdio.h>


Vector2 GetRandomMoneyBagCoord(char* fileName) {
	SetRandomSeed(time(0));
	FILE* file;
	file = fopen(fileName, "rb");
	if (file == NULL)
	{
		printf("\nfailed to open file %s", fileName);
		return (Vector2) { 0, 0 };
	}

	unsigned int number_of_points;
	fread(&number_of_points, sizeof(number_of_points), 1, file);

	if (number_of_points > 0)
	{
		int randomIndex = GetRandomValue(0, number_of_points - 1);
		fseek(file, randomIndex * sizeof(Vector2), SEEK_CUR);
		Vector2 point;
		fread(&point, sizeof(Vector2), 1, file);
		fclose(file);
		return point;
	}
	return (Vector2) { 0, 0 };
	
}

void savePoint(Vector2 point, char* fileName) {
	unsigned int number_of_points;
	FILE* file;
	//  read the number of points 1st
	file = fopen(fileName, "rb+");
	if (file == NULL)
	{
		printf("\nfailed to open file %s", fileName);
		return;
	}
	fread(&number_of_points, sizeof(number_of_points), 1, file);
	// now increase the number_of_points by 1
	number_of_points++;
	fseek(file, 0, SEEK_SET);
	fwrite(&number_of_points, sizeof(number_of_points), 1, file);
	// now append the point in file
	fseek(file, 0, SEEK_END);
	fwrite(&point, sizeof(point), 1, file);
	fclose(file);
}

void loadAllMoneyBagPoints(const char* const fileName ,Rectangle* moneyBag_points ,unsigned int* moneyBags_length , int width , int height) {
	FILE* file = fopen(fileName, "rb");
		if (file == NULL)
		{
			printf("\nfailed to load all money Bags");
		}
		else
		{
			printf("\nThe file %s is now opened." , fileName);
			unsigned int number_of_points;
			fread(&number_of_points, sizeof(number_of_points), 1, file);
			*moneyBags_length = number_of_points;
			for (int i = 0; i < number_of_points; i++) {
				Vector2 point;
				fread(&point, sizeof(point), 1, file);
				moneyBag_points[*moneyBags_length] = (Rectangle){ point.x , point.y , width , height };
				*moneyBags_length += 1;
			}
			// Closing the file using fclose()
			fclose(file);
			printf("\nData successfully read from file %s" , fileName);
			printf("\nThe file %s  is now closed. " , fileName);
		}
}

void drawAllMoneyBags(Texture moneyBag_texture, Rectangle* moneyBag_points, unsigned int moneyBags_length) {
	for (int i = 0; i < moneyBags_length; i++) {
		DrawTexture(moneyBag_texture, moneyBag_points[i].x, moneyBag_points[i].y, WHITE);
	}
}
