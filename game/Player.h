#pragma once
#include "raylib.h"
#include "utility_file.h"


//-----------------    Pre-Processor Directives Constants  ------------
#define PLAYER_MOVING_SPEED 3

//-----------------    ENUMS        --------------------------
typedef enum PlayerState {
	MOVING,
	STOP
}PlayerState;
typedef enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
}Direction;
//-----------------    STRUCTS       --------------------------
typedef struct Player {
	Texture* frames;
	int max_frames;
	int current_frame;
	int default_frame; // this is the default frame when player is not moving
	PlayerState state;
	Direction direction;

} Player;

//-----------------    FUNCTIONS     --------------------------

void loadPlayerFrames(Player* player, const char* baseFileName, const int width, const int height);
void playerNewFrame(Player* player);
void DrawPlayer(Player* player, Vector2 position, Color tint);
void movePlayerIfKeyPressed(Player* player, Vector2* cam_focus, RectEx* walls, unsigned int walls_length);
