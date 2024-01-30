// plane.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//飞机小游戏项目


#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include<conio.h>
#include <time.h>

void plane(int x, int y);//输出飞机目前的位置
void input(int &x, int &y);//获取输入
void bulletplane(int& x, int &y);//发射子弹
void target();//用于放置靶子
void buildtarget();//用于生成靶子

//坐标
int size_x = 30;
int size_y = 100;
//是否击中
bool t = 0; 
//靶子位置
int t_position;
//得分
int score = 0;
//子弹数量
int bullet = 10;


int main()
{
	//定义坐标变量
	int x, y;

	//初始化数据
	x = 10;
	y = 10;
	srand((unsigned)time(NULL));

	//初始化场景
	buildtarget();
	plane(x, y);

	//运行游戏主体
	while (1)
	{
		if (t)
		{
			buildtarget();
			t = !t;
		}
		input(x, y);//获得输入
	}
	return 0;
}


void plane(int x, int y)
{
	system("cls");//清屏函数
	printf("Your score is : %d.\n", score);
	printf("Your have %d bullet(s).\n", bullet);
	target();
	int i, j;
	for (i = 0; i < x-3; i++)
		printf("\n");
	for (j = 0; j < y; j++)
	{
		printf(" ");
	}
	printf("*\n");
	for (j = 0; j < y - 2; j++)
	{
		printf(" ");
	}
	printf("*****");
	printf("\n");
	for (j = 0; j < y - 1; j++)
	{
		printf(" ");
	}
	printf("* *");
	printf("\n");
	return;
}
void bulletplane(int& x, int& y)
{
	bullet--;
	system("cls");//清屏函数
	if (y != t_position)
	{
		printf("Your score is : %d\n", score);
		printf("Your have %d bullet(s).\n", bullet);
		target();
	}
	else
	{
		t = TRUE;
		x = x + 1;
		score++;
		printf("Your score is : %d\n", score);
		printf("Your have %d bullet(s).\n", bullet);
	}
	int i, j;
	for (i = 0; i < x - 3; i++)
	{
		for (j = 0; j < y; j++)
		{
			printf(" ");
		}
		printf("|\n");
	}
	for (j = 0; j < y; j++)
	{
		printf(" ");
	}
	printf("*\n");
	for (j = 0; j < y - 2; j++)
	{
		printf(" ");
	}
	printf("*****");
	printf("\n");
	for (j = 0; j < y - 1; j++)
	{
		printf(" ");
	}
	printf("* *");
	printf("\n");
	if (t)
	{
		x--;
	}
	return;
}
void input(int &x, int &y)
{
	char input;
	input = _getch();//获得键盘输入
	if (input == ' ')
	{
		if (bullet > 0)
			bulletplane(x, y);
	}
	else
	{
		if (input == 'w')
		{
			if (x >= 1)
				x--;
		}
		else if (input == 's')
		{
			if (x <= size_x)
				x++;
		}
		else if (input == 'a')
		{
			if (y > 2)
				y--;
		}
		else if (input == 'd')
		{
			if (y <= size_y)
				y++;
		}
		plane(x, y);
	}
	return;
}
void buildtarget()
{
	t_position = rand() % 26 + 2;
	return;
}
void target()
{
	int i;
	for (i = 0; i < t_position; i++)
		printf(" ");
	printf("+\n");
	return;
}