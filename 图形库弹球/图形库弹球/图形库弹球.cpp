//头文件
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>//清屏函数等
#include <time.h>//clock函数
#include <conio.h>//输入
#include <graphics.h>//图形库


//游戏界面
const int width = 420;
const int height = 600;
int isGame;
int backgroundcolor;

//球 x y 速度 颜色 xy的方向 半径 移动函数
struct BALL
{
	int x;
	int y;
	int step;
	int color;
	int x_way;//1 2 左右
	int y_way;//1 2 上下
	int r;

	void move();
};
BALL ball;

//板子 上下左右边界 颜色 步调
struct BORDER {
	int left;
	int right;
	int top;
	int bottom;
	int color;
	int step;

	void move(char input);
};
BORDER border;

//方块
const int side = 30;
int step;
void Bset();
void Bmove();
int block[height / side][width / side] = { 0 };
int Bcolor;

//时间
clock_t start, end;
double duration;

//得分
int score;


void initiation();//初始化
void updateWithoutinput();//无输入更新
void updateWithinput();//输入更新
double distance(int x1, int y1, int x2, int y2);//计算距离


// 主函数
int main()
{
	initiation();

	while (isGame)
	{
		updateWithoutinput();
		updateWithinput();
	}

	closegraph();					// 关闭绘图窗口
	return 0;
}


void initiation()
{
	srand((unsigned)time(NULL));	// 随机种子

	//基本设置
	initgraph(width, height);;			// 创建绘图窗口
	backgroundcolor = 0xFFFFFF;	//背景白色
	setbkcolor(backgroundcolor);
	cleardevice();// 用背景色清空屏幕
	isGame = 1;
	score = 0;
	
	//时间
	start = end = clock();
	duration = 0;

	//方块设置
	step = 0;
	Bset();
	Bcolor = 0x1A2B3C;
	//方块绘制
	setfillstyle(HS_HORIZONTAL);
	setfillcolor(Bcolor);
	int i = 0;
	for (int j = 0; j < width / side; j++)
	{
		if (block[i][j] == 1)
		{
			fillrectangle(j * side, i * side, (j + 1) * side, (i + 1) * side);
		}
	}


	//板子设置
	border.left = width / 3;
	border.right = width * 2 / 3;
	border.top = height * 4 / 5;
	border.bottom = border.top - 10;
	border.color = 0;
	border.step = 10;
	//绘制板子
	setfillstyle(BS_SOLID);
	setfillcolor(border.color);
	fillrectangle(border.left, border.top, border.right, border.bottom);

	//小球设置
	ball.r = 10;
	ball.x = width / 2;
	ball.y = border.top - 4 * ball.r;;
	ball.step = 2;
	ball.color = 0;
	ball.x_way = 1;
	ball.y_way = 1;
	//绘制小球
	setfillstyle(BS_SOLID);
	setfillcolor(ball.color);
	fillcircle(ball.x, ball.y, 10);
};



void updateWithoutinput()
{
	
	Bmove();
	ball.move();

	return;
}

void updateWithinput()
{
	char input = 0;
	if (_kbhit())
	{
		input = _getch();
		border.move(input);
	}
	return;
}

void BALL::move()
{
	BeginBatchDraw();

	//先清除原来的（画背景色的球）
	setfillstyle(BS_SOLID);
	setfillcolor(backgroundcolor);
	fillcircle(x, y, 10);

	int i, j;//球心在哪个方块区域
	i = y / side;
	j = x / side;

	if (block[i][j])
	{
		block[i][j] = 0;
		setfillstyle(BS_SOLID);
		setfillcolor(backgroundcolor);
		fillrectangle(j * side, i * side, (j + 1) * side, (i + 1) * side);

	}

	if (x_way == 1)
	{
		if (x - r <= 0//撞边界
			|| (x - r <= border.right && x + r >= border.left && y >= border.bottom && y <= border.top)//撞板子
			|| distance(x, y, border.right, border.top) <= r
			|| distance(x, y, border.right, border.bottom) <= r)
		{
			x_way = 2;
			x += step;
		}
		else if (j > 0 && block[i][j - 1])//撞方块
		{
			if (x - r <= j * side)
			{
				score++;
				block[i][j - 1] = 0;
				//擦除
				setfillstyle(BS_SOLID);
				setfillcolor(backgroundcolor);
				fillrectangle((j - 1) * side, i * side, j * side, (i + 1) * side);
				//反弹
				x_way = 2;
				x += step;
			}
			else
			{
				x -= step;
			}
		}
		else if (j > 0 && i > 0 && i < (height / side) - 1 &&
			(block[i - 1][j - 1] && (distance(x, y, j * side, i * side) <= r)
				|| (block[i + 1][j - 1] && distance(x, y, j * side, (i + 1) * side) <= r)))
		{
			setfillstyle(BS_SOLID);
			setfillcolor(backgroundcolor);
			if (y_way == 1)
			{
				block[i - 1][j - 1] = 0;
				fillrectangle((j - 1) * side, (i - 1) * side, j * side, i * side);
				y_way = 2;
			}
			else
			{
				block[i + 1][j - 1] = 0;
				fillrectangle((j - 1) * side, (i + 1) * side, j * side, (i + 2) * side);
				y_way = 1;
			}
			//反弹
			score++;
			x_way = 2;
			x += step;
		}
		else
		{
			x -= step;
		}
	}
	else if (x_way == 2)
	{
		if (x + r >= width
			|| (x - r <= border.right && x + r >= border.left && y >= border.bottom && y <= border.top)
			|| distance(x, y, border.left, border.top) <= r
			|| distance(x, y, border.left, border.bottom) <= r)
		{
			x_way = 1;
			x -= step;
		}
		else if (j < (width / side) - 1 && block[i][j + 1])
		{
			if (x + r >= (j + 1) * side)
			{
				score++;
				block[i][j + 1] = 0;
				//覆盖
				setfillstyle(BS_SOLID);
				setfillcolor(backgroundcolor);
				fillrectangle((j + 1) * side, i * side, (j + 2) * side, (i + 1) * side);
				//反弹
				x_way = 1;
				x -= step;
			}
			else
				x += step;
		}
		else if (j < (width / side) - 1 && i > 0 && i < (height / side) - 1 &&
			((block[i - 1][j + 1] && distance(x, y, (j + 1) * side, i * side) <= r)
				|| (block[i + 1][j + 1] && distance(x, y, (j + 1) * side, (i + 1) * side) <= r)))
		{
			setfillstyle(BS_SOLID);
			setfillcolor(backgroundcolor);
			if (y_way == 1)
			{
				block[i - 1][j + 1] = 0;
				fillrectangle((j + 1) * side, (i - 1) * side, (j + 2) * side, i * side);
				y_way = 2;
			}
			else
			{
				block[i + 1][j + 1] = 0;
				fillrectangle((j + 1)* side, (i + 1)* side, (j + 2)* side, (i + 2)* side);
				y_way = 1;
			}
			//反弹
			score++;
			x_way = 2;
			x += step;
		}
		else
		{
			x += step;
		}
	}


	if (y_way == 2)
	{
		if (y + r >= height
			|| (y < border.bottom && y + r >= border.bottom && x >= border.left && x <= border.right)
			|| distance(x, y, border.left, border.bottom) <= r
			|| distance(x, y, border.right, border.bottom) <= r)
		{
			y_way = 1;
			y -= step;
		}
		else if (i < (height / side) - 1 && block[i + 1][j])
		{
			if (y + r >= (i + 1) * side)
			{
				score++;
				//擦除
				block[i + 1][j] = 0;
				setfillstyle(BS_SOLID);
				setfillcolor(backgroundcolor);
				fillrectangle(j * side, (i + 1) * side, (j + 1) * side, (i + 2) * side);
				//反弹
				y_way = 2;
				y -= step;
			}
			else
			{
				y += step;
			}
		}
		else
		{
			y += step;
		}
	}
	else if (y_way == 1)
	{
		if (y - r <= 0
			|| (y > border.top && y - r <= border.top && x >= border.left && x <= border.right)
			|| distance(x, y, border.left, border.top) <= r
			|| distance(x, y, border.right, border.top) <= r)
		{

			y_way = 2;
			y += step;
		}
		else if (i > 0 && block[i - 1][j])
		{
			if (y - r <= i * side)
			{
				score++;
				block[i - 1][j] = 0;
				//重涂
				setfillstyle(BS_SOLID);
				setfillcolor(backgroundcolor);
				fillrectangle(j * side, (i - 1) * side, (j + 1) * side, i * side);
				//反弹
				y_way = 2;
				y += step;
			}
			else
				y -= step;
		}
		else
		{
			y -= step;
		}
	}

	setfillstyle(BS_SOLID);
	setfillcolor(ball.color);
	fillcircle(ball.x, ball.y, 10);
	FlushBatchDraw();
	Sleep(10);
	EndBatchDraw();

}

void BORDER::move(char input)
{
	//先清除板子
	setfillstyle(BS_SOLID);
	setfillcolor(backgroundcolor);
	fillrectangle(border.left, border.top, border.right, border.bottom);

	if (input == 'a')
	{
		if (left - step >= 0)
		{
			left -= step;
			right -= step;
		}
	}
	if (input == 'd')
	{
		if (right + step <= width)
		{
			left += step;
			right += step;
		}
	}

	setfillstyle(BS_SOLID);
	setfillcolor(border.color);
	fillrectangle(border.left, border.top, border.right, border.bottom);
}

void Bmove()
{
	step++;
	if (step == 150)
	{
		BeginBatchDraw();

		step = 0;
		int i, j;
		//一行一行来移动
		for (i = height / side - 1; i > 0; i--)
		{
			for (j = 0; j < (width / side); j++)
			{
				block[i][j] = block[i - 1][j];
				
				if (block[i - 1][j] == 1)
				{
					if ((i + 1) * side >= border.top)
					{
						isGame = 0;
						EndBatchDraw();
						return;
					}
					//先擦除
					setfillstyle(BS_SOLID);
					setfillcolor(backgroundcolor);
					fillrectangle((j - 1) * side, (i - 1) * side, j * side, i * side);
					//重画
					setfillcolor(Bcolor);
					fillrectangle(j * side, i * side, (j + 1) * side, (i + 1) * side);
					
					FlushBatchDraw();
				}
				block[i - 1][j] = 0;
			}
		}
		Bset();
		EndBatchDraw();
	}
}

void Bset()
{
	int i, j;
	int num = rand() % 5 + 3;//3-8d个方块
	for (i = 0; i < num; i++)
	{
		j = rand() % (width / side);
		if (block[0][j] != 1)
		{
			block[0][j] = 1;
		}
		else
		{
			i--;
		}

	}
}

double distance(int x1, int y1, int x2, int y2)
{
	return sqrt((double)((x1 - x2) * (x1 - x2)) + (double)((y1 - y2) * (y1 - y2)));
}
