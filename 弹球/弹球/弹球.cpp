//头文件
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

//游戏信息
int isGame;
int const height = 30;
int const width = 20;

//球的信息
int ball_x, ball_y;
double ball_speed, ball_move;
int x_way;//1 2 上 下
int y_way;//1 2 左 右


//弹板信息
int border_x;
int border_left;
int border_right;

//要打碎的方块
int block_y[height][5] = { 0 };

double block_move, block_speed;

//得分
int score;

//循环
int i, j;

//展示
int show[height + 1][width + 1] = { 0 };

//计时
clock_t start, end;
clock_t skill;
double duration;



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

void initiation();
void updateWithuotInput();
void updateWithInput();
void Show();
void Move();
void bset();
void bmove();


int main()
{
	initiation();
	while(isGame)
	{
		updateWithuotInput();
		updateWithInput();
	}
	return 0;
}

void initiation()
{
	srand((unsigned)time(NULL));
	HideCursor();
	isGame = 1;

	//球
	ball_x = height - 4;
	ball_y = width / 2;
	x_way = 1;
	y_way = 1;

	//球的移动
	ball_speed = 0.1;
	ball_move = 0;

	//板子
	border_x = height - 3;
	border_left = width / 3;
	border_right = width / 3 * 2;

	//show
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			//边界
			if (((i == 0) || (i == height)) && ((j == 0) || (j == width)))// *
			{
				show[i][j] = -1;
			}
			else if (i == 0)// -
			{
				show[i][j] = -2;
			}
			else if ((j == 0) || (j == width))// |
			{
				show[i][j] = -3;
			}
			//球
			else if (i == ball_x && j == ball_y)
			{
				show[i][j] = -4;
			}
			//板子
			else if (i == border_x && j == border_left)
			{
				for (; j <= border_right; j++)
				{
					show[i][j] = -5;
				}
			}
		}
	}

	//得分
	score = 0;

	//方块
	block_move = 0;
	block_speed = 0.005;
	bset();

	//计时
	start = end = clock();

	duration = -1;
}

void updateWithuotInput()
{
	end = clock();
	
	//每得10分就会使得下边界出现,且球速加快
	if (duration >= 10)
	{
		duration = -1;
		ball_speed = 0.1;
		for (i = 1; i < width; i++)
		{
			show[height][i] = 0;
		}
	}
	else if (duration >= 0)
	{
		duration = (double)(end - skill) / CLK_TCK;
	}
	else if (score % 11 == 10)
	{
		duration = 0.1;
		skill = clock();
		for (i = 1; i < width; i++)
		{
			show[height][i] = -2;
			ball_speed = 0.3;
		}
	}

	
	Show();

	//球的移动
	Move();
	
	//方块的移动
	bmove();
}

void updateWithInput()
{

	char input = 0;
	if (_kbhit())
	{
		input = _getch();//获取输入
		if (input == 'a')
		{
			if (border_left - 1 != 0)
			{
				show[border_x][border_left - 1] = -5;
				show[border_x][border_right] = 0;
				border_left = border_left - 1;
				border_right = border_right - 1;
			} 
		}
		else if (input == 'd')
		{
			if (border_right + 1 != width)
			{
				show[border_x][border_left] = 0;                                                             
				show[border_x][border_right + 1] = -5;
				border_left = border_left + 1;
				border_right = border_right + 1;
			}
		}
	}
}

void Show()
{
	gotoxy(0, 0);
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			if (show[i][j] == -1)//四个角
			{
				printf("*");
			}
			else if (show[i][j] == -2)//上边界
			{
				printf("-");
			}
			else if (show[i][j] == -3)//下边界
			{
				printf("|");
			}
			else if (show[i][j] == -4)//球
			{
				printf("O");
			}
			else if (show[i][j] == -5)//板子
			{
				printf("-");
			}
			else if (show[i][j] == 0)//空白
			{
				printf(" ");
			}
			else if (show[i][j] == 1)//一下碎的方块
			{
				printf("#");
			}
			else if (show[i][j] == 2)//两下碎
			{
				printf("@");
			}
			else if (show[i][j] == 3)//三下碎
			{
				printf("$");
			}
		}
		printf("\n");
	}
	printf("Score:%d\n", score);
	printf("Time:%lf", ((double)(end - start) / CLK_TCK));
}

void Move()
{
	if (ball_move >= 1)
	{
		ball_move = 0;
		show[ball_x][ball_y] = 0;
		if (x_way == 1)
		{
			if (show[ball_x - 1][ball_y] == 0)
			{
				ball_x--;
			}
			else
			{
				if (show[ball_x - 1][ball_y] > 0)
				{
					show[ball_x - 1][ball_y]--;
					if (show[ball_x - 1][ball_y] == 0)
					{
						score++;
					}
				}
				x_way = 2;
				ball_x++;
			}
		}
		else if (x_way == 2)
		{
			if (ball_x + 1 == height && show[height][1] == 0)
			{
				isGame = 0;
			}
			else if (show[ball_x + 1][ball_y] == 0)
			{
				ball_x++;
			}
			else
			{
				if (show[ball_x + 1][ball_y] > 0)
				{
					show[ball_x + 1][ball_y]--;
					if (show[ball_x + 1][ball_y] == 0)
					{
						score++;
					}
				}
				x_way = 1;
				ball_x--;
			}
		}

		if (y_way == 1)
		{
			if (show[ball_x][ball_y - 1] == 0)
			{
				ball_y--;
			}
			else
			{
				if (show[ball_x][ball_y - 1] > 0)
				{
					show[ball_x][ball_y - 1]--;
					if (show[ball_x][ball_y - 1] == 0)
					{
						score++;
					}
				}
				y_way = 2;
				ball_y++;
			}
		}
		else if (y_way == 2)
		{
			if (show[ball_x][ball_y + 1] == 0)
			{
				ball_y++;
			}
			else
			{
				if (show[ball_x][ball_y + 1] > 0)
				{
					show[ball_x][ball_y + 1]--;
					if (show[ball_x][ball_y + 1] == 0)
					{
						score++;
					}
				}
				y_way = 1;
				ball_y--;
			}
		}
		show[ball_x][ball_y] = -4;
	}
	else
	{
		ball_move += ball_speed;
	}

}

void bset()
{
	for (j = 0; j < 5; j++)
	{
		block_y[0][j] = rand() % (width - 1) + 1;
		if (show[1][block_y[0][j]] == 0)
		{
			show[1][block_y[0][j]] = rand() % 2 + 1;
		}
		else
		{
			j--;
		}
	}
}

void bmove()
{
	if (block_move >= 1)
	{
		for (i = 0; i < 5; i++)//判断是否撞到板子
		{
			if (block_y[height - 5][i] != 0)
			{
				isGame = 0;
			}
		}
		block_move = 0;
		for (i = height - 3; i >= 2; i--)
		{
			if (block_y[i - 2][0] != 0)
			{
				for (j = 0; j < 5; j++)
				{
					block_y[i - 1][j] = block_y[i - 2][j];
					show[i][block_y[i - 1][j]] = show[i - 1][block_y[i - 2][j]];
					show[i - 1][block_y[i - 2][j]] = 0;
					block_y[i - 2][j] = 0;
				}
			}
		}
		bset();
	}
	else
	{
		block_move += block_speed;
	}
}
