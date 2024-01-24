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

struct SnakeNode {
	int x;
	int y;
	SnakeNode* next;
};

struct Snake {
	SnakeNode* head = NULL;
	SnakeNode* tail = NULL;
	Direction dir = Direction::Up;
};

Snake snake;
Apple apple;

void init_snake(){
	SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
	new_node->x = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
	new_node->y = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
	snake.dir = Direction::Up;
	new_node->next = NULL;
	snake.head = new_node;
	snake.tail = new_node;
}


void gen_apple(){
	apple.x = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE); // 1,2,3,4,5,6,7,8,9,10 -> 60, 120, 180, 240, 300, 360, 420, 480, 540, 600
	apple.y = ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
}


void move_snake_up(){
	if(snake.dir != Direction::Down && snake.head->y - CELL_SIZE >= MARGIN){
		// prepend new_node
		SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
		new_node->x = snake.head->x;
		new_node->y = snake.head->y - CELL_SIZE;
        snake.dir = Direction::Up;
		new_node->next = snake.head;
		snake.head = new_node;

		// delete last node
		SnakeNode* track = snake.head;
		while(track->next->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		snake.tail = track;
	}
}

void move_snake_down(){
	if(snake.dir != Direction::Up && snake.head->y + CELL_SIZE < WINDOW_SIZE - MARGIN){
		// prepend new_node
		SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
		new_node->x = snake.head->x;
		new_node->y = snake.head->y + CELL_SIZE;
		snake.dir = Direction::Down;
		new_node->next = snake.head;
		snake.head = new_node;

		// delete last node
		SnakeNode* track = snake.head;
		while(track->next->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		snake.tail = track;
	}
}

void move_snake_left(){
	if(snake.dir != Direction::Right && snake.head->x - CELL_SIZE >= MARGIN){
		// prepend new_node
		SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
		new_node->x = snake.head->x - CELL_SIZE;
		new_node->y = snake.head->y;
		snake.dir = Direction::Left;
		new_node->next = snake.head;
		snake.head = new_node;

		// delete last node
		SnakeNode* track = snake.head;
		while(track->next->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		snake.tail = track;
	}
}

void move_snake_right(){
	if(snake.dir != Direction::Left && snake.head->x + CELL_SIZE < WINDOW_SIZE - MARGIN){
		// prepend new_node
		SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
		new_node->x = snake.head->x + CELL_SIZE;
		new_node->y = snake.head->y;
		snake.dir = Direction::Right;
		new_node->next = snake.head;
		snake.head = new_node;

		// delete last node
		SnakeNode* track = snake.head;
		while(track->next->next != NULL){
			track = track->next;
		}
		track->next = NULL;
		snake.tail = track;
	}
}


void get_coordinate(int x, int y, int arr[]){
	if(snake.dir == Direction::Up && y + CELL_SIZE < WINDOW_SIZE - MARGIN){
		arr[0] = x;
		arr[1] = y + CELL_SIZE;
	} else if(snake.dir == Direction::Down && y - CELL_SIZE >= MARGIN){
		arr[0] = x;
		arr[1] = y - CELL_SIZE;
	} else if(snake.dir == Direction::Left && x + CELL_SIZE < WINDOW_SIZE - MARGIN){
		arr[0] = x + CELL_SIZE;
		arr[1] = y;
	} else if(snake.dir == Direction::Right && x - CELL_SIZE >= MARGIN){
		arr[0] = x - CELL_SIZE;
		arr[1] = y;
	}
}


void grow_snake(){
	SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));

	int arr[2]; // saves new_x, new_y
	get_coordinate(snake.tail->x, snake.tail->y, arr);
	new_node->x = arr[0];
	new_node->y = arr[1];
	
	snake.tail->next = new_node;
	snake.tail = new_node;
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
}


void render_apple(){
	int center_x = apple.x + (CELL_SIZE / 2);
	int center_y = apple.y + (CELL_SIZE / 2);
	DrawCircle(center_x, center_y, CELL_SIZE / 2 / 2, RED);
}


void render_snake(){
	SnakeNode* track = snake.head;
	DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, DARKGREEN);
	track = track->next;
	while(track != NULL){
		DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, GREEN);
		track = track->next;
	}
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

		if(snake.head->x == apple.x && snake.head->y == apple.y){
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
