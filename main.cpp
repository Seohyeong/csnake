#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define WINDOW_SIZE 800
#define MARGIN 100
#define CELL_SIZE 60

enum {
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT
};

struct snake {
	int x;
	int y;
	int dir;
	struct snake * next;
};
typedef struct snake Snake;

Snake* head = NULL;
Snake* tail = NULL;

void init_snake(){
	Snake* new_node = (Snake*)malloc(sizeof(Snake));
	new_node -> x = ((rand() % 10) + 1) * 60 + 100;
	new_node -> y = ((rand() % 10) + 1) * 60 + 100;
	new_node -> dir = SNAKE_UP;
	new_node -> next = NULL;
	head = new_node;
	tail = new_node;
	return;
}

void get_coordinate(int x, int y, int dir, int arr[]){
	if(dir == SNAKE_UP){
		arr[0] = x;
		arr[1] = y - CELL_SIZE;
	} else if(dir == SNAKE_DOWN){
		arr[0] = x;
		arr[1] = y + CELL_SIZE;
	} else if(dir == SNAKE_LEFT){
		arr[0] = x - CELL_SIZE;
		arr[1] = y;
	} else {
		arr[0] = x + CELL_SIZE;
		arr[1] = y;
	}
	return;
}


void move_snake_up(){
	// prepend new_node
	Snake* new_node = (Snake*)malloc(sizeof(Snake));
	new_node -> x = head -> x;
	new_node -> y = head -> y - CELL_SIZE;
	new_node -> dir = head -> dir;
	new_node -> next = head;
	head = new_node;

	// delete last node
	Snake* track = head;
	while((track -> next) -> next != NULL){
		track = track -> next;
	}
	track->next = NULL;
	tail = track;
	return;
}

void move_snake_down(){
	return;
}

void move_snake_left(){
	return;
}

void move_snake_right(){
	return;
}


void grow_snake(){
	Snake* new_node = (Snake*)malloc(sizeof(Snake));

	int arr[2]; // saves new_x, new_y
	get_coordinate(tail -> x, tail -> y, tail -> dir, arr);
	new_node -> x = arr[0];
	new_node -> y = arr[1];
	new_node -> dir = tail -> dir;
	new_node -> next = NULL;

	tail -> next = new_node;
	tail = new_node;

	return;
}

void render_grid(){
	int x = MARGIN;
	int y = MARGIN;
	while (x < WINDOW_SIZE - MARGIN) {
		while (y < WINDOW_SIZE - MARGIN) {
			// DrawRectangleLines(int posX, int posY, int width, int height, Color color);
			DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
			y += CELL_SIZE;
		}
		y = MARGIN;
		x += CELL_SIZE;
	}
	return;
}

void render_apple(int row_idx, int col_idx){
	int x = MARGIN + ((col_idx - 1) * CELL_SIZE);
	int y = MARGIN + ((row_idx - 1) * CELL_SIZE);
	int center_x = x + (CELL_SIZE / 2);
	int center_y = y + (CELL_SIZE / 2);
	// DrawCircle(int centerX, int centerY, float radius, Color color); 
	DrawCircle(center_x, center_y, CELL_SIZE / 2 / 2, RED);
}

void render_snake(){
	Snake* track = head;
	while(track != NULL){
		DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, GREEN);
		track = track->next;
	}
	return;
}

int main() {
	InitWindow(800, 800, "snake");
	SetTargetFPS(60);

	init_snake();
	grow_snake();
	grow_snake();


	while (!WindowShouldClose()) {

        if(IsKeyPressed(KEY_UP)){move_snake_up();}
        if(IsKeyPressed(KEY_DOWN)){move_snake_down();}
        if(IsKeyPressed(KEY_LEFT)){move_snake_left();}
        if(IsKeyPressed(KEY_RIGHT)){move_snake_right();}

		BeginDrawing();

			ClearBackground(RAYWHITE);
			// DrawRectangle(int posX, int posY, int width, int height, Color color);
			render_grid();
			render_apple(2, 3);


			render_snake();

		EndDrawing();

	}

	CloseWindow();
	return 0;
}
