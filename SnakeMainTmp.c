// test20180814.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <varargs.h>
#include <Windows.h>
#include <conio.h>
#include "QueueTool.h"
#include <time.h>

enum eMapType{Space, Snake, Wall};
enum eArrow{up, down, left, right, dead};
static eArrow gArrow = up;

const int gHeight = 35;
const int gWidth  = 60;

static int gMap[gHeight][gWidth] = {(int)Space};

DWORD WINAPI ReadKey(void* data);

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
	int snakeSize = 8;

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

	struct Snake* s = CreateSnake(x, y, snakeSize);
	for(int i = 0; i < snakeSize; ++i){
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

int Push(eArrow arrow, struct Snake* snake){
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
		//printf("dead");
		return -1;
	}
	if(gMap[x][y] != (int)Space){
		return -1;
	}

	PushSnakeBody(x, y,  snake);
	printToCoordinates(x, y, '*');
	gMap[x][y] = (int)Snake;
	return 0;
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
	int rv;
	eArrow arrow;

	struct Snake* snake = IniGmae();
	srand(time(NULL));

	CreateThread(NULL, 0, ReadKey, NULL, 0, NULL);
	while(1){
		Sleep(500);
		Pop(snake);
		//arrow = NextWay(snake);
		
		rv = Push(gArrow, snake);
		if(rv < 0){
			printf("you dead");
			Sleep(1000);
			break;
		}
	}
	return 0;
}


DWORD WINAPI ReadKey(void* data){
	HANDLE handle;
    DWORD cnt;//儲存出入
    DWORD mode;//Console Mode
    INPUT_RECORD input;

    handle = GetStdHandle(STD_INPUT_HANDLE);//先取得基本輸入Handle
    GetConsoleMode(handle, &mode);//得到基本輸入的Mode
    SetConsoleMode(handle, mode & ~ENABLE_LINE_INPUT);//設定基本輸入模式，其中~ENABLE_LINE_INPUT代表不用按下Enter也可動作

    while(ReadConsoleInput(handle, &input, 1, &cnt))//開始讀取使用者的動作
    {
        if(input.EventType == KEY_EVENT)
        {
            if(input.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(input.Event.KeyEvent.wVirtualKeyCode == VK_UP)//大寫鎖定
                    gArrow = up;
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)//Enter按鍵
                    gArrow = down;
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_LEFT)//Ctrl
                    gArrow = left;
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)//F4
                    gArrow = right;
            }
        }
		Sleep(1);
    }

    return 0;
}
