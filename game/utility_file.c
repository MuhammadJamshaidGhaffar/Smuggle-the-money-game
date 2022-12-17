#include "utility_file.h"
#include "raylib.h"
#include "string.h"
#include <stdlib.h>

int  isCollidedWithWallsEx(Rectangle rect, RectEx* walls,unsigned int walls_length) {
	int  flag = 0;
	for (int i = 0; i <= walls_length; i++) {
		if (CheckCollisionRecs(rect , walls[i].rect))
		{
			flag = 1;
			break;
		}
	}
	return flag;
}




void getInput(char* name, int max_chars ,const char* label, float screenWidth , InputType inputType) {
	// NOTE: One extra space required for null terminator char '\0'
	int letterCount = 0;
	int boxWidth = 25 * max_chars;
	if (boxWidth < 225)
		boxWidth = 225;
	Rectangle textBox = { screenWidth / 2.0f - 100, 180, boxWidth, 50};
	int mouseOnText = 1;

	int framesCounter = 0;

	SetTargetFPS(10);               // Set our game to run at 10 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (1)    // Detect window close button or ESC key
	{
		if (WindowShouldClose())
			CloseWindow();
		// Update
		//----------------------------------------------------------------------------------
		if (mouseOnText)
		{
			// Set the window's cursor to the I-Beam
			SetMouseCursor(MOUSE_CURSOR_IBEAM);

			// Get char pressed (unicode character) on the queue
			int key = GetCharPressed();

			// Check if more characters have been pressed on the same frame
			while (key > 0)
			{
				// NOTE: Only allow keys in range [32..125]
				if ((letterCount < max_chars) && ((inputType == TEXT && key >= 32 && key <= 125) ||(inputType == NUMBER && key >= 48 && key <= 57) ))
				{
						// if input type is number and it is not in range then
						name[letterCount] = (char)key;
						name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
						letterCount++;				
				}

				key = GetCharPressed();  // Check next character in the queue
			}

			if (IsKeyPressed(KEY_BACKSPACE))
			{
				letterCount--;
				if (letterCount < 0) letterCount = 0;
				name[letterCount] = '\0';
			}
			if (IsKeyPressed(KEY_ENTER))
			{
				SetTargetFPS(60);
				return;
			}
		}
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

		if (mouseOnText) framesCounter++;
		else framesCounter = 0;
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		DrawText(label, 240, 140, 20, WHITE);

		DrawRectangleRec(textBox, RAYWHITE);
		if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
		else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

		DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, RED);

		
		if (mouseOnText)
		{
			if (letterCount < max_chars)
			{
				// Draw blinking underscore char
				if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, RED);
			}
			else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, WHITE);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
}

//--------------------------------------------------------------------
//-----------------                       ----------------------------
//-----------------    BUTTON DUNCTIONS   ----------------------------
//-----------------                       ----------------------------
//--------------------------------------------------------------------
void initButton(Button* button , Vector2 position , char* text) {
	button->padding = (Padding){ 10,10,10,10 };
	button->position = (Vector2){ position.x,position.y };
	button->text = text;
	button->background_color = RED;
	button->font_color = WHITE;
	button->fontSize = 20;
	button->background_color_on_click = DARKGREEN;
	button->font_color_on_click = WHITE;
	button->background_color_on_hover = BLUE;
	button->font_color_on_hover= WHITE;
	button->buttonState = NOT_HOVER;
	button->clicked_animation_move_by = (Vector2){ 0,10 };
	button->clicked_animation_time = 1;
}

void drawButton(Button* button) {
	//------ Local Variables  --------------------

	//----------- Logic ------------------

		if (isButtonHover(*button))
			button->buttonState = HOVER;
		else
			button->buttonState = NOT_HOVER;
		if (isButtonDown(*button))
		{
			button->buttonState = CLICKED;
			button->position.x += button->clicked_animation_move_by.x;
			button->position.y += button->clicked_animation_move_by.y;
		}
	
	switch (button->buttonState)
	{
	case NOT_HOVER:
		DrawRectangle(button->position.x , button->position.y ,getButtonWidth(*button),getButtonHeight(*button), button->background_color);
		DrawText(button->text, button->position.x + button->padding.left, button->position.y + button->padding.top, button->fontSize, button->font_color);

		break;
	case HOVER:
		DrawRectangle(button->position.x, button->position.y, getButtonWidth(*button), getButtonHeight(*button), button->background_color_on_hover);
		DrawText(button->text, button->position.x + button->padding.left, button->position.y + button->padding.top, button->fontSize, button->font_color_on_hover);
		break;
	case CLICKED:

		DrawRectangle(button->position.x, button->position.y, getButtonWidth(*button), getButtonHeight(*button), button->background_color_on_click);
		DrawText(button->text, button->position.x + button->padding.left, button->position.y + button->padding.top, button->fontSize, button->font_color_on_click);
			
			// move the rect back to its position
		button->position.x -= button->clicked_animation_move_by.x;
		button->position.y -= button->clicked_animation_move_by.y;
		break;

	}
}

int isButtonHover(Button button) {
	return CheckCollisionPointRec(GetMousePosition(), getButtonRect(button));
}

int isButtonDown(Button button) {
	return IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isButtonHover(button);
}

int isButtonPressed(Button button) {
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isButtonHover(button);
}

int getButtonWidth(Button button) {
	return button.padding.left + MeasureText(button.text , button.fontSize) + button.padding.right;
}

int getButtonHeight(Button button) {
	return button.padding.top+ button.fontSize + button.padding.bottom;
}

Rectangle getButtonRect(Button button) {
	return (Rectangle) { button.position.x, button.position.y, getButtonWidth(button), getButtonHeight(button) };
}

//--------------------------------------------------------------------
//-----------------                       ----------------------------
//-----------------       LINE EDIT       ----------------------------
//-----------------                       ----------------------------
//--------------------------------------------------------------------
void initTextEdit(TextEdit* textEdit, Vector2 position ,char* buffer , int buffer_size) {
	textEdit->position = (Vector2){ position.x,position.y };
	textEdit->text = buffer;
	textEdit->max_text = buffer_size-1;
	textEdit->text_length = strlen(buffer);
	textEdit->background_color = WHITE;
	textEdit->font_color = RED;
	textEdit->border_color = RED;
	textEdit->fontSize = 20;
	textEdit->background_color_on_active = WHITE;
	textEdit->font_color_on_active = BLUE;
	textEdit->border_color_on_active = BLUE;
	textEdit->background_color_on_hover = WHITE;
	textEdit->font_color_on_hover = DARKGREEN;
	textEdit->border_color_on_hover = DARKGREEN;
	textEdit->textEditState = INACTIVE;
	textEdit->padding = (Padding){ 10,20,10,20 };
	textEdit->width = getTextEditWidth(*textEdit);
	textEdit->type = TEXT;
	textEdit->activate_with_mouse = 1;
}

int isTextEditHover(TextEdit textEdit) {
	return CheckCollisionPointRec(GetMousePosition(), getTextEditRect(textEdit));
}

int isTextEditDown(TextEdit textEdit) {
	return IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isTextEditHover(textEdit);
}

int isTextEditPressed(TextEdit textEdit) {
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isTextEditHover(textEdit);
}

int getTextEditWidth(TextEdit textEdit) {
	char* filledText = (char*)malloc(textEdit.max_text + 1);
	for (int i = 0; i < textEdit.max_text; i++) {
		filledText[i] = '_';
	}
	filledText[textEdit.max_text] = '\0';
	int textWidth = MeasureText(filledText, textEdit.fontSize);
	free(filledText);
	return textEdit.padding.left +  textWidth + textEdit.padding.right;
}

void setTextEditFontSize(TextEdit* textEdit , int fontSize) {
	textEdit->fontSize = fontSize;
	textEdit->width = getTextEditWidth(*textEdit);
}
void setTextEditPadding(TextEdit* textEdit , Padding padding) {
	textEdit->padding = padding;
	textEdit->width = getTextEditWidth(*textEdit);
}

int getTextEditHeight(TextEdit textEdit) {
	return textEdit.padding.top + textEdit.fontSize + textEdit.padding.bottom;
}

Rectangle getTextEditRect(TextEdit textEdit) {
	return (Rectangle) {textEdit.position.x, textEdit.position.y, textEdit.width, getTextEditHeight(textEdit) };
}

void drawTextEdit(TextEdit* textEdit) {

	if (textEdit->activate_with_mouse)
		setTextEditState(textEdit);

	static int framesCounter = 0;
	if (textEdit->textEditState != ACTIVE) {
		framesCounter = 0;
	}

	switch (textEdit->textEditState)
	{
	case INACTIVE:
		DrawRectangle(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->background_color);
		DrawRectangleLines(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->border_color);
		DrawText(textEdit->text, textEdit->position.x + textEdit->padding.left, textEdit->position.y + textEdit->padding.top, textEdit->fontSize, textEdit->font_color);
		break;
	case HOVER:
		DrawRectangle(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->background_color_on_hover);
		DrawRectangleLines(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->border_color_on_hover); 
		DrawText(textEdit->text, textEdit->position.x + textEdit->padding.left, textEdit->position.y + textEdit->padding.top, textEdit->fontSize, textEdit->font_color_on_hover);
		break;
	case ACTIVE:
		framesCounter++;
		DrawRectangle(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->background_color_on_active);
		DrawRectangleLines(textEdit->position.x, textEdit->position.y, textEdit->width, getTextEditHeight(*textEdit), textEdit->border_color_on_active);
		DrawText(textEdit->text, textEdit->position.x + textEdit->padding.left, textEdit->position.y + textEdit->padding.top, textEdit->fontSize, textEdit->font_color_on_active);
		// display blinking cursor
		if (((framesCounter / 20) % 2) == 0)
			DrawText("_", textEdit->position.x+textEdit->padding.left+ MeasureText(textEdit->text ,textEdit->fontSize ), textEdit->position.y+textEdit->padding.top, textEdit->fontSize, textEdit->font_color_on_active);
		
		setTextEditTextIfKeyPressed(textEdit);
		break;

	}


	}

void setTextEditState(TextEdit* textEdit) {

	if (textEdit->textEditState != ACTIVE)
	{
		if (isTextEditHover(*textEdit))
			textEdit->textEditState = HOVER;
		else
			textEdit->textEditState = INACTIVE;
	}
	if (isTextEditPressed(*textEdit))
	{
		textEdit->textEditState = ACTIVE;
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isTextEditHover(*textEdit)) {
		textEdit->textEditState = INACTIVE;
	}
}

void setTextEditTextIfKeyPressed(TextEdit* textEdit) {
	// Get char pressed (unicode character) on the queue
	int key = GetCharPressed();

	// Check if more characters have been pressed on the same frame
	while (key > 0)
	{
		if (textEdit->text_length < textEdit->max_text && ((textEdit->type == TEXT && key >= 32 && key <= 125) || (textEdit->type == NUMBER && key >= 48 && key <= 57)))
		{
			textEdit->text[textEdit->text_length] = (char)key;
			textEdit->text[textEdit->text_length+1] = '\0'; // Add null terminator at the end of the string.
			textEdit->text_length += 1;
		}
		key = GetCharPressed();  // Check next character in the queue
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		textEdit->text_length -= 1;
		if (textEdit->text_length < 0) textEdit->text_length = 0;
		textEdit->text[textEdit->text_length] = '\0';
	}
}
