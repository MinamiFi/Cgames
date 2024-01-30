// ball.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//小球弹跳项目


#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void ball(int x, int y);//输出小球目前的位置

int main()
{
	int x, y,x_speed,y_speed;
	int top, bottom, left, right;
	int i, j, p;

	//初始化
	top = left = 0;
	bottom = 20;
	right = 66;
	x = y = x_speed = y_speed = 1;
	
	//运行游戏主体
	while (1)
	{
		//控制竖直方向的边界
		if (x == bottom || x == top)
		{
			printf("\a");
			x_speed = -x_speed;
		}
		x = x + x_speed;//下落或上升
		//控制水平方向的边界
		if (y == left || y == right)
		{
			printf("\a");
			y_speed = -y_speed;
		}
		y = y + y_speed;//下落或上升
		ball(x, y);//输出小球目前的位置
	}
	return 0;
}


void ball(int x, int y)
{
	int i, j;
	system("cls");//清屏函数
	for (i = 0; i < x; i++)
		printf("\n");
	for (j = 0; j < y; j++)
		printf(" ");
	printf("O");
	Sleep(50);
	return;
}