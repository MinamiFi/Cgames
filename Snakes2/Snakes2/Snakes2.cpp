//头文件
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>


void HideCursor()//隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };//第二个值0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y)//类似于清屏函数
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}


//变量
const int height = 20;
const int width = 30;

int isGame;//判断游戏是否继续

int Show[height + 1][width + 1] = { 0 };//用来判断场地上该有什么东西-1 边界，-2 食物，0 空白，1 蛇头，>=2 蛇身

int isfood, food_x, food_y;//食物相关信息

int i, j;//循环

int way;//蛇的运动方向,1 上，2 下，3 左，4 右
double speed, move;//移动
int movecold;//移动节流

int slen;//蛇的长度

int head_x, head_y;//蛇头的位置


void startup();//初始化
void updateWithoutInput();//无输入更新
void updateWithInput();//输入更新
void show();//展示场地
void foodset();//放置食物




int main()
{
	startup();
	while (isGame == 1)
	{
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
}

//初始化函数
void startup()
{
	srand(time(NULL));
	//开始游戏
	isGame = 1;

	//初始化食物信息
	isfood = 0;
	foodset();

	//初始化蛇的移动方向向上
	way = 1;

	//初始化蛇的移动
	move = 0;
	speed = 0.02;
	movecold = 1;

	//初始化蛇头的位置
	head_x = height / 2;
	head_y = width / 2;

	//初始化蛇的长度
	slen = 2;

	//初始化场地信息
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			if (i == 0 || i == height || j == 0 || j == width)//边界
			{
				Show[i][j] = -1;
			}
			else if (i == food_x && j == food_y)//食物
			{
				Show[i][j] = -2;
			}
			else if (i == height / 2 && j == width / 2)//蛇头
			{
				Show[i][j] = 1;
			}
			else if (i == height / 2 + 1 && j == width / 2)//初始蛇身
			{
				Show[i][j] = 2;
			}
		}
	}
	return;
}

//展示场地
void show()
{
	HideCursor();
	gotoxy(0, 0);
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			if (Show[i][j] == -1)//边界
			{
				printf("#");
			}
			else if (Show[i][j] == 1)//蛇头
			{
				printf("@");
			}
			else if (Show[i][j] >= 2)//蛇身
			{
				printf("*");
			}
			else if (Show[i][j] == -2 && isfood == 1)//食物
			{
				printf("$");
			}
			else
			{
				printf(" ");
			}

		}
		printf("\n");
	}
}

//生成食物函数
void foodset()
{
	if (isfood == 0)
	{
	//LOOP:
		food_x = rand() % (height - 1) + 1;
		food_y = rand() % (width - 1) + 1;
		if (Show[food_x][food_y] == 0)
		{
			isfood = 1;
			Show[food_x][food_y] = -2;
		}
		else
		{
			foodset();
			//goto LOOP;
		}
	}

}

//无输入更新
void updateWithoutInput()
{
	foodset();

	if (move <= 1)
	{
		move += speed;
	}
	else//移动
	{
		movecold = 1;
		move = 0;
		if (way == 1)//上
		{
			head_x--;
		}
		else if (way == 2)//下
		{
			head_x++;
		}
		else if (way == 3)//左
		{
			head_y--;
		}
		else//右
		{
			head_y++;
		}

		if (Show[head_x][head_y] == -2)//食物
		{
			isfood = 0;
			slen++;
		}
		else if (Show[head_x][head_y] != 0)
		{
			isGame = 0;
			return;
		}
		for (i = 1; i < height; i++)
		{
			for (j = 1; j < width; j++)
			{
				if (Show[i][j] == slen || (isfood == 0 && Show[i][j] == slen - 1))
				{
					if (isfood)
					{
						Show[i][j] = 0;
					}
					else
					{
						Show[i][j]++;
					}
				}
				else if (Show[i][j] > 0)
				{
					Show[i][j]++;
				}
			}
		}
		Show[head_x][head_y] = 1;
	}
}

//输入更新
void updateWithInput()
{
	char input;
	if (_kbhit())
	{
		input = _getch();//获取输入
		
		//控制方向
		if (input == 'w' && way != 2 && movecold)
		{
			movecold = 0;
			way = 1;
		}
		else if (input == 's' && way != 1 && movecold)
		{
			movecold = 0;
			way = 2;
		}
		else if (input == 'a' && way != 4 && movecold)
		{
			movecold = 0;
			way = 3;
		}
		else if (input == 'd' && way != 3 && movecold)
		{
			movecold = 0;
			way = 4;
		}

		//空格加速
		if (input == ' ')
		{
			speed = 0.05;
		}
		else
			speed = 0.02;
	}
	return;
}