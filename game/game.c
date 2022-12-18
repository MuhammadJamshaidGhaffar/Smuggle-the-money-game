
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
#define PLAYER_NAME_LEN 30

//----------------   GLOBAL CONSTANTS     --------------------
//const int screenWidth = 1800;
//const int screenHeight = 1080;
const int screenHeight = 900;
const int screenWidth = 16.0f/9.0f*900;

//----------------   GLOBAL VARIABLES     --------------------
char player_1_name[PLAYER_NAME_LEN + 1] = "Player 1";
char player_2_name[PLAYER_NAME_LEN + 1] = "Player 2";


//-----------------    STRUCTURES   --------------------------

//-----------------    FUNCTIONS    --------------------------
void startMenu();
void settingsScene();
void creditsScene();
void mainGameScene();
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
	//--------- Local Constants ----------
	const int baseX = 649;
	const int baseY = 265;
	//--------- Variables ------------
	Button newGameBtn , settingsBtn , creditsBtn;

	initButton(&newGameBtn , (Vector2){baseX , baseY} , "NewGame");
	newGameBtn.borderThickness = (BorderThickness){ 0,0,2,0 };
	
	initButton(&settingsBtn , (Vector2){baseX,0 }, "Settings");
	setButtonAtCertainMarginYFromBtn(&settingsBtn, newGameBtn, 40);
	settingsBtn.borderThickness = (BorderThickness){ 0,0,2,0 };
	
	initButton(&creditsBtn , (Vector2){baseX,0 }, "Credits");
	setButtonAtCertainMarginYFromBtn(&creditsBtn, settingsBtn, 40);
	creditsBtn.borderThickness = (BorderThickness){ 0,0,2,0 };

	
	while (1) {
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		printf("\n( %d , %d )", GetMouseX(), GetMouseY());

		if (WindowShouldClose())
			CloseWindow();

		if (isButtonPressed(newGameBtn))
			mainGameScene();
		if (isButtonPressed(settingsBtn))
			settingsScene();
		if (isButtonPressed(creditsBtn)) {
			creditsScene();
		}

		BeginDrawing();
		ClearBackground(BLACK);
		drawButton(&newGameBtn );
		drawButton(&settingsBtn);
		drawButton(&creditsBtn);
		EndDrawing();

	}
}

void settingsScene() {
	SetTargetFPS(60);

	Button backBtn;
	initButton(&backBtn, (Vector2) { 63, 50 }, "Go Back");

	TextEdit player_1_name_edit;
	initTextEdit(&player_1_name_edit, (Vector2) { 226, 272 } , player_1_name , sizeof(player_1_name));
	setTextEditFontSize(&player_1_name_edit, 50);
	
	TextEdit player_2_name_edit;
	initTextEdit(&player_2_name_edit, (Vector2) { 226, 495 } , player_2_name , sizeof(player_2_name));
	setTextEditFontSize(&player_2_name_edit, 50);

	while (1) {
#ifdef DEVELOPING_MODE
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			printf("\n( %d , %d )", GetMouseX(), GetMouseY());
#endif
		if (WindowShouldClose())
			CloseWindow();
		if (isButtonPressed(backBtn))
			break;

		BeginDrawing();
		ClearBackground(BLACK);
		drawButton(&backBtn);
		// Display Label for player 1 Name
		DrawText("Player 1 Name", 226, 200, 50, GREEN);
		drawTextEdit(&player_1_name_edit);
		// Display Label for player 1 Name
		DrawText("Player 2 Name", 226, 423, 50, BLUE);
		drawTextEdit(&player_2_name_edit);
		EndDrawing();
	}
}

void creditsScene() {
	SetTargetFPS(10);

	Button backBtn;
	initButton(&backBtn, (Vector2) { 63, 50 }, "Go Back");


	while (1) {
#ifdef DEVELOPING_MODE
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			printf("\n( %d , %d )", GetMouseX(), GetMouseY());
#endif
		if (WindowShouldClose())
			CloseWindow();
		if (isButtonPressed(backBtn))
			break;

		BeginDrawing();
		ClearBackground(BLACK);
		drawButton(&backBtn);
		DrawText("Created By : ", 450, 265, 50, GREEN);
		DrawText("Muhammad Jamshaid Ghffar", 678, 370, 50, RED);
		EndDrawing();
	}
}

//---------------------------------------------------------------
//---------------                     ---------------------------
//---------------   MAIN GAME SCENE   ---------------------------
//---------------                     ---------------------------
//---------------------------------------------------------------
void mainGameScene() {

	SetTargetFPS(60);
	//-------------		LOCAL CONSTANTS   ---------------------
	const int headerHeight = 100;

	//-------------     LOCAL VARIABLES   ---------------------
	int player_1_money = 0;
	int player_2_money = 0;
	int turn = 1;

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
#ifdef DEVELOPING_MODE
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			printf("\n( %d , %d )", GetMouseX(), GetMouseY());
#endif
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
		// Draw things which are not in 2d Mode
		// draw Header on top
		DrawRectangle(0, 0, screenWidth, headerHeight, LIME);
		char player_1_money_text[50];
		char player_2_money_text[50];
		char turn_text[30];
		sprintf(player_1_money_text, "%s's Money : %d", player_1_name, player_1_money);
		sprintf(player_2_money_text, "%s's Money : %d", player_2_name, player_2_money);
		sprintf(turn_text, "Turn : %d", turn);
		DrawText(player_1_money_text, 59, 33, 30, WHITE);
		DrawText(player_2_money_text, 1183, 33, 30, WHITE);
		DrawText(turn_text, 674, 33, 30, WHITE);


		EndDrawing();
	}
}




