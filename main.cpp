#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define WINDOW_SIZE 800
#define MARGIN 100
#define CELL_SIZE 30
// how many cells in a row (always square)
#define CELL_DIM ((WINDOW_SIZE - 2 * MARGIN) / CELL_SIZE)

enum struct Direction {
	Up,
	Down,
	Left,
	Right
};

struct Apple {
	int x;
	int y;
};

struct Snake {
	int x;
	int y;
	Snake* next;
};


Snake* head = NULL;
Snake* tail = NULL;
Direction dir = Direction::Up;
Apple apple;

void init_snake(){
	Snake* new_node = (Snake*)malloc(sizeof(Snake));
	new_node->x = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
	new_node->y = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
	dir = Direction::Up;
	new_node->next = NULL;
	head = new_node;
	tail = new_node;
	return;
}


void gen_apple(){
	apple.x = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE); // 1,2,3,4,5,6,7,8,9,10 -> 60, 120, 180, 240, 300, 360, 420, 480, 540, 600
	apple.y = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
	return;
}


void move_snake_up(){
	if(dir != Direction::Down & (head->y) - CELL_SIZE >= MARGIN){
		// prepend new_node
		Snake* new_node = (Snake*)malloc(sizeof(Snake));
		new_node->x = head->x;
		new_node->y = head->y - CELL_SIZE;
        dir = Direction::Up;
		new_node->next = head;
		head = new_node;

		// delete last node
		Snake* track = head;
		while((track->next)->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		tail = track;
	}
	return;
}

void move_snake_down(){
	if(dir != Direction::Up & (head->y) + CELL_SIZE < WINDOW_SIZE - MARGIN){
		// prepend new_node
		Snake* new_node = (Snake*)malloc(sizeof(Snake));
		new_node->x = head->x;
		new_node->y = head->y + CELL_SIZE;
		dir = Direction::Down;
		new_node->next = head;
		head = new_node;

		// delete last node
		Snake* track = head;
		while((track->next)->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		tail = track;
		return;
	}
}

void move_snake_left(){
	if(dir != Direction::Right & (head->x) - CELL_SIZE >= MARGIN){
		// prepend new_node
		Snake* new_node = (Snake*)malloc(sizeof(Snake));
		new_node->x = head->x - CELL_SIZE;
		new_node->y = head->y;
		dir = Direction::Left;
		new_node->next = head;
		head = new_node;

		// delete last node
		Snake* track = head;
		while((track->next)->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		tail = track;
		return;
	}
}

void move_snake_right(){
	if(dir != Direction::Left & (head->x) + CELL_SIZE < WINDOW_SIZE - MARGIN){
		// prepend new_node
		Snake* new_node = (Snake*)malloc(sizeof(Snake));
		new_node->x = head->x + CELL_SIZE;
		new_node->y = head->y;
		dir = Direction::Right;
		new_node->next = head;
		head = new_node;

		// delete last node
		Snake* track = head;
		while((track->next)->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		tail = track;
	}
	return;
}


void get_coordinate(int x, int y, int arr[]){
	if(dir == Direction::Up & y + CELL_SIZE < WINDOW_SIZE - MARGIN){
		arr[0] = x;
		arr[1] = y + CELL_SIZE;
	} else if(dir == Direction::Down & y - CELL_SIZE >= MARGIN){
		arr[0] = x;
		arr[1] = y - CELL_SIZE;
	} else if(dir == Direction::Left & x + CELL_SIZE < WINDOW_SIZE - MARGIN){
		arr[0] = x + CELL_SIZE;
		arr[1] = y;
	} else if(dir == Direction::Right & x - CELL_SIZE >= MARGIN){
		arr[0] = x - CELL_SIZE;
		arr[1] = y;
	}
	return;
}


void grow_snake(){
	Snake* new_node = (Snake*)malloc(sizeof(Snake));

	int arr[2]; // saves new_x, new_y
	get_coordinate(tail->x, tail->y, arr);
	new_node->x = arr[0];
	new_node->y = arr[1];
	
	tail->next = new_node;
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


void render_apple(){
	int center_x = apple.x + (CELL_SIZE / 2);
	int center_y = apple.y + (CELL_SIZE / 2);
	DrawCircle(center_x, center_y, CELL_SIZE / 2 / 2, RED);
	return;
}


void render_snake(){
	Snake* track = head;
	DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, DARKGREEN);
	track = track->next;
	while(track != NULL){
		DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, GREEN);
		track = track->next;
	}
	return;
}


int main() {
	srand(1111);
	InitWindow(800, 800, "snake");
	SetTargetFPS(60);

	gen_apple();

	init_snake();
	// grow_snake();
	// grow_snake();

	while (!WindowShouldClose()) {

        if(IsKeyPressed(KEY_UP)){move_snake_up();}
        if(IsKeyPressed(KEY_DOWN)){move_snake_down();}
        if(IsKeyPressed(KEY_LEFT)){move_snake_left();}
        if(IsKeyPressed(KEY_RIGHT)){move_snake_right();}

		if(head->x == apple.x & head->y == apple.y){
			grow_snake();
			gen_apple();
		}

		BeginDrawing();

			ClearBackground(RAYWHITE);

			render_grid();
			render_apple();
			render_snake();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
