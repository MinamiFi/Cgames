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



int isGame = 1;//判断游戏开始
//边界
const int width = 100;
const int height = 40;
//场地
int show[height + 1][width + 1] = { 0 };

//场上对象的信息
struct object {
	int x = 0, y = 0;
	int way = 1;//1234上下左右
	double speed = 0.05, move = 0;
};
//玩家与敌人
object player;
object enemy[150] = {};

//难度
int hardlevel;
//循环信息
int i, j;

//经过时间
clock_t start, end;
double duration;

void initial();
void updateWithoutInput();
void updateWithInput();
void Show();
void enemysetup(int& i);//生成敌人
void Move(object& obj);//玩家移动
void eMove(object& enemy, const object& player);//敌人移动


int main() {
	initial();
	while(isGame)
	{
		updateWithoutInput();
		updateWithInput();
	}
	system("cls");
	if (duration >= 100)
	{
		printf("太牛啦");
	}
	else if (duration >= 80)
	{
		printf("有点可惜了，加油吧");
	}
	else if (duration >= 60)
	{
		printf("就差一点了");
	}
	else if (duration >= 40)
	{
		printf("还不错");
	}
	else if (duration >= 20)
	{
		printf("多加练习还是能成的");
	}
	else
	{
		printf("你已经超过了0.01%%的选手");
	}
	return 0;
}


void initial()
{
	srand((unsigned)time(NULL));

	start = clock();
	end = start;
	duration = 0;

	//玩家信息
	player.x = height / 2;
	player.y = width / 2;
	player.speed = 0.3;
	
	//难度
	hardlevel = 1;

	//场地信息
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			//边界
			if (((i == 0) || (i == height)) && ((j == 0) || (j == width)))
			{
				show[i][j] = -1;
			}
			else if ((i == 0) || (i == height))
			{
				show[i][j] = -2;
			}
			else if ((j == 0) || (j == width))
			{
				show[i][j] = -3;
			}
			//玩家位置
			else if (i == player.x && j == player.y)
			{
				show[i][j] = -4;
			}
			else
			{
				show[i][j] = 0;
				printf(" ");
			}
		}
		printf("\n");
	}
	
	//敌人生成
	for (i = 0; i < 30; i++)
	{
		enemysetup(i);
	}


	return;
}

void updateWithoutInput()
{
	Show();
	
	//经过时间
	end = clock();
	duration = (double)(end - start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位，即时钟打点

	if (duration >= 20 * hardlevel && hardlevel != 5)
	{
		hardlevel++;
		for (i = 30 * (hardlevel - 1); i < 30 * hardlevel; i++)
		{
			enemysetup(i);
		}
	}
	else if (duration >= 100)
	{
		isGame=0;
	}

	//玩家的移动
	Move(player);

	//敌人移动
	for (i = 0; i < hardlevel * 30; i++)
	{
		eMove(enemy[i], player);
	}

	if (show[player.x][player.y] == 0 || show[player.x][player.y] == -4)
	{
		show[player.x][player.y] = -4;
	}
	else
	{
		isGame = 0;
	}



}

void updateWithInput()
{
	char input = 0;


	if (_kbhit())
	{
		input = _getch();//获取输入
		if (input == 'w')
		{
			player.way = 1;
		}
		else if (input == 's')
		{
			player.way = 2;
		}
		if (input == 'a')
		{
			player.way = 3;
		}
		else if (input == 'd')
		{
			player.way = 4;
		}
	}
}

void Show()
{
	gotoxy(0, 0);
	HideCursor();
	for (i = 0; i <= height; i++)
	{
		for (j = 0; j <= width; j++)
		{
			if (show[i][j] == -4)//玩家
			{
				printf("@");
			}
			else if (show[i][j] == -1)//四个角
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
			else if (show[i][j] == 0)//空白
			{
				printf(" ");
			}
			else if (show[i][j] == 1)//敌人
			{
				printf("#");
			}
		}
		printf("\n");
	}
	printf("已经过了%lf秒", duration);
}

void enemysetup(int& i)
{
	if (i % 4 == 0)
	{
		enemy[i].x = rand() % (height - 1) + 1;
		enemy[i].y = 1;
	}
	else if (i % 4 == 1)
	{
		enemy[i].x = rand() % (height - 1) + 1;
		enemy[i].y = width - 1;
	}
	else if (i % 4 == 2)
	{
		enemy[i].x = 1;
		enemy[i].y = rand() % (width - 1) + 1;
	}
	else if (i % 4 == 3)
	{
		enemy[i].x = height - 1;
		enemy[i].y = rand() % (width - 1) + 1;
	}

	if (show[enemy[i].x][enemy[i].y] == 0)
	{
		show[enemy[i].x][enemy[i].y] = 1;
	}
	else
	{
		i--;
	}

}

void Move(object& obj)
{
	if (obj.move >= 1)
	{
		obj.move = 0;
		if (obj.way == 1)
		{
			show[obj.x][obj.y] = 0;
			obj.x--;
		}
		else if (obj.way == 2)
		{
			show[obj.x][obj.y] = 0;
			obj.x++;
		}
		else if (obj.way == 3)
		{
			show[obj.x][obj.y] = 0;
			obj.y--;
		}
		else if (obj.way == 4)
		{
			show[obj.x][obj.y] = 0;
			obj.y++;
		}
	}
	else
	{
		obj.move += obj.speed;
	}
}

void eMove(object& enemy, const object& player)
{
	if (enemy.move >= 1)
	{
		enemy.move = 0;
		if (player.x > enemy.x)
		{
			if (show[enemy.x + 1][enemy.y] == 0)
			{
				show[enemy.x][enemy.y] = 0;
				enemy.x++;
			}
		}
		else if (player.x < enemy.x)
		{
			if (show[enemy.x - 1][enemy.y] == 0)
			{
				show[enemy.x][enemy.y] = 0;
				enemy.x--;
			}
		}

		if (player.y > enemy.y)
		{
			if (show[enemy.x][enemy.y+1] == 0)
			{
				show[enemy.x][enemy.y] = 0;
				enemy.y++;
			}
		}
		else if (player.y < enemy.y)
		{
			if (show[enemy.x][enemy.y - 1] == 0)
			{
				show[enemy.x][enemy.y] = 0;
				enemy.y--;
			}
		}

		show[enemy.x][enemy.y] = 1;
	}
	else
	{
		enemy.move += enemy.speed;
	}
}
