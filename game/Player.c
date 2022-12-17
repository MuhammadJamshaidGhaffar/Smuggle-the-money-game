#include "Player.h"
#include <string.h>
#include <stdio.h>



void DrawPlayer(Player* player, Vector2 position, Color tint) {
	playerNewFrame(player);
	Rectangle source = (Rectangle){ 0.0f , 0.0f, player->frames[player->current_frame].width , player->frames[player->current_frame].height };
	if (player->direction == LEFT)
		source.width *= -1;
	DrawTextureRec(player->frames[player->current_frame], source, position, tint);
}

void playerNewFrame(Player* player) {
	if (player->state == STOP)
	{
		player->current_frame = player->default_frame;
		return;
	}
	static float timer = 0; 
	timer += GetFrameTime();
	if (timer < 0.1f) // to control speed of animation
	{
		return;
	}
	timer = 0;
	switch (player->state) {
	case MOVING:
		player->current_frame = (player->current_frame + 1) % player->max_frames;
		break;
	}
}

void loadPlayerFrames(Player* player,const char* baseFileName , const int width, const int height )
{
	//  file type for player frames
	const char* type = ".png";
	const int type_len =  strlen(type);
	//  allocating dynamic memory for player frames
	player->frames = (Texture*)MemAlloc(sizeof(Texture) * player->max_frames);
	// calculating file_len to allocate memory for dynamic string / filename
	const int file_len = strlen(baseFileName);
	
	for (int i = 0; i < player->max_frames; i++) {
		// converting int to char
		char num[4]; // it can maximum store 999
		sprintf(num, "%d", i + 1);
		const int num_len = strlen(num);

		// alloc mem for dynamic string
		int fileName_len = file_len + num_len + type_len + 1;
		char* fileName = MemAlloc(fileName_len); // fileLen + num + format + null character
		// creating path for file
		strcpy_s(fileName, fileName_len,  baseFileName);
		strcat_s(fileName , fileName_len , num);
		strcat_s(fileName, fileName_len, type);

		// loading the image and creating texture , then saving it in player ith frame
		Image img = LoadImage(fileName);
		ImageResize(&img, width, height);
		player->frames[i] = LoadTextureFromImage(img);
		UnloadImage(img);
		MemFree(fileName);
	}
}

void movePlayerIfKeyPressed(Player* player , Vector2* cam_focus , RectEx* walls , unsigned int walls_length) {
	Vector2 temp_focus = *cam_focus; // to reset back if after moving player has collided

	//  Move Player if key is pressed
	int is_key_pressed = 0;
	if (IsKeyDown(KEY_UP))
	{
		cam_focus->y -= PLAYER_MOVING_SPEED;
		is_key_pressed = 1;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		cam_focus->y += PLAYER_MOVING_SPEED;
		is_key_pressed = 1;
	}
	if (IsKeyDown(KEY_LEFT))
	{
		player->direction = LEFT;
		cam_focus->x -= PLAYER_MOVING_SPEED;
		is_key_pressed = 1;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		cam_focus->x += PLAYER_MOVING_SPEED;
		player->direction = RIGHT;
		is_key_pressed = 1;
	}
	if (!is_key_pressed)
		player->state = STOP;
	else
		player->state = MOVING;
	// Reset the player to previous postion if after moving it has collided
	if (isCollidedWithWallsEx((Rectangle) { cam_focus->x, cam_focus->y, player->frames[player->current_frame].width, player->frames[player->current_frame].height }, walls, walls_length))
	{
		*cam_focus = temp_focus;
	}
}
