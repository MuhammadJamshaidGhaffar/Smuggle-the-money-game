#pragma once
#include "raylib.h"


//----------------    ENUMS         --------------------
typedef enum InputType {
	NUMBER,
	TEXT
}InputType;

//----------------    STRUCTURES    --------------------

typedef struct RectEx {
	Rectangle rect;
	Color color;
} RectEx;

typedef struct Padding {
	int top;
	int right;
	int bottom;
	int left;
}Padding , BorderThickness ;

typedef enum ButtonState {
	CLICKED,
	HOVER,
	NOT_HOVER,
	ACTIVE,
	INACTIVE
}ButtonState, TextEditState;

typedef struct Button {
	Vector2 position;
	char* text;
	Color background_color;
	Color font_color;
	Color borderColor;
	BorderThickness borderThickness;
	int fontSize;
	Color background_color_on_click;
	Color font_color_on_click;
	Color background_color_on_hover;
	Color font_color_on_hover;
	Padding padding;
	ButtonState buttonState;
	Vector2 clicked_animation_move_by;
	float clicked_animation_time;
}Button;

typedef struct TextEdit {
	Vector2 position;
	char* text;
	int max_text;
	char text_length;
	Color background_color;
	Color font_color;
	Color border_color;
	int fontSize;
	Color background_color_on_active;
	Color font_color_on_active;
	Color border_color_on_active;
	Color background_color_on_hover;
	Color font_color_on_hover;
	Color border_color_on_hover;
	Padding padding;
	int width;
	TextEditState textEditState;
	InputType type;
	int activate_with_mouse; 
 } TextEdit;

//----------------    FUNCTIONS     --------------------
//----------------    BUTTON        --------------------
int  isCollidedWithWallsEx(Rectangle rect, RectEx* walls,unsigned int walls_length);
void getInput(char* name, int max_chars ,const char* label, float screenWidth , InputType inputType);
void initButton(Button* button, Vector2 position, char* text);
void drawButton(Button* button);
void drawButtonBorder(Button button);
int isButtonHover(Button button);
int isButtonDown(Button button);
int isButtonPressed(Button button);
int getButtonWidth(Button button);
int getButtonHeight(Button button);
Rectangle getButtonRect(Button button);
void setButtonAtCertainMarginFromBtn(Button* button_to_place, Button from_button, float x);
void setButtonAtCertainMarginYFromBtn(Button* button_to_place, Button from_button, float y);
//----------------    TEXT EDIT      ---------------------
void initTextEdit(TextEdit* textEdit, Vector2 position , const char* buffer , int buffer_size);
int isTextEditHover(TextEdit textEdit);
int isTextEditDown(TextEdit textEdit);
int isTextEditPressed(TextEdit textEdit);
int getTextEditWidth(TextEdit textEdit);
int getTextEditHeight(TextEdit textEdit);
Rectangle getTextEditRect(TextEdit textEdit);
void setTextEditState(TextEdit* textEdit);
void setTextEditFontSize(TextEdit* textEdit, int fontSize);
void setTextEditPadding(TextEdit* textEdit, Padding padding);
void setTextEditTextIfKeyPressed(TextEdit* textEdit);
void drawTextEdit(TextEdit* textEdit);



