#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
//判断周围8个细胞还未完善



//size
const int height = 15;
const int width = 30;

int cell[height][width] = { 0 };//live or not
int i, j, k;//for LOOP



void gotoxy(int x, int y)  //光标移动到(x,y)位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//initialation
void startup()
{
	srand(time(NULL));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			cell[i][j] = rand() % 2;//随机
		}
	}
}

//展现
void show()
{
	gotoxy(0, 0);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (cell[i][j])
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

//非输入更新
void updateWithOutInput()
{
	int change[height][width];
	int count;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			count = 0;//计算周围八个活细胞的数量
			if (i > 0)
			{
				if (j > 0)
				{
					if (cell[i - 1][j - 1])
					{
						count++;
					}
					if (cell[i][j - 1])
					{
						count++;
					}
					if (i + 1 < height) 
					{
						if (cell[i + 1][j - 1])
						{
							count++;
						}
					}
				}
				if (j + 1 < width)
				{
					if (cell[i - 1][j + 1])
					{
						count++;
					}
					if (cell[i][j + 1])
					{
						count++;
					}
					if (i + 1 < height) 
					{
						if (cell[i + 1][j + 1])
						{
							count++;
						}
					}
				}
				if (cell[i - 1][j])
				{
					count++;
				}
				if (i + 1 < height)
				{
					if (cell[i + 1][j])
					{
						count++;
					}
				}
			}
			

			//判断下个阶段的状态
			if (count == 3)
			{
				change[i][j] = 1;
			}
			else if (count == 2)
			{
				change[i][j] = cell[i][j];
			}
			else
			{
				change[i][j] = 0;
			}

		}
	}
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			cell[i][j] = change[i][j];
		}
	}
}

//输入更新
void updateWithInput()
{
	
}


int main()
{
	startup();
	while (1)
	{
		show();
		updateWithOutInput();
		updateWithInput();
		system("pause");
	}
	return 0;
}