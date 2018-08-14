#include <Windows.h>
#include <conio.h>
#include "QueueTool.h"
#include <time.h>

enum eMapType{Space, Snake, Wall};
enum eArrow{up, down, left, right, dead};
static eArrow arrow = up;

const int gHeight = 35;
const int gWidth  = 60;

static int gMap[gHeight][gWidth] = {(int)Space};

void gotoxy(int row, int column)
{
  COORD coord;
  coord.X = column;
  coord.Y = row;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printToCoordinates(int x, int y, char c){
	gotoxy(x, y);
	printf("%c", c);
}

void SetWall(int row, int column, char c){
	for(int i = 0; i < row; ++i){
		if(i == 0 || i == row -1){
			for(int j = 0; j < column; ++j){
				printToCoordinates(i, j, c);
				gMap[i][j] = (int)Wall;
			}
		}else{
			printToCoordinates(i, 0, c);
			gMap[i][0] = (int)Wall;
			printToCoordinates(i, column - 1, c);
			gMap[i][column - 1] = (int)Wall;
		}
	}
}

struct Snake* IniGmae(){
	int x, y;
	
	//設定windows console的buffer
	COORD coord;
	coord.X = gWidth;
	coord.Y = gHeight;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	
	//設定windows console的windows size
	SMALL_RECT windowSize = {0, 0, gWidth, gHeight};
    BOOL b = SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);	
	
	//建立範圍
	SetWall(gHeight, gWidth, '%');

	x = gHeight / 2;
	y = gWidth / 2;

	struct Snake* s = CreateSnake(x, y, 8);
	for(int i = 0; i < 4; ++i){
		printToCoordinates(x, y - i, '*');
		gMap[x][y - i] = (int)Snake;
	}
	return s;
}

void Pop(struct Snake* snake){
	int x, y;
	PopSnakeBody(snake, &x, &y);
	printToCoordinates(x, y, ' ');
	gMap[x][y] = (int)Space;
}

void Push(eArrow arrow, struct Snake* snake){
	int x = snake->head->row;
	int y = snake->head->col;
	if(arrow == up){
		x--;
	}else if(arrow == down){
		x++;
	}else if(arrow == left){
		y--;
	}else if(arrow == right){
		y++;
	}else{
		printf("dead");
		return;
	}
	PushSnakeBody(x, y,  snake);
	printToCoordinates(x, y, '*');
	gMap[x][y] = (int)Snake;
}

eArrow NextWay(struct Snake* snake){
	int headX = snake->head->row;
	int headY = snake->head->col;

	int i = 0;
	int way = rand()%4;
	while(i++ < 4){
		if(way == 0 && gMap[headX - 1][headY] == Space){
			return up;
		}else if(way == 1 && gMap[headX + 1][headY] == Space){
			return down;
		}else if(way == 2 && gMap[headX][headY - 1] == Space){
			return left;
		}else if(way == 3 && gMap[headX][headY + 1] == Space){
			return right;
		}
		way = (way + 1) % 4;
	}
	return dead;
}

int _tmain(int argc, _TCHAR* argv[])
{
	eArrow arrow;
	struct Snake* snake = IniGmae();
	srand(time(NULL));

	while(1){
		Sleep(50);
		Pop(snake);
		arrow = NextWay(snake);
		if(arrow == dead){
			printf("dead");
			break;
		}
		Push(arrow, snake);
	}
	return 0;
}

