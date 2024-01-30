#include <graphics.h>
#include <conio.h>
#include <time.h>

// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

IMAGE img_bk, img_bd1, img_bd2, img_bar_up1, img_bar_up2, img_bar_down1, img_bar_down2; int bird_x;
int bird_y;

int bar_left;
int bar_top1, bar_top2;
int isGame = 1;

void startup()
{
	srand((unsigned)time(NULL));
	initgraph(350, 600);
	loadimage(&img_bk, L"background.jpg");
	loadimage(&img_bd1, L"bird1.jpg");
	loadimage(&img_bd2, L"bird2.jpg");
	loadimage(&img_bar_up1, L"bar_up1.gif");
	loadimage(&img_bar_up2, L"bar_up2.gif");
	loadimage(&img_bar_down1, L"bar_down1.gif");
	loadimage(&img_bar_down2, L"bar_down2.gif");
	bird_x = 50;
	bird_y = 200;
	bar_left = 350;
	bar_top1 = -300;
	bar_top2 = 400;
	BeginBatchDraw();

	mciSendString(L"open background.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString(L"play bkmusic repeat", NULL, 0, NULL);  // 循环播放
}

void gameover()
{
	// 输出数值 1024，先将数字格式化输出为字符串（自适应字符集）
	outtextxy(150, 280, L"You Lose");
	mciSendString(L"close bkmusic", NULL, 0, NULL);
	EndBatchDraw();
	_getch();
	closegraph();
}

void show()
{
	putimage(0, 0, &img_bk);	// 显示背景	
	putimage(bar_left, bar_top1, &img_bar_up1, NOTSRCERASE); // 显示上一半的障碍物
	putimage(bar_left, bar_top1, &img_bar_up2, SRCINVERT);
	putimage(bar_left, bar_top2, &img_bar_down1, NOTSRCERASE); // 显示下一半的障碍物
	putimage(bar_left, bar_top2, &img_bar_down2, SRCINVERT);
	putimage(bird_x, bird_y, &img_bd1, NOTSRCERASE); // 显示小鸟	
	putimage(bird_x, bird_y, &img_bd2, SRCINVERT);
	FlushBatchDraw();
	Sleep(50);
}

void updateWithoutInput()
{
	if (bird_y < 500)
		bird_y = bird_y + 5;
	if (bar_left > -140)
		bar_left -= 5;
	else
	{
		bar_left = 350;
		bar_top1 = -(rand() % 300 + 300);
		bar_top2 = bar_top1 + (rand() % 50 + 100) + 600;
	}
}

void updateWithInput()
{
	char input;
	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();
		if (input == ' ' && bird_y > 20)
		{
			bird_y = bird_y - 40;
			mciSendString(L"close jpmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(L"open Jump.mp3 alias jpmusic", NULL, 0, NULL); // 打开跳动音乐
			mciSendString(L"play jpmusic", NULL, 0, NULL); // 仅播放一次

		}
	}
	if (bird_x + 34 > bar_left && bird_x <= bar_left + 140)
	{
		if (bird_y < bar_top1 + 600 || bird_y + 24 > bar_top2)
			isGame = 0;
	}
}

int main()
{
	startup();  // 数据初始化	
	while (isGame)  //  游戏循环执行
	{
		show();  // 显示画面
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();     // 与用户输入有关的更新
	}
	gameover();     // 游戏结束、后续处理
	return 0;
}
