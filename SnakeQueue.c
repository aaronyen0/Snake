#include "SnakeQueue.h"
#include <stdlib.h>

struct SnakeBody* CreateSnakeBody(int row, int col);

struct Snake* CreateSnake(int row, int col){
	struct Snake* snake = (struct Snake*)malloc(sizeof(struct Snake));
	snake->head = CreateSnakeBody(row, col);
	//for(int i = 1; i < len ; ++i){
	//	PushSnakeBody(row, col - i,snake);
	//}

	return snake;
}

void DisposeSnake(struct Snake* snake){
	struct SnakeBody* current, *next;
	next = snake->head;
	while(next){
		current = next;
		if(current->next){
			next = current->next;
		}else{
			next = NULL;
		}
		free(current);
	}
	free(snake);
}

struct SnakeBody* CreateSnakeBody(int row, int col){
	struct SnakeBody* body = (struct SnakeBody*)malloc(sizeof(struct SnakeBody));
	body->next = NULL;
	body->row = row;
	body->col = col;
	return body;
}

void PushSnakeBody(int row, int col, struct Snake* snake){
	struct SnakeBody* tmp = snake->head;
	struct SnakeBody* body =  CreateSnakeBody(row, col);

	snake->head = body;
	body->next = tmp;
}

void PopSnakeBody(struct Snake* snake, int *row, int *col){
	struct SnakeBody* preNode = snake->head;
	struct SnakeBody* curNode = snake->head;
	while(curNode->next){
		preNode = curNode;
		curNode = curNode->next;
	}
	preNode->next = NULL;
	*row = curNode->row;
	*col = curNode->col;
	free(curNode);
}
