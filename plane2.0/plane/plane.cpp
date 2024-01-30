// plane.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//飞机小游戏项目


#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include<conio.h>
#include <time.h>

void startup();
void show();
void updateWithoutInput();
void updateWithInput();

int height,width;//边界
int position_x, position_y;//飞机位置
int isbullet,bullet_x,bullet_y;//子弹
int istarget = 0, target_x = 1, target_y;//靶子
double delt_target_x = 0, target_speed = 0.005;
int isTool = 0, tool_x = 1, tool_y;
double delt_tool_x = 0;
int score = 0, chance = 10;//得分,血量
int level = 1;


void HideCursor()//隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };//第二个值0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y)//类似于清屏函数，光标移动到原点位置进行重画
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}




void main()
{
	srand(time(NULL));
	startup();  // 数据初始化	
	HideCursor();
	while (chance != 0) //  游戏循环执行
	{
		show();  // 显示画面
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();  // 与用户输入有关的更新
	}
	//结束清屏
	system("cls");
	printf("Your have no more chances.\n");
	printf("Your score is: %d!", score);
}


void startup()
{
	height = 25;
	width = 20;
	position_x = height / 2;
	position_y = width / 2;
}
void show()
{
	gotoxy(0, 0);
	int i, j;
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			//边框
			if (j == 0 || j == width)
			{
				printf("|");
			}
			else if (i == 0 || i == height)
			{
				printf("-");
			}
			//飞机
			else if (i == position_x && j == position_y)
			{
				printf("*");
				//是否拾取道具
				if (i == (int)(delt_tool_x + tool_x) && j == tool_y)
				{
					isTool = 2;
				}
				//撞到目标
				if (i == (int)(target_x + delt_target_x) && j == target_y)
				{
					istarget = 3;
					chance--;
				}
			}
			//子弹
			else if (level == 1 && (i == bullet_x && j == bullet_y))
			{
				printf("|");
				//命中目标
				if ((i == (int)(target_x + delt_target_x) && j == target_y) || ((i == ((int)(target_x + delt_target_x) + 1) && j == target_y)))
				{
					istarget = 2;
				}
			}
			else if (level == 2 && (i == bullet_x && (j == bullet_y - 1 || j == bullet_y + 1 || j == bullet_y)))
			{
				printf("|");
				//命中目标
				if ((i == (int)(target_x + delt_target_x) && j == target_y) || ((i == ((int)(target_x + delt_target_x) + 1) && j == target_y)))
				{
					istarget = 2;
				}
			}
			else if (level == 3 && ((i == bullet_x || (i == bullet_x - 1)) && (j == bullet_y - 1 || j == bullet_y + 1 || j == bullet_y)))
			{
				printf("|");
				//命中目标
				if ((i == (int)(target_x + delt_target_x) && j == target_y) || ((i == ((int)(target_x + delt_target_x) + 1) && j == target_y)))
				{
					istarget = 2;
				}
			}
			//目标
			else if (i == (int)(target_x + delt_target_x) && j == target_y)
			{
				printf("#");
			}
			//道具
			else if (i == (int)(delt_tool_x + tool_x) && j == tool_y)
			{
				printf("O");
			}
			//空白
			else 
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("score: %d   chance: %d  \n", score, chance);
	printf("Level: %d   \n", level);
}
void updateWithoutInput()
{
	//istarget共三个状态:0,1,2,3
	if (istarget == 0)//0表示目标消失，重新生成目标
	{
		target_y = rand() % (width - 1) + 1;
		istarget = 1;
	}
	//1表示目标已生成，但是未被击落
	else if (istarget == 2)//2表示目标被击落，加得分、重新初始化目标信息
	{
		delt_target_x = 0;
		score++;
		istarget = 0;
	}
	else if (istarget == 3)//3表示目标与本机相撞，重新初始化信息
	{
		delt_target_x = 0;
		istarget = 0;
	}

	//isTool公有三个状态:0,1,2,3
	if (isTool == 0)//0表示道具消失，重新生成道具
	{
		tool_y = rand() % (width - 1) + 1;
		isTool = 1;
	}//1表示道具已生成，但没有被拾取
	else if (isTool == 2)//2表示道具已被拾取，飞机升级，重新初始化道具信息
	{
		if(level<3)//满级为3级
		{
			level++;
			chance += 1;//升级之后+3血
		}
		else
		{
			chance += 3;//满级之后+5血
		}

		delt_tool_x = 0;
		isTool = 0;
	}

	//子弹向上移动
	if (bullet_x >= 0)
	{
		bullet_x--;
	}

	//目标向下移动
	if (delt_target_x + target_x < height)
	{
		delt_target_x = delt_target_x + target_speed;
	}
	else//目标超出下界，重新生成
	{
		chance--;//机会减少
		delt_target_x = 0;
		istarget = 0;
	}


	//逐渐加快目标下落的速度
	if (target_speed < 0.1)
	{
		target_speed += 0.00001;
	}

	//道具向下移动 
	if ((delt_tool_x + tool_x < height) && (isTool == 1))
	{
		delt_tool_x = delt_tool_x + 0.025;
	}
	else if (isTool == 1)//道具超出下界
	{
		delt_tool_x = 0;
		isTool = 0;
	}

}
void updateWithInput()
{
	char input;
	if (_kbhit())
	{
		input = _getch();
		//实现飞机的移动
		if (input == 'w' || input == 'W')
		{
			if (position_x - 2 >= 0)
			{
				position_x--;
			}
		}
		else if (input == 's' || input == 'S')
		{
			if (position_x + 1 < height)
			{
				position_x++;
			}
		}
		else if (input == 'a' || input == 'A')
		{
			if (position_y - 2 >= 0)
			{
				position_y--;
			}
		}
		else if (input == 'd' || input == 'D')
		{
			if (position_y + 1 < width)
			{
				position_y++;
			}
		}
		//是否射出子弹
		else if (input == ' ')
		{

			bullet_x = position_x - 1;
			bullet_y = position_y;
		}
	}
}
