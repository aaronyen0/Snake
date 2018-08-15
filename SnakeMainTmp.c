//#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "SnakeQueue.h"
#include <time.h>

enum eMapType{Space, Snake, Wall, Candy};
enum eArrow{up, down, left, right, dead};
static eArrow gArrow        = right;
static eArrow gCurrentArrow = right;

const int gHeight = 30;
const int gWidth  = 60;

static int gMap[gHeight][gWidth] = {(int)Space};

DWORD WINAPI ReadKey(void* data);
void Pop(struct Snake* snake);
int Push(eArrow arrow, struct Snake* snake);
void UpdateCandy();

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
			
			for(int j = 1; j < column - 1; ++j){
				printToCoordinates(i, j, ' ');
				gMap[i][j] = (int)Space;
			}
		}
	}
}

struct Snake* IniGmae(){
	int x, y;
	int snakeSize = 5;
	struct Snake* s;
	//設定windows console的buffer
	COORD coord;
	coord.X = gWidth;
	coord.Y = gHeight;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	
	//設定windows console的windows size
	SMALL_RECT windowSize = {0, 0, gWidth, gHeight};
    	BOOL b = SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);	
	
	//建立範圍
	SetWall(gHeight, gWidth, '#');

	x = gHeight / 2;
	y = gWidth / 2;

	gArrow = right;
	gCurrentArrow = right;
	s = CreateSnake(x, y);
	printToCoordinates(x, y, '*');
	for(int i = 0; i < snakeSize -1; ++i){
		Push(right, s);
	}
	srand((unsigned int)time(NULL));
	UpdateCandy();
	//for(int i = 1; i < snakeSize; ++i){
	//	
	//	printToCoordinates(x, y + i, '*');
	//	gMap[x][y - i] = (int)Snake;
	//}
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
	if(gMap[x][y] != (int)Space && gMap[x][y] != (int)Candy){
		return -1;
	}

	PushSnakeBody(x, y,  snake);
	printToCoordinates(x, y, '*');
	gMap[x][y] = (int)Snake;
	return 0;
}

int UpdateSnake(eArrow arrow, struct Snake* snake){
	int x = snake->head->row;
	int y = snake->head->col;
	gCurrentArrow = arrow;
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
	if(gMap[x][y] != (int)Space && gMap[x][y] != (int)Candy){
		return -1;
	}

	if(gMap[x][y] == (int)Space){
		Pop(snake);
		Push(arrow, snake);
	}else if(gMap[x][y] == (int)Candy){
		Push(arrow, snake);
		UpdateCandy();
	}else{
		return -1;
	}
	return 0;
}

void UpdateCandy(){
	int row = gHeight - 1;
	int col = gWidth - 1;

	int x = rand()%((gHeight - 2) * (gWidth - 2));
	int i = 1, j = 0;
	while(x > 0){
		++j;
		if(gMap[i][j] == (int)Space){
			--x;
		}
		if(j == col){
			j = 1;
			++i;
		}
		if(i == row){
			i = 1;
		}
	}
	gMap[i][j] = Candy;
	printToCoordinates(i, j, '$');
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

int main()
{
	int rv;
	//eArrow arrow;

	struct Snake* snake;
	CreateThread(NULL, 0, ReadKey, NULL, 0, NULL);
	
	while(1){
		
		snake = IniGmae();
		while(1){
			Sleep(100);
			//Pop(snake);
			////arrow = NextWay(snake);
			//
			//rv = Push(gArrow, snake);
			rv = UpdateSnake(gArrow, snake);
			if(rv < 0){
				printToCoordinates(gHeight / 2 - 5, gWidth / 2 - 5, ' ');
				printf("Game Over");
				//Sleep(300000);
				break;
			}
		}
		//printToCoordinates(gHeight, gWidth, ' ');
		DisposeSnake(snake);
		Sleep(1000);
		printToCoordinates(gHeight / 2 -4, gWidth / 2, '3');
		Sleep(1000);
		printToCoordinates(gHeight / 2 - 4, gWidth / 2, '2');
		Sleep(1000);
		printToCoordinates(gHeight / 2 - 4, gWidth / 2, '1');
		Sleep(1000);
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
                if(input.Event.KeyEvent.wVirtualKeyCode == VK_UP){
					if(gCurrentArrow != down){
						gArrow = up;
					}
				}
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_DOWN){
                    if(gCurrentArrow != up){
						gArrow = down;
					}
				}
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_LEFT){
                    if(gCurrentArrow != right){
						gArrow = left;
					}
				}
                else if (input.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT){
                    if(gCurrentArrow != left){
						gArrow = right;
					}
				}
            }
        }
		Sleep(1);
    }
	
    return 0;
}
