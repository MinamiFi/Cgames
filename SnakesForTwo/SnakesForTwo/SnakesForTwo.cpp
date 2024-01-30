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

int isfood1, isfood2, food_x, food_y;//食物相关信息

int i, j;//循环

int way1,way2;//蛇的运动方向,1 上，2 下，3 左，4 右
double speed1, speed2, move1,move2;//移动
int movecold1, movecold2;//移动节流

int slen1,slen2;//蛇的长度

int head_x1, head_y1;//蛇头的位置
int head_x2, head_y2;//蛇头的位置


void startup();//初始化
void updateWithoutInput();//无输入更新
void updateWithInput();//输入更新
void show();//展示场地
void foodset();//放置食物
void move(double& Move, double& Speed, int& Movecold, int& slen, int& head_x, int& head_y, int& way, int& isfood);//移动函数




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
	isfood1= isfood2 = 0;
	foodset();

	//初始化蛇的移动方向向上
	way1 = 1;
	way2 = 2;

	//初始化蛇的移动
	move1 = move2 = 0;
	speed1 = speed2 = 0.02;
	movecold1 = movecold2 = 1;

	//初始化蛇头的位置
	head_x1 = 2 * height / 3;
	head_y1 = width / 3;
	head_x2 = height / 3;
	head_y2 = width / 2;

	//初始化蛇的长度
	slen1 = 2;
	slen2 = 2;

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
			else if (i == head_x1 && j == head_y1)//蛇头1
			{
				Show[i][j] = 1;
			}
			else if (i == head_x2 && j == head_y2)//蛇头2
			{
				Show[i][j] = 1001;
			}
			else if (i == head_x1 + 1 && j == head_y1)//初始蛇身1
			{
				Show[i][j] = 2;
			}
			else if (i == head_x2 - 1 && j == head_y2)//初始蛇身2
			{
				Show[i][j] = 1002;
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
			else if (Show[i][j] == 1)//蛇头1
			{
				printf("@");
			}
			else if (Show[i][j] == 1001)//蛇头2
			{
				printf("&");
			}
			else if (Show[i][j] >= 2)//蛇身
			{
				printf("*");
			}
			else if (Show[i][j] == -2 && isfood1 * isfood2 == 1)//食物
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
	if (isfood1 * isfood2 == 0)
	{
		//LOOP:
		food_x = rand() % (height - 1) + 1;
		food_y = rand() % (width - 1) + 1;
		if (Show[food_x][food_y] == 0)
		{
			isfood1 = isfood2 = 1;
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
	move(move1, speed1, movecold1, slen1, head_x1, head_y1, way1, isfood1);
	move(move2, speed2, movecold2, slen2, head_x2, head_y2, way2, isfood2);
	
}

//输入更新
void updateWithInput()
{
	char input = 0;


	if (_kbhit())
	{
		input = _getch();//获取输入

		//蛇1移动改变方向
		if (input == 'w' && way1 != 2 && movecold1)
		{
			movecold1 = 0;
			way1 = 1;
		}
		else if (input == 's' && way1 != 1 && movecold1)
		{
			movecold1 = 0;
			way1 = 2;
		}
		else if (input == 'a' && way1 != 4 && movecold1)
		{
			movecold1 = 0;
			way1 = 3;
		}
		else if (input == 'd' && way1 != 3 && movecold1)
		{
			movecold1 = 0;
			way1 = 4;
		}

		//蛇2移动改变方向
		if (input == -32)
		{
			input = _getch();
			if (input == 72 && way2 != 2)//↑
			{
				way2 = 1;
			}
			else if (input == 80 && way2 != 1)//↓
			{
				way2 = 2;
			}
			else if (input == 75 && way2 != 4)//←
			{
				way2 = 3;
			}
			else if (input == 77 && way2 != 3)//→
			{
				way2 = 4;
			}
		}

		//蛇1空格加速
		if (input == ' ')
		{
			speed1 = 0.05;
		}
		else
			speed1 = 0.02;

		//蛇2按0加速
		if (input == '0')
		{
			speed2 = 0.05;
		}
		else
			speed2 = 0.02;
	}
	return;
}


void move(double& Move, double& Speed, int& Movecold, int& slen, int& head_x, int& head_y, int& way,int& isfood)
{
	int judge;
	judge = Show[head_x][head_y] / 1000;
	if (Move <= 1)
	{
		Move += Speed;
	}
	else//移动
	{
		Movecold = 1;
		Move = 0;
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

		if (Show[head_x][head_y] == -2)//吃到食物
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
				if (Show[i][j] == slen + 1000 * judge || (isfood == 0 && Show[i][j] == slen + 1000 * judge - 1))
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
				else if (judge * 1000 < Show[i][j] && Show[i][j] < 1000 + 1000 * judge)
				{
					Show[i][j]++;
				}
			}
		}
		Show[head_x][head_y] = judge * 1000 + 1;
	}
}
