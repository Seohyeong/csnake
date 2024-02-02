#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cassert>
#include <raylib.h>

#define WINDOW_SIZE 800
#define MARGIN 100
#define CELL_SIZE 30
// how many cells in a row (always square)
#define CELL_DIM ((WINDOW_SIZE - 2 * MARGIN) / CELL_SIZE)

#define SH_WHITE {150,150,150,255}
#define SH_BLACK {31,31,31,255}
#define SH_DARK_GREEN {64,148,131,255}
#define SH_GREEN {77,201,176,255}
#define SH_RED {248,82,73,255}

enum struct Direction {
	Up,
	Down,
	Left,
	Right
};

// NOTE(TB): it would be simpler if model coordinates are in cell space instead of screen space
// then have a function that does the conversion, and use it in the drawing functions
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
	SnakeNode* head;
	SnakeNode* tail;
	Direction dir;
};

struct State {
	Snake snake;
	Apple apple;
};


int get_random_value(){
	// 1,2,3,4,5,6,7,8,9,10 -> 60, 120, 180, 240, 300, 360, 420, 480, 540, 600
	return ((rand() % CELL_DIM) + 1) * CELL_SIZE + (MARGIN - CELL_SIZE);
}


// INITIALIZATION
void init_snake(Snake& snake){
	SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
	new_node->x = get_random_value();
	new_node->y = get_random_value();
	snake.dir = Direction::Up; // TODO: randomly init to a class from enum but how?
	new_node->next = nullptr;
	snake.head = new_node;
	snake.tail = new_node;
}


void gen_apple(Apple& apple){
	apple.x = get_random_value();
	apple.y = get_random_value();
}


// SNAKE MOVES
void delete_last_node(Snake& snake){
	SnakeNode* track = snake.head;
		while(track->next->next != nullptr){
			track = track->next;
		}
		track->next = nullptr;
		snake.tail = track;
}


// TODO: if len == 1 then should be able to move any dir
void move_snake(Snake& snake, enum Direction input_dir){
	if(input_dir==Direction::Up){
		// if(snake.dir != Direction::Down && snake.head->y - CELL_SIZE >= MARGIN){
		if(snake.dir != Direction::Down){
			// prepend new_node
			SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
			new_node->x = snake.head->x;
			new_node->y = snake.head->y - CELL_SIZE;
			snake.dir = input_dir;
			new_node->next = snake.head;
			snake.head = new_node;
			// delete last node
			delete_last_node(snake);
		}
	} else if(input_dir==Direction::Down){
		// if(snake.dir != Direction::Up && snake.head->y + CELL_SIZE < WINDOW_SIZE - MARGIN){
		if(snake.dir != Direction::Up){
			// prepend new_node
			SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
			new_node->x = snake.head->x;
			new_node->y = snake.head->y + CELL_SIZE;
			snake.dir = input_dir;
			new_node->next = snake.head;
			snake.head = new_node;
			// delete last node
			delete_last_node(snake);
		}
	} else if(input_dir==Direction::Left){
		// if(snake.dir != Direction::Right && snake.head->x - CELL_SIZE >= MARGIN){
		if(snake.dir != Direction::Right){
			// prepend new_node
			SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
			new_node->x = snake.head->x - CELL_SIZE;
			new_node->y = snake.head->y;
			snake.dir = input_dir;
			new_node->next = snake.head;
			snake.head = new_node;
			// delete last node
			delete_last_node(snake);
		} 
	} else {
		// if(snake.dir != Direction::Left && snake.head->x + CELL_SIZE < WINDOW_SIZE - MARGIN){
		if(snake.dir != Direction::Left){
			// prepend new_node
			SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
			new_node->x = snake.head->x + CELL_SIZE;
			new_node->y = snake.head->y;
			snake.dir = input_dir;
			new_node->next = snake.head;
			snake.head = new_node;
			// delete last node
			delete_last_node(snake);
		}
	}
}


// SNAKE GROW
void get_coordinate(const Snake& snake, int x, int y, int arr[]){
	if(snake.dir == Direction::Up){
		if (y + CELL_SIZE < WINDOW_SIZE - MARGIN) {
			arr[0] = x;
			arr[1] = y + CELL_SIZE;
		}
	} else if(snake.dir == Direction::Down){
		if (y - CELL_SIZE >= MARGIN) {
			arr[0] = x;
			arr[1] = y - CELL_SIZE;
		}
	} else if(snake.dir == Direction::Left){
		if (x + CELL_SIZE < WINDOW_SIZE - MARGIN) {
			arr[0] = x + CELL_SIZE;
			arr[1] = y;
		}
	} else {
		assert(snake.dir == Direction::Right);
		if (x - CELL_SIZE >= MARGIN) {
			arr[0] = x - CELL_SIZE;
			arr[1] = y;
		}
	}
}


void grow_snake(Snake& snake){
	SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));

	int arr[2]; // saves new_x, new_y
	get_coordinate(snake, snake.tail->x, snake.tail->y, arr);
	new_node->x = arr[0];
	new_node->y = arr[1];
	
	snake.tail->next = new_node;
	snake.tail = new_node;
}


void get_apple(Snake& snake, Apple& apple){
	SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
	new_node->x = apple.x;
	new_node->y = apple.y;
	new_node->next = snake.head;
	snake.head = new_node;
}


// RENDERING
void render_grid(){
	int x = MARGIN;
	int y = MARGIN;
	while (x < WINDOW_SIZE - MARGIN) {
		while (y < WINDOW_SIZE - MARGIN) {
			DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, SH_WHITE);
			y += CELL_SIZE;
		}
		y = MARGIN;
		x += CELL_SIZE;
	}
}


void render_apple(const Apple& apple){
	int center_x = apple.x + (CELL_SIZE / 2);
	int center_y = apple.y + (CELL_SIZE / 2);
	DrawCircle(center_x, center_y, CELL_SIZE / 2 / 2, SH_RED);
}


void render_snake(const Snake& snake){
	SnakeNode* track = snake.head;
	DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, SH_DARK_GREEN);
	track = track->next;
	while(track != nullptr){
		DrawRectangle(track->x, track->y, CELL_SIZE, CELL_SIZE, SH_GREEN);
		track = track->next;
	}
}


bool detect_crash(Snake& snake){
	// check if the snake is running into its body
	SnakeNode* track = snake.head->next;
	// nested if loops prevent the program from crasing
	// get_apple() assign the location of the apple as a new head of the snake
	// however, at the exact moment when the snake eats an apple, 
	// head of the snake overlaps with its body
	if(track != nullptr){
		track = track->next;
		if(track != nullptr){
			while(track->next != nullptr){
				if(snake.head->x == track->x && snake.head->y == track->y){
					return true;
				}
				track = track->next;
			}
		}
	}
	// check if the snake is outside the grid
	if((snake.head->x < MARGIN || snake.head->x > WINDOW_SIZE-MARGIN-CELL_SIZE)|| (snake.head->y < MARGIN || snake.head->y > WINDOW_SIZE-MARGIN-CELL_SIZE)){
		return true;
	}
	return false;
}


int main() {
	srand(1111);

	State state{};
	int score = 0;
	bool pause = false;

	InitWindow(800, 800, "snake");
	SetTargetFPS(10);

	gen_apple(state.apple);

	init_snake(state.snake);
	// grow_snake();
	// grow_snake();

	while (!WindowShouldClose()) {

		if(detect_crash(state.snake)){
			pause = true;
		}

		if(!pause){
			move_snake(state.snake, state.snake.dir);

			// NOTE(TB): consider here if multiple keys have been pressed
			if(IsKeyPressed(KEY_UP)){
				move_snake(state.snake, Direction::Up);
			} else if(IsKeyPressed(KEY_DOWN)){
				move_snake(state.snake, Direction::Down);
			} else if(IsKeyPressed(KEY_LEFT)){
				move_snake(state.snake, Direction::Left);
			} else if(IsKeyPressed(KEY_RIGHT)){
				move_snake(state.snake, Direction::Right);
			}

			if(state.snake.head->x == state.apple.x && state.snake.head->y == state.apple.y){
				get_apple(state.snake, state.apple);
				gen_apple(state.apple);
				score ++;
			}
		}

		BeginDrawing();

		{
			ClearBackground(SH_BLACK);

			DrawText(TextFormat("SCORE: %i", score), 310, 35, 40, RAYWHITE);

			render_grid();
			render_apple(state.apple);
			render_snake(state.snake);
			
			if(pause){
				DrawText("PAUSED", 275, 350, 60, GREEN);
			}
		}

		EndDrawing();
	}
	CloseWindow();
	return 0;
}