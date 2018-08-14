#pragma once

struct SnakeBody{
	int row;
	int col;
	struct SnakeBody* next;
};

struct Snake{
	struct SnakeBody* head;
};

struct Snake* CreateSnake(int row, int col);
void DisposeSnake(struct Snake* snake);
void PushSnakeBody(int row, int col, struct Snake* snake);
void PopSnakeBody(struct Snake* snake, int *row, int *col);
