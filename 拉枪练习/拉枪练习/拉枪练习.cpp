#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>


int isGame = 1;//判断游戏进行
int isStart = 0;//判断游戏开始
int isOver = 0;//判断游戏结束
//边界
const int width = 600;
const int height = 600;

//经过时间
clock_t start, end;
double duration;
int last;

//字符输出
TCHAR s[20];

//准星
IMAGE foresight1, foresight2;

//鼠标
MOUSEMSG m;

//气球
int bool_x, bool_y, bool_r;

//得分
int score = 0;

void init();
void beforeStart();
void updateWithoutInput();
void updateWithInput();
void Show();
void boolSet();
double distance(int x1, int y1, int x2, int y2);
void gameOver();

int main()
{
	init();
	beforeStart();
	while (!isStart)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton && m.x >= width / 2 - 60 && m.x <= width / 2 + 60 && m.y >= height / 2 - 50 && m.y <= height / 2)
			{
				isStart = 1;
			}
			beforeStart();
		}
	}
	start = clock();
	while (isGame)
	{
		updateWithInput();
		updateWithoutInput();
	}
	while (!isOver)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			if (m.mkLButton && m.x >= width / 2 - 60 && m.x <= width / 2 + 60 && m.y >= height / 2 - 50 && m.y <= height / 2)
			{
				isOver = 1;
			}
			gameOver();
		}
	}
	EndBatchDraw();
}

void init()
{
	srand((unsigned)time(NULL));
	//绘图窗口初始化
	initgraph(width, height);

	//背景
	setbkcolor(0x1F1F1F);
	cleardevice();

	//开始绘画
	BeginBatchDraw();

	//准星
	loadimage(&foresight1, _T("./image/foresight1.png"));
	loadimage(&foresight2, _T("./image/foresight2.png"));

	//随机生成气球的参数
	boolSet();

	FlushBatchDraw();
}

void beforeStart()
{
	cleardevice();
	// 显示准星
	putimage(m.x - 10, m.y - 10, &foresight1, NOTSRCERASE);
	putimage(m.x - 10, m.y - 10, &foresight2, SRCINVERT);

	//显示开始游戏的按钮
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(30, 0, _T("宋体"));
	outtextxy(width / 2 - 60, height / 2 - 50, _T("开始游戏"));
	outtextxy(width/2-250, 0, _T("在限定时间内射击气球，越多得分越高"));

	FlushBatchDraw();
}

void updateWithInput()
{
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.mkLButton && distance(m.x, m.y, bool_x, bool_y) <= bool_r)
		{
			score++;
			boolSet();
		}
	}
}

void updateWithoutInput()
{
	//时间参数
	end = clock();
	duration = (double)(end - start) / CLK_TCK;
	last = 20 - duration;
	Show();
	if (last <= 0)
	{
		isGame = 0;
	}
}

void Show()
{
	cleardevice();

	// 显示准星
	putimage(m.x - 10, m.y - 10, &foresight1, NOTSRCERASE); 
	putimage(m.x - 10, m.y - 10, &foresight2, SRCINVERT);

	//显示气球
	setfillcolor(WHITE);
	fillcircle(bool_x, bool_y, bool_r);

	//显示时间
	_stprintf_s(s, _T("剩余时间：%d"),last);
	outtextxy(0, height - 30, s);

	//显示分数
	_stprintf_s(s, _T("得分：%d"), score);
	outtextxy(width - 180, height - 30, s);
	FlushBatchDraw();
}

void boolSet()
{
	bool_r = rand() % 10 + 50;
	bool_x = rand() % (width - 2 * bool_r) + bool_r;
	bool_y = rand() % (height - 2 * bool_r) + bool_r;
}

double distance(int x1, int y1, int x2, int y2)
{
	return sqrt((double)((x1 - x2) * (x1 - x2)) + (double)((y1 - y2) * (y1 - y2)));
}

void gameOver()
{
	cleardevice();

	// 显示准星
	putimage(m.x - 10, m.y - 10, &foresight1, NOTSRCERASE);
	putimage(m.x - 10, m.y - 10, &foresight2, SRCINVERT);
	//显示得分
	_stprintf_s(s, _T("得分：%d"), score);
	outtextxy(width / 2 - 65, height / 2, s);
	//结束游戏
	outtextxy(width / 2 - 60, height / 2 - 50, _T("结束游戏"));

	FlushBatchDraw();
}