
//----------------    Pre-Processor directives  ------------------
#define DEVELOPING_MODE
//#define SHOW_ALL_MONEY_BAGS
//#define SAVE_POINT
#define MAP_BUILDER_ON

//---------------    STD Libraries  -------------------------
#include "raylib.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//----------------    MY FILES      -------------------------
#include "map_builder.h"
#include "money_bag.h"
#include "Player.h"
#include "utility_file.h"

//----------------    Pre-Processor directives Constants  ------------------
#define MAX_RECT 200
#define MAX_INPUT_TEXT 30
#define MAX_INPUT_NUM 6

//----------------   GLOBAL CONSTANTS     --------------------
//const int screenWidth = 1800;
//const int screenHeight = 1080;
const int screenHeight = 900;
const int screenWidth = 16.0f/9.0f*900;

//----------------   GLOBAL VARIABLES     --------------------



//-----------------    STRUCTURES   --------------------------

//-----------------    FUNCTIONS    --------------------------
void startMenu();
void mainGame();
//-----------------      MAIN       --------------------------   

void main() {
	//-------------		LOCAL CONSTANTS   ---------------------
	
	//-------------		LOCAL VARIABLES   ---------------------
	
	InitWindow(screenWidth ,screenHeight, "Smuggle the Money");

	startMenu();
	//mainGame();
}

//---------------------------------------------------------------
//---------------                     ---------------------------
//---------------    START MENU       ---------------------------
//---------------                     ---------------------------
//---------------------------------------------------------------
void startMenu() {
	SetTargetFPS(60);

	Button newGameBtn;

	initButton(&newGameBtn , (Vector2){20,20} , "Click Here");
	newGameBtn.padding.top = 100;
	Button btn2;
	initButton(&btn2, (Vector2) { 20, 260 }, "2nd Button");


	char str[] = "12345";
	TextEdit textEdit;
	initTextEdit(&textEdit, (Vector2) { 200, 100 } , str , sizeof(str));
	setTextEditFontSize(&textEdit, 100);

	char str2[20] = "123124234";
	TextEdit edit2;
	initTextEdit(&edit2, (Vector2) { 500, 0 }, str2 , sizeof(str2));
	edit2.type = NUMBER;
	edit2.activate_with_mouse = 0;
	
	while (1) {
		if (WindowShouldClose())
			CloseWindow();

		if (isButtonPressed(newGameBtn)) {
			printf("\nButton has been clicked");
		}
		if (CheckCollisionPointCircle(GetMousePosition(), (Vector2) { 700, 700 }, 10) || isTextEditHover(textEdit))
			textEdit.textEditState = ACTIVE;
		else
			textEdit.textEditState = INACTIVE;


		BeginDrawing();
		ClearBackground(BLACK);
		drawButton(&newGameBtn );
		drawButton(&btn2 );
		drawTextEdit(&textEdit);
		drawTextEdit(&edit2);
		DrawCircle(700, 700, 10, MAGENTA);

		EndDrawing();

	}
}


//---------------------------------------------------------------
//---------------                     ---------------------------
//---------------   MAIN GAME SCENE   ---------------------------
//---------------                     ---------------------------
//---------------------------------------------------------------
void mainGame() {
	SetTargetFPS(60);
	//-------------		LOCAL CONSTANTS   ---------------------

	//-------------     LOCAL VARIABLES   ---------------------
	Camera2D camera = {0};
	RectEx baseA, baseB, inspectionRoom;
	Player player;

	RectEx walls[MAX_RECT];
	unsigned int walls_length = 0;

	RectEx grounds[MAX_RECT];
	unsigned int grounds_length = 0;

	MoneyBag moneyBag;

#ifdef DEVELOPING_MODE
	#ifdef SHOW_ALL_MONEY_BAGS
		Rectangle moneyBags_Points[MAX_RECT];
		unsigned int moneyBags_length = 0;
	#endif
#endif
	

	//--------- Initialization --------------------


	//  Initializing camera
	camera.offset = (Vector2){screenWidth / 2 , screenHeight / 2};
	camera.rotation = 0.0f;
	camera.zoom = 2.0f;
	Vector2 cam_focus = { 30,30 }; 

	// Initializing Base A
	baseA.rect = (Rectangle){ 0 , 0 , 1500,1500 };
	baseA.color = GREEN;
	// Intializing Border
	Rectangle border = (Rectangle){ baseA.rect.x + baseA.rect.width , baseA.rect.y + baseA.rect.height / 2 , 2000 , 300 };
	Color border_color = BEIGE;
	// Initializing Base B
	baseB.rect = (Rectangle){ baseA.rect.x + baseA.rect.width + border.width , baseA.rect.y  , baseA.rect.width , baseA.rect.height };
	baseB.color = BLUE;
	//  Initializing Inspection Room
	inspectionRoom.rect = (Rectangle){ border.x + 200 , baseA.rect.y-1000 , border.width - 200*2 , 1000};
	inspectionRoom.color = BROWN;

	//  Intializing Player
	player.max_frames = 10;
	player.default_frame = 7;
	player.current_frame = player.default_frame;
	player.state = STOP;
	player.direction = RIGHT;
	loadPlayerFrames(&player, "./Game_Resources/player_1_anim_" , 100 , 100);

	// Intialzing walls and ground from stored map file
	readRectEx("./Game_Resources/map/walls.map", walls, &walls_length);
	readRectEx("./Game_Resources/map/ground.map", grounds , &grounds_length);
	
	// Initiliazing All Money Bag Points for developing mode only
#ifdef DEVELOPING_MODE
	#ifdef SHOW_ALL_MONEY_BAGS
		loadAllMoneyBagPoints("./Game_Resources/map/spawnable_base_b.map" , &moneyBags_Points , &moneyBags_length  , 100 , 100 );
	#endif
#endif

	//  Initializing moneyBag
	Image moneyBagImg = LoadImage("./Game_Resources/money_bag.png");
	ImageResize(&moneyBagImg, 100, 100);
	moneyBag.texture = LoadTextureFromImage(moneyBagImg);
	UnloadImage(moneyBagImg);
	moneyBag.position = GetRandomMoneyBagCoord("./Game_Resources/map/spawnable_base_a.map");

	//-------------   GAME LOOP START   ---------------	
	while (1) {
		if (WindowShouldClose())
			CloseWindow();
		
		movePlayerIfKeyPressed(&player, &cam_focus, walls, walls_length);
		
	#ifdef DEVELOPING_MODE
		// Move with Mouse only in developing mode
		if (IsKeyDown(KEY_LEFT_ALT))
		{
			Vector2 mouseDelta = GetMouseDelta();
			cam_focus.x -= mouseDelta.x;
			cam_focus.y -= mouseDelta.y;
		}
		#ifdef SAVE_POINT
			if (IsKeyPressed(KEY_Z)) {
				savePoint(cam_focus, "spawnable_base.map");
				printf("\nPoint saved at Focus ( %f , %f )", cam_focus.x, cam_focus.y);
			}
		#endif
	#endif

		camera.target = cam_focus;

		
		//-------- DRAW -----------

		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(camera);

		// Draw Base A
		DrawRectangleRec(baseA.rect, baseA.color);
		// Draw Border 
		DrawRectangleRec(border, border_color);
		// Draw Base B
		DrawRectangleRec(baseB.rect, baseB.color);
		// Draw Inspection Room
		DrawRectangleRec(inspectionRoom.rect, inspectionRoom.color);
		// Draw walls
		drawRectsEx(walls , walls_length);
		// Draw Ground
		drawRectsEx(grounds, grounds_length);
		// Draw Money Bag
		DrawTexture(moneyBag.texture, moneyBag.position.x, moneyBag.position.y, WHITE);
		//   Draw player 
		DrawPlayer(&player, cam_focus, WHITE);

#ifdef DEVELOPING_MODE
	#ifdef MAP_BUILDER_ON
		buildRectEx(cam_focus, walls, &walls_length, "./Game_Resources/map/walls.map", 0);
	#endif
	#ifdef SHOW_ALL_MONEY_BAGS
		drawAllMoneyBags(moneyBag.texture, moneyBags_Points, moneyBags_length);
	#endif
	#ifdef DEVELOPING_MODE
		// horizontal Line
		DrawLine(-60000, cam_focus.y, 60000, cam_focus.y, WHITE);
		// vertical line
		DrawLine(cam_focus.x, -100000, cam_focus.x, 100000, WHITE);
	#endif
#endif

		EndMode2D();
		EndDrawing();
	}
}





