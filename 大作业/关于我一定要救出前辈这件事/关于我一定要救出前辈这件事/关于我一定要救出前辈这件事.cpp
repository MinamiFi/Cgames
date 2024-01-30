#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

void init();//初始化
void gameOver();//游戏结束
void gameClear();//游戏通关
void conversation(wchar_t character[], wchar_t text[], int& isA);//对话
void dconver(wchar_t character[], wchar_t text1[], wchar_t text2[], int& isA);//两行对话
void converpause(int& isA);

//通用的参数
//作者名字
wchar_t author_name[] = _T("南  飞");
//游戏信息
const int width = 1024;
const int height = 512;

//玩家信息
IMAGE player1;
IMAGE player2;
int player_x, player_y;
int jumpup, jumpdown;//跳跃状态
int isjump;
int playermove = 0;//移动状态
wchar_t player_name[20];

wchar_t mario_name[20] = L"马里奥";

//血量显示
IMAGE heart1, heart2, heartempty;
int heart;

//背景
int bkleft;

//奖励
IMAGE star1, star2;
int star_i = 0;

//怪
IMAGE monster1, monster2;
int monster_left;
int monster_i;
int monster_life;

//对话
IMAGE conversation1, conversation2;
IMAGE answer1, answer2;
int isAnswer;
wchar_t player_word1[40], player_word2[40];
int choose;

int isStart = 0;
//是否拿到星星
int isStar;

//时间
clock_t start, end;
double duration = 0;

//获胜或失败封面
IMAGE win_title1;
IMAGE win_title2;
IMAGE lost_title1;
IMAGE lost_title2;

IMAGE relife_title1;
IMAGE relife_title2;
IMAGE relife_small1;
IMAGE relife_small2;

IMAGE exit_title1;
IMAGE exit_title2;
IMAGE exit_small1;
IMAGE exit_small2;

//封面
typedef struct Surface
{
	IMAGE title1;
	IMAGE title2;
	IMAGE bkground;
	IMAGE btn1;
	IMAGE btn2;

	void set();
	void updateWithInput();
	void updateWithoutInput();
	void show();
}Sur;
Sur s;

//教学
typedef struct Before //进入游戏之前，填写玩家名字等
{
	void set();
	void updateWithInput();
	void updateWithoutInput();
	void show();

	//流程
	void inputname();
	void introduce();
	void teach();
	void getStar();

	IMAGE bkground;//背景
	IMAGE grass1, grass2;//土块 48*33

	//剧情是否拿到星星
	int isfinish;
}Bef;
Bef b = {};
int isBefore = 1;

//关卡
typedef struct First {
	void set();
	void updateWithInput();
	void updateWithoutInput();
	void show();

	//扣血
	void hurt();
	//剧情
	void begin();
	int isBack;

	int bl = 0;//刺
	int block[32][16] = { 0 };//0空 1砖 2刺1 3刺2
	//陷阱
	int track3;
	int track3_y;
	int track4;
	int track4_y;

	IMAGE bkground;
	IMAGE bride;
	IMAGE thorn1;
	IMAGE thorn2;
	IMAGE thorn_down1;
	IMAGE thorn_down2;

	IMAGE prison1;
	IMAGE prison2;

	IMAGE mario1;
	IMAGE mario2;
}Fir;
Fir f;



int main()
{
	init();
	//封面surface
	s.set();
	while (!isStart)
	{
		s.show();
		s.updateWithInput();
		s.updateWithoutInput();
	}
	//教学关卡begin
	b.set();
	b.show();
	b.inputname();
	b.introduce();
	b.teach();
	//正式关卡First
	f.set();
	f.begin();
	while (heart && !isStar)
	{
		f.show();
		f.updateWithInput();
		f.updateWithoutInput();
		if (!heart)
		{
			gameOver();
		}
	}
	while (!f.isBack && heart)
	{
		f.show();
		f.updateWithInput();
		f.updateWithoutInput();
		if (player_x <= 80 && player_y < 96)
		{
			f.isBack = 1;
		}
		if (!heart)
		{
			gameOver();
		}
	}
	if (heart)
	{
		wchar_t str[40];
		swprintf(player_word1, 40, L"下次可小心点吧");
		swprintf(player_word2, 40, L"建议回去之后给我升官");
		isAnswer = 1;
		f.show();
		swprintf(str, 40, L"我的好后辈终于等到你来救我了，我们快一起出去吧");
		conversation(mario_name, str, isAnswer);
		converpause(isAnswer);
		isAnswer = 0;
		swprintf(str, 40, L"哈哈，下次一定");
		conversation(mario_name, str, isAnswer);
		converpause(isAnswer);
		gameClear();
	}
	return 0;
}

//初始化
void init()
{
	srand((unsigned)time(NULL));

	//绘制窗口
	initgraph(width, height);
	BeginBatchDraw();

	//玩家设定
	loadimage(&player1, _T("image/playermove1.jpg"));
	loadimage(&player2, _T("image/playermove2.jpg"));
	player_x = 50;
	player_y = height / 2 - 55;
	jumpup = jumpdown = 0;
	heart = 5;

	//对话
	loadimage(&conversation1, _T("image/conversation1.jpg"));
	loadimage(&conversation2, _T("image/conversation2.jpg"));
	isAnswer = 0;
	choose = 0;
	loadimage(&answer1, _T("image/conversation3.jpg"));
	loadimage(&answer2, _T("image/conversation4.jpg"));

	//星星
	loadimage(&star1, _T("image/star1.jpg"));
	loadimage(&star2, _T("image/star2.jpg"));
	star_i = 0;

	//血量
	loadimage(&heart1, _T("image/heart1.jpg"));
	loadimage(&heart2, _T("image/heart2.jpg"));
	loadimage(&heartempty, _T("image/heartempty.jpg"));

	start = clock();

	//win lost窗口
	loadimage(&win_title1, L"image/win_title1.jpg");
	loadimage(&win_title2, L"image/win_title2.jpg");
	loadimage(&lost_title1, L"image/lost_title1.jpg");
	loadimage(&lost_title2, L"image/lost_title2.jpg");

	loadimage(&relife_title1, L"image/relife_title1.jpg");
	loadimage(&relife_title2, L"image/relife_title2.jpg");
	loadimage(&relife_small1, L"image/relife_small1.jpg");
	loadimage(&relife_small2, L"image/relife_small2.jpg");

	loadimage(&exit_title1, L"image/exit_title1.jpg");
	loadimage(&exit_title2, L"image/exit_title2.jpg");
	loadimage(&exit_small1, L"image/exit_small1.jpg");
	loadimage(&exit_small2, L"image/exit_small2.jpg");
}

//结束
void gameOver()
{
	mciSendString(L"close bkmusic", NULL, 0, NULL);//关闭背景音乐
	int isrelife = 0;
	int isexit = 0;
	while (1)
	{
		cleardevice();
		putimage(0, 0, width, height, &s.bkground, 0, 0);
		putimage(6 * 32 + 16, 2 * 32, &lost_title2, NOTSRCERASE);
		putimage(6 * 32 + 16, 2 * 32, &lost_title1, SRCINVERT);

		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x >= 12 * 32 && m.x <= 20 * 32 && m.y <= 10 * 32 && m.y >= 9 * 32)
		{
			putimage(6 * 32 + 16, 8 * 32, &relife_title2, NOTSRCERASE);
			putimage(6 * 32 + 16, 8 * 32, &relife_title1, SRCINVERT);
			isrelife = 1;
		}
		else
		{
			putimage(6 * 32 + 16, 8 * 32, &relife_small2, NOTSRCERASE);
			putimage(6 * 32 + 16, 8 * 32, &relife_small1, SRCINVERT);
			isrelife = 0;
		}

		if (m.x >= 12 * 32 && m.x <= 20 * 32 && m.y <= 13 * 32 && m.y >= 12 * 32)
		{
			putimage(6 * 32 + 16, 11 * 32, &exit_title2, NOTSRCERASE);
			putimage(6 * 32 + 16, 11 * 32, &exit_title1, SRCINVERT);
			isexit = 1;
		}
		else
		{
			putimage(6 * 32 + 16, 11 * 32, &exit_small2, NOTSRCERASE);
			putimage(6 * 32 + 16, 11 * 32, &exit_small1, SRCINVERT);
			isexit = 0;
		}

		FlushBatchDraw();

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (isrelife)
			{
				heart = 5;
				//背景音乐
				mciSendString(L"open bkmusic.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
				mciSendString(L"play bkmusic repeat", NULL, 0, NULL);  // 循环播放
				break;
			}
			else if (isexit)
			{
				EndBatchDraw();
				closegraph();
				exit(0);
			}
		}
	}
}

//通关
void gameClear()
{
	int isexit = 0;
	while (1)
	{
		cleardevice();
		putimage(0, 0, width, height, &s.bkground, 0, 0);
		putimage(6 * 32 + 16, 2 * 32, &win_title2, NOTSRCERASE);
		putimage(6 * 32 + 16, 2 * 32, &win_title1, SRCINVERT);

		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x >= 12 * 32 && m.x <= 20 * 32 && m.y <= 10 * 32 && m.y >= 9 * 32)
		{
			putimage(6 * 32 + 16, 8 * 32, &exit_title2, NOTSRCERASE);
			putimage(6 * 32 + 16, 8 * 32, &exit_title1, SRCINVERT);
			isexit = 1;
		}
		else
		{
			putimage(6 * 32 + 16, 8 * 32, &exit_small2, NOTSRCERASE);
			putimage(6 * 32 + 16, 8 * 32, &exit_small1, SRCINVERT);
			isexit = 0;
		}

		FlushBatchDraw();

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (isexit)
			{
				EndBatchDraw();
				closegraph();
				exit(0);
			}
		}
	}
}

//对话
void conversation(wchar_t character[], wchar_t text[], int& isA)
{
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(0, 0, _T("宋体"));
	if (isA)
	{
		putimage(width / 2 - 325, height - 270, &answer2, NOTSRCERASE);
		putimage(width / 2 - 325, height - 270, &answer1, SRCINVERT);
		outtextxy(width / 2 + 100, height - 245, player_word1);
		outtextxy(width / 2 + 100, height - 200, player_word2);
	}
	else
	{
		putimage(width / 2 - 325, height - 200, &conversation2, NOTSRCERASE);
		putimage(width / 2 - 325, height - 200, &conversation1, SRCINVERT);
	}
	outtextxy(width / 2 - 325 + 20, height - 200 + 10, character);
	outtextxy(width / 2 - 325 + 20, height - 200 + 65, text);

	FlushBatchDraw();
	Sleep(500);

}

void dconver(wchar_t character[], wchar_t text1[], wchar_t text2[], int& isA)
{
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(0, 0, _T("宋体"));
	if (isA)
	{
		putimage(width / 2 - 325, height - 300, &answer2, NOTSRCERASE);
		putimage(width / 2 - 325, height - 300, &answer1, SRCINVERT);
		outtextxy(width / 2 + 100, height - 245, player_word1);
		outtextxy(width / 2 + 100, height - 200, player_word2);
	}
	else
	{
		putimage(width / 2 - 325, height - 200, &conversation2, NOTSRCERASE);
		putimage(width / 2 - 325, height - 200, &conversation1, SRCINVERT);
	}
	outtextxy(width / 2 - 325 + 20, height - 200 + 10, character);
	outtextxy(width / 2 - 325 + 20, height - 200 + 65, text1);
	outtextxy(width / 2 - 325 + 20, height - 200 + 90, text2);

	FlushBatchDraw();
	Sleep(500);
}

void converpause(int& isA)
{
	if (isA)
	{
		isA = 0;
		while (1)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				choose = 1;
				break;
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				choose = 2;
				break;
			}
		}
	}
	else
	{
		while (1)
		{
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				Sleep(500);
				break;
			}
		}
	}

}

//Surface的一些函数
void Sur::set()
{
	//背景音乐
	mciSendString(L"open bkmusic.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString(L"play bkmusic repeat", NULL, 0, NULL);  // 循环播放

	//背景
	loadimage(&bkground, _T("image/Sur_bk.jpg"));
	bkleft = 0;
	//标题
	loadimage(&title1, _T("image/Sur_title1.jpg"));
	loadimage(&title2, _T("image/Sur_title2.jpg"));

	//按钮
	loadimage(&btn1, _T("image/Sur_startbtn1.jpg"));
	loadimage(&btn2, _T("image/Sur_startbtn2.jpg"));
}

void Sur::updateWithInput()
{
	//鼠标
	MOUSEMSG m = {};
	m = GetMouseMsg();
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && m.x >= width / 2 - 128 && m.x <= width / 2 + 128 && m.y >= height / 2 && m.y <= height / 2 + 100)
	{
		isStart = 1;
	}
}

void Sur::updateWithoutInput()
{}

void Sur::show()
{
	//背景
	putimage(0, 0, width, height, &bkground, bkleft, 0);
	//标题
	putimage(50, 0, &title2, NOTSRCERASE);
	putimage(50, 0, &title1, SRCINVERT);

	//开始按钮
	putimage(width / 2 - 128, height / 2, &btn2, NOTSRCERASE);
	putimage(width / 2 - 128, height / 2, &btn1, SRCINVERT);


	//一些说明
	FlushBatchDraw();
	Sleep(10);
}

//Before的一些函数
void Bef::set()
{
	//背景
	loadimage(&bkground, _T("image/Bef_bk.jpg"), width + 200, height);
	loadimage(&grass1, _T("image/Bef_grass1.jpg"));
	loadimage(&grass2, _T("image/Bef_grass2.jpg"));
	bkleft = 200;

	//怪物
	loadimage(&monster1, _T("image/monster1_1.jpg"));
	loadimage(&monster2, _T("image/monster1_2.jpg"));
	monster_i = 0;
	monster_left = width / 3 * 2;
	monster_life = 3;

	//剧情
	isStar = 0;
	isfinish = 0;
	star_i = 0;
}

void Bef::show()
{
	cleardevice();
	//背景覆盖
	putimage(0, 0, width, height, &bkground, bkleft, 0);
	//地面
	for (int i = 0; i <= width / 40; i++)
	{
		putimage(i * 40, height / 2, 40, 33, &grass2, 4, 0, NOTSRCERASE);
		putimage(i * 40, height / 2, 40, 33, &grass1, 4, 0, SRCINVERT);
	}
	for (int i = 0; i < height / 2 / 18; i++)
	{
		for (int j = 0; j <= width / 40; j++)
		{
			putimage(j * 40, height / 2 + 33 + i * 18, 40, 18, &grass2, 4, 15, NOTSRCERASE);
			putimage(j * 40, height / 2 + 33 + i * 18, 40, 18, &grass1, 4, 15, SRCINVERT);
		}
	}
	//显示小怪
	putimage(monster_left - bkleft + 200, height / 2 - 25, 30, 30, &monster2, 91 - 30 * monster_i, 0, NOTSRCERASE);
	putimage(monster_left - bkleft + 200, height / 2 - 25, 30, 30, &monster1, 91 - 30 * monster_i, 0, SRCINVERT);
	//显示玩家
	putimage(player_x, player_y, 32, 58, &player2, 160 + playermove * 32, 0, NOTSRCERASE);
	putimage(player_x, player_y, 32, 58, &player1, 160 + playermove * 32, 0, SRCINVERT);

	//如果还没拿到星星
	if (!isStar)
	{
		putimage(-bkleft, height / 2 - 25, 25, 24, &star2, star_i * 25, 0, NOTSRCERASE);
		putimage(-bkleft, height / 2 - 25, 25, 24, &star1, star_i * 25, 0, SRCINVERT);
	}
	//显示血量
	for (int i = 0; i < 5; i++)
	{
		putimage(i * 27, 0, &heart2, NOTSRCERASE);
		if (i < heart)
		{
			putimage(i * 27, 0, &heart1, SRCINVERT);
		}
		else
		{
			putimage(i * 27, 0, &heartempty, SRCINVERT);
		}
	}
	Sleep(70);
	FlushBatchDraw();
}

void Bef::updateWithInput()
{
	//跳跃
	if (jumpdown > 0)
	{
		if (jumpup > 0)
		{
			player_y -= 15;
			jumpup--;
		}
		else if (jumpdown > 0)
		{
			player_y += 15;
			jumpdown--;
		}
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//跳动音效
		mciSendString(L"close jpmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
		mciSendString(L"open jump.mp3 alias jpmusic", NULL, 0, NULL); // 打开跳动音乐
		mciSendString(L"play jpmusic", NULL, 0, NULL); // 仅播放一次

		if (playermove >= 0)
		{
			playermove = 4;
		}
		else
		{
			playermove = -5;
		}
		jumpup = 4;
		jumpdown = 5;
		player_y -= 15;
	}
	else if (!jumpdown)
	{
		if (playermove == 4)
		{
			playermove = 0;
		}
		else if (playermove == -5)
		{
			playermove = -1;
		}
	}
	//前进后退
	if (GetAsyncKeyState(65) & 0x8000)
	{
		if (player_x <= 100 && bkleft > 0)
		{
			bkleft -= 10;
		}
		else if (player_x - 10 >= 0)
		{
			player_x -= 10;
		}
		if (playermove == 4 || playermove == -5)
		{
			playermove = -5;
		}
		else if (playermove == -4 || playermove >= 0)
		{
			playermove = -1;
		}
		else
		{
			playermove--;
		}
	}
	else if (GetAsyncKeyState(68) & 0x8000)
	{
		if (player_x + 32 + 200 >= width && bkleft < 200)
		{
			bkleft += 10;
		}
		else if (player_x + 32 + 10 <= width)
		{
			player_x += 10;
		}
		if (playermove == 4 || playermove == -5)
		{
			playermove = 4;
		}
		else if (playermove == 3 || playermove < 0)
		{
			playermove = 0;
		}
		else
		{
			playermove++;
		}
	}
	else if (playermove != 4 && playermove != -5)//走路姿势复位
	{
		if (playermove >= 0)
		{
			playermove = 0;
		}
		else
		{
			playermove = -1;
		}
	}
}

void Bef::updateWithoutInput()
{
	if (isStar == 0 && player_x <= 25 && bkleft == 0 && player_y <= height / 2 + 55)
	{
		getStar();
	}
	else if (!isStar)
	{
		if (star_i == 7)
		{
			star_i = 0;
		}
		else
		{
			star_i++;
		}
	}

	if (isStar && isfinish)
	{
		//踩怪
		if (monster_i != 3 && player_x + 32 >= monster_left - bkleft + 200 && player_x <= monster_left - bkleft + 200 + 30 && jumpdown == 1)
		{
			jumpup += 4;
			jumpdown += 5;
			player_y -= 15;
			monster_i = 3;
		}
		//碰怪扣血
		else if (monster_i != 3 && monster_left - bkleft + 200 <= player_x + 30 && monster_left - bkleft + 200 + 30 >= player_x && jumpdown < 2)
		{
			//背景音乐
			mciSendString(L"close bkmusic", NULL, 0, NULL);//关闭背景音乐

			//扣血
			mciSendString(L"open dead.mp3 alias deadmusic", NULL, 0, NULL); // 打开扣血
			mciSendString(L"play deadmusic", NULL, 0, NULL); // 仅播放一次

			show();
			Sleep(1000);
			int i = 3;
			while (player_y <= height)
			{
				if (i > 0)
				{
					player_y -= 20;
					i--;
				}
				else
				{
					player_y += 50;
				}
				show();
				Sleep(100);
			}

			heart--;
			player_x = 50;
			player_y = height / 2 - 55;
			jumpup = jumpdown = 0;
			if (heart)
			{
				mciSendString(L"open bkmusic.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
				mciSendString(L"play bkmusic repeat", NULL, 0, NULL);  // 循环播放
			}
		}

	}
}

void Bef::inputname()
{
	//字符输出
	wchar_t str[] = _T("欢迎来到游戏，我是本游戏的作者，首先能否告诉我你的名字");
	wchar_t str2[] = _T("（按空格继续对话，如果有选项的按上下键选择）");
	dconver(author_name, str, str2, isAnswer);
	converpause(isAnswer);
	while (!InputBox(player_name, 20, L"请输入您的游戏名"));
	Sleep(300);
}

void Bef::introduce()
{
	wchar_t str[40], str2[40];
	swprintf_s(str, 40, L"感谢您的配合，%s", player_name);
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf_s(str, 40, L"接下来由我，为你介绍游戏背景");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"你叫%s，是一个平平无奇的下水道工人。", player_name);
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"一天，你看到恶龙库巴从你们的公司离开，而你的前辈马里奥好像被");
	swprintf(str2, 40, L"带走了");
	show();
	dconver(author_name, str, str2, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"为了救回你敬爱的马里奥前辈，你打算立马冲进库巴的城堡去营救。");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"是不是十分简单易懂。");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

}

void Bef::teach()
{
	wchar_t str[40], str2[40];

	swprintf(str, 40, L"好了不要偷偷吐槽这么简单的设定了，让我教教你最基本的操作。");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"按a向左走，按d向右走，按空格键跳跃");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"想必你平常也不怎么锻炼，没病先走两步");
	show();
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

	while (player_x < width / 2)
	{
		updateWithInput();
		updateWithoutInput();
		show();
	}

	swprintf(str, 40, L"看来你已经完全适应人类的活动方式了呢/doge。看到了吗，前面有一");
	swprintf(str2, 40, L"个小怪。想办法过去吧。");
	show();
	dconver(author_name, str, str2, isAnswer);
	converpause(isAnswer);

	while (player_x <= monster_left - bkleft + 200 + 20)
	{
		updateWithInput();
		updateWithoutInput();
		show();
	}

	wchar_t monster_name[] = L"蘑菇头";
	swprintf(str, 40, L"喂！");
	while (jumpdown > 0)
	{
		if (jumpup > 0)
		{
			player_y -= 15;
			jumpup--;
		}
		else if (jumpdown >= 0)
		{
			player_y += 15;
			jumpdown--;
		}
		show();
	}
	playermove = 0;
	show();
	conversation(monster_name, str, isAnswer);
	converpause(isAnswer);

	while (monster_left - bkleft + 200 <= player_x + 35)
	{
		monster_i++;
		if (monster_i == 2)
		{
			monster_i = 0;
		}
		monster_left += 10;
		show();
	}

	swprintf(str, 40, L"没看到本大爷在这里看着吗？！你是个什么品种的东西。");
	show();
	isAnswer = 1;
	swprintf(player_word1, 40, L"我是新来的保安队长");
	swprintf(player_word2, 40, L"我是库巴的朋友");
	conversation(monster_name, str, isAnswer);
	converpause(isAnswer);

	if (choose == 1)
	{
		swprintf(str, 40, L"你是故意找茬是不是，我就是保安队长，趁本大爷心情好，哪凉快哪");
		swprintf(str2, 40, L"呆着去");
	}
	else if (choose == 2)
	{
		swprintf(str, 40, L"我靠，怎么搞诈骗都搞到我头上来了，我还以为你是瞎子拉琴呢，和");
		swprintf(str2, 40, L"我瞎扯老半天。库巴大人没有你这么搓的朋友。哪凉快哪呆着去");
	}
	show();
	dconver(monster_name, str, str2, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"事先说好，你可别想着踩扁我，我和你可不在一个图层/笑");
	show();
	conversation(monster_name, str, isAnswer);
	converpause(isAnswer);

	swprintf(str, 40, L"（看来得想想办法过去了）");
	show();
	conversation(player_name, str, isAnswer);
	converpause(isAnswer);

	isfinish = 1;

	if (!isStar)
	{
		while (player_x >= 100 && bkleft >= 0)//回到左边再给一个教程
		{
			updateWithInput();
			if (player_x >= monster_left - bkleft + 200)//不能超过怪物
			{
				player_x -= 10;
			}
			updateWithoutInput();
			show();
		}
		swprintf(str, 40, L"哦，刚才忘了告诉你了，左边这个转圈的星星里面可能会有好东西哦");
		show();
		conversation(author_name, str, isAnswer);
		converpause(isAnswer);
		while (!isStar)
		{
			updateWithInput();
			if (player_x >= monster_left - bkleft + 200)//不能超过怪物
			{
				player_x -= 10;
			}
			updateWithoutInput();
			show();
		}

	}

	while (monster_i != 3 && heart)
	{
		updateWithInput();
		updateWithoutInput();
		show();
		if (!heart)
		{
			gameOver();
		}
	}
	while (player_x + 32 + 10 <= width)
	{
		updateWithInput();
		updateWithoutInput();
		show();
	}

	swprintf(str, 40, L"恭喜你成功通过第一关，接下来就是城堡里面的冒险了");
	conversation(author_name, str, isAnswer);
	converpause(isAnswer);

}

void Bef::getStar()
{
	wchar_t str[40];
	isStar = 1;
	swprintf(str, 40, L"进入了怪物的图层");
	show();
	conversation(player_name, str, isAnswer);
	converpause(isAnswer);
}


//First的一些函数
void Fir::set()
{
	start = clock();
	int i;
	loadimage(&bkground, _T("./image/First_bkground.jpg"), width, height);
	loadimage(&bride, _T("./image/bride.jpg"));
	loadimage(&thorn1, _T("./image/thorn1.jpg"));
	loadimage(&thorn2, _T("./image/thorn2.jpg"));
	loadimage(&thorn_down1, _T("./image/thorn_down1.jpg"));
	loadimage(&thorn_down2, _T("./image/thorn_down2.jpg"));
	loadimage(&mario1, _T("./image/mario1.jpg"));
	loadimage(&mario2, _T("./image/mario2.jpg"));
	loadimage(&prison1, _T("./image/prison1.jpg"));
	loadimage(&prison2, _T("./image/prison2.jpg"));


	isStar = 0;
	isBack = 0;

	//玩家
	player_x = 0;
	player_y = 32 * 6 - 58;
	isjump = 0;

	//陷阱
	track3 = 0;
	track3_y = -32;
	track4 = 0;
	track4_y = -32;

	//block设置
	for (i = 0; i < 9; i++)
	{
		block[i][3] = 1;
	}
	for (i = 0; i < 5; i++)
	{
		block[i][6] = 1;
	}
	for (i = 2; i < 9; i++)
	{
		block[i][10] = 1;
	}
	for (i = 4; i < 7; i++)
	{
		block[i][9] = 2;
	}
	for (i = 4; i < 10; i++)
	{
		block[7][i] = 1;
		block[8][i] = 1;
	}
	for (i = 0; i < 14; i++)
	{
		block[i][14] = 1;
		block[i][15] = 1;
	}
	block[9][7] = 3;
	block[9][8] = 1;
	block[11][4] = 2;
	block[11][5] = 1;
	block[11][10] = 2;
	block[11][11] = 1;

	for (i = 3; i < 14; i++)
	{
		block[12][i] = 1;
		block[13][i] = 1;
	}
	block[18][3] = 1;
	block[18][2] = 2;

	for (i = 23; i < 28; i++)
	{
		block[i][3] = 1;
	}
	for (i = 14; i < 25; i += 4)
	{
		block[i][6] = 2;
		block[i + 1][6] = 2;
		block[i + 2][6] = 3;
		block[i + 3][6] = 3;
	}
	block[30][4] = 3;
	block[30][5] = 1;
	block[31][8] = 1;
	for (i = 14; i < 28; i++)
	{
		block[i][7] = 1;
	}
	for (i = 19; i < 25; i += 4)
	{
		block[i][10] = 3;
		block[i + 1][10] = 3;
		block[i + 2][10] = 2;
		block[i + 3][10] = 2;
	}
	block[24][10] = 0;
	block[23][10] = 0;
	block[27][10] = 3;
	block[28][10] = 3;
	for (i = 16; i < 32; i++)
	{
		block[i][11] = 1;
	}
	block[14][11] = 3;
	block[14][12] = 1;

	block[14][15] = 1;
	block[15][15] = 1;
	for (i = 18; i < 28; i += 3)
	{
		block[i][15] = 1;
		block[i][14] = 2 + i % 2;
	}
	for (i = 28; i < 32; i++)
	{
		block[i][15] = 1;
	}
}

void Fir::begin() {
	wchar_t str[40];
	wchar_t str2[40];

	show();
	isAnswer = 1;
	swprintf(str, 40, L"嘿，%s！是我，马里奥", player_name);
	swprintf(player_word1, 40, L"终于找到前辈您了");
	swprintf(player_word2, 40, L"你谁啊喂");
	conversation(mario_name, str, isAnswer);
	converpause(isAnswer);

	show();
	if (choose == 1)
	{
		swprintf(str, 40, L"我太感动了，你居然会来救我 T_T");
	}
	else
	{
		swprintf(str, 40, L"怎么翻脸不认人啊喂");
	}
	conversation(mario_name, str, isAnswer);
	converpause(isAnswer);

	show();
	swprintf(str, 40, L"总之，我需要你帮我找到钥匙带回来。这里的地刺好像是4秒变一次");
	swprintf(str2, 40, L"要小心");
	dconver(mario_name, str, str2, isAnswer);
	converpause(isAnswer);
}

void Fir::show()
{
	cleardevice();
	//背景覆盖
	putimage(0, 0, &bkground);


	//如果还没拿到星星
	if (!isStar)
	{
		putimage(width - 25, height - 32 - 24, 25, 24, &star2, star_i * 25, 0, NOTSRCERASE);
		putimage(width - 25, height - 32 - 24, 25, 24, &star1, star_i * 25, 0, SRCINVERT);
	}

	//显示前辈
	if (!isBack)
	{
		putimage(0, 32, &prison2, NOTSRCERASE);
		putimage(0, 32, &prison1, SRCINVERT);
	}
	else
	{
		putimage(16, 32, &mario2, NOTSRCERASE);
		putimage(16, 32, &mario1, SRCINVERT);
	}

	//显示血量
	for (int i = 0; i < 5; i++)
	{
		putimage(i * 27, 0, &heart2, NOTSRCERASE);
		if (i < heart)
		{
			putimage(i * 27, 0, &heart1, SRCINVERT);
		}
		else
		{
			putimage(i * 27, 0, &heartempty, SRCINVERT);
		}
	}
	//显示地形
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (block[i][j] == 1)
			{
				putimage(i * 32, j * 32, &bride);
			}
			else if (block[i][j] == 2)//地刺1
			{

				if ((int)(duration * 10) % 30 == 0 && bl || (int)(duration * 10) % 30 == 1 && !bl)
				{
					putimage(i * 32, j * 32 + 20, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32 + 20, &thorn1, SRCINVERT);
				}
				else if ((int)(duration * 10) % 30 == 1 && bl || (int)(duration * 10) % 30 == 0 && !bl)
				{
					putimage(i * 32, j * 32 + 10, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32 + 10, &thorn1, SRCINVERT);
				}
				else if (bl)
				{
					putimage(i * 32, j * 32, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32, &thorn1, SRCINVERT);
				}
				else
				{
					putimage(i * 32, (j + 1) * 32, &thorn2, NOTSRCERASE);
					putimage(i * 32, (j + 1) * 32, &thorn1, SRCINVERT);
				}

			}
			else if (block[i][j] == 3)//地刺2
			{
				if ((int)(duration * 10) % 30 == 0 && !bl || (int)(duration * 10) % 30 == 1 && bl)
				{
					putimage(i * 32, j * 32 + 20, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32 + 20, &thorn1, SRCINVERT);
				}
				else if ((int)(duration * 10) % 30 == 0 && bl || (int)(duration * 10) % 30 == 1 && !bl)
				{
					putimage(i * 32, j * 32 + 10, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32 + 10, &thorn1, SRCINVERT);
				}
				else if (!bl)
				{
					putimage(i * 32, j * 32, &thorn2, NOTSRCERASE);
					putimage(i * 32, j * 32, &thorn1, SRCINVERT);
				}
				else
				{
					putimage(i * 32, (j + 1) * 32, &thorn2, NOTSRCERASE);
					putimage(i * 32, (j + 1) * 32, &thorn1, SRCINVERT);
				}
			}
		}
	}

	//陷阱3
	if (track3 && track3_y != -100)
	{
		putimage(10 * 32, track3_y, &thorn_down2, NOTSRCERASE);
		putimage(10 * 32, track3_y, &thorn_down1, SRCINVERT);
	}
	//陷阱4
	if (track4 && track4_y != -100)
	{
		putimage(15 * 32, track4_y, &thorn_down2, NOTSRCERASE);
		putimage(15 * 32, track4_y, &thorn_down1, SRCINVERT);
	}

	//显示玩家
	putimage(player_x, player_y, 32, 58, &player2, 160 + playermove * 32, 0, NOTSRCERASE);
	putimage(player_x, player_y, 32, 58, &player1, 160 + playermove * 32, 0, SRCINVERT);

	FlushBatchDraw();
	Sleep(70);
}

void Fir::updateWithInput()
{
	//左走
	if (GetAsyncKeyState(65) & 0x8000)
	{
		if (player_x % 32 >= 8)
		{
			player_x -= 8;
		}
		else if (player_x >= 32 && block[(player_x - 8) / 32][player_y / 32] != 1 && block[(player_x - 8) / 32][player_y / 32 + 1] != 1 && block[(player_x - 8) / 32][(player_y + 57) / 32] != 1)
		{
			player_x -= 8;
		}

		if (playermove == 4 || playermove == -5)//跳跃
		{
			playermove = -5;
		}
		else if (playermove == -4 || playermove >= 0)
		{
			playermove = -1;
		}
		else
		{
			playermove--;
		}
	}
	//右走
	else if (GetAsyncKeyState(68) & 0x8000)
	{
		if (player_x % 32 <= 32 - 8)
		{
			if (player_x % 32 == 0)
			{
				if (player_x / 32 + 1 < 32 && block[player_x / 32 + 1][player_y / 32] != 1 && block[player_x / 32 + 1][player_y / 32 + 1] != 1 && block[player_x / 32 + 1][(player_y + 57) / 32] != 1)
				{
					player_x += 8;
				}
			}
			else
			{
				if (block[player_x / 32 + 1][player_y / 32] != 1 && block[player_x / 32 + 1][(player_y + 57) / 32] != 1)
				{
					player_x += 8;
				}
			}
		}
		else if (player_x <= 32 * 15 && block[(player_x + 32 + 8) / 32][player_y / 32] != 1 && block[(player_x + 32 + 8) / 32][(player_y + 57) / 32] != 1)
		{
			player_x += 8;
		}

		if (playermove == 4 || playermove == -5)
		{
			playermove = 4;
		}
		else if (playermove == 3 || playermove < 0)
		{
			playermove = 0;
		}
		else
		{
			playermove++;
		}
	}
	else if (playermove != 4 && playermove != -5)//走路姿势复位
	{
		if (playermove >= 0)
		{
			playermove = 0;
		}
		else
		{
			playermove = -1;
		}
	}

	//跳跃
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && isjump == 0)
	{
		//跳动
		mciSendString(L"close jpmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
		mciSendString(L"open jump.mp3 alias jpmusic", NULL, 0, NULL); // 打开跳动音乐
		mciSendString(L"play jpmusic", NULL, 0, NULL); // 仅播放一次
		isjump = 1;
		jumpup = 12;
		if (playermove >= 0)
		{
			playermove = 4;
		}
		else
		{
			playermove = -5;
		}
	}
}

void Fir::updateWithoutInput()
{
	//陷阱1
	if (player_x >= 5 * 32 && player_x <= 9 * 32 && player_y >= 10 * 32 && block[6][14] != 0)
	{
		for (int i = 6; i < 10; i++)
		{
			block[i][14] = 0;
			block[i][15] = 2;
		}
	}
	else if (block[6][14] == 0 && (player_x < 5 * 32 || player_x>9 * 32 || player_y < 10 * 32))
	{
		for (int i = 6; i < 10; i++)
		{
			block[i][14] = 1;
			block[i][15] = 1;
		}
	}

	//陷阱2
	if (player_x >= 5 * 32 && player_x <= 6 * 32 && player_y <= 3 * 32 && block[5][3] != 0)
	{
		block[5][3] = 0;
		block[6][3] = 0;
		block[5][0] = 1;
		block[6][0] = 1;
	}
	else if (!(player_x >= 5 * 32 && player_x <= 6 * 32 && player_y <= 3 * 32) && block[5][3] == 0)
	{
		block[5][3] = 1;
		block[6][3] = 1;
		block[5][0] = 0;
		block[6][0] = 0;
	}



	//陷阱3
	if (!track3 && player_x >= 10 * 32 - 16 && player_x <= 11 * 32 && player_y >= 12 * 32 && player_y <= 13 * 32)
	{
		track3 = 1;
		track3_y = -32;
	}
	else if (track3 && track3_y != -50)
	{
		if (track3_y > height)//一次性
		{
			track3_y = -100;
		}
		else if (track3_y + 32 >= player_y && track3_y + 32 <= player_y + 50 && player_x < 11 * 32 && player_x > 9 * 32)
		{
			hurt();
		}
		track3_y += 50;//下落
	}
	//陷阱4
	if (!track4 && player_x >= 14 * 32 - 8 && player_x <= 15 * 32)
	{
		track4 = 1;
		track4_y = -32;
	}
	else if (track4 && track4_y != -50)
	{
		if (track4_y > height)//一次性
		{
			track4_y = -100;
		}
		else if (track4_y + 32 >= player_y && track4_y + 32 <= player_y + 50 && player_x < 11 * 32 && player_x >= 9 * 32)
		{
			hurt();
		}
		track4_y += 50;//下落
	}



	//地刺
	end = clock();
	duration = (double)(end - start) / CLK_TCK;
	if (duration >= 3)
	{
		start = clock();
		bl = 1 - bl;
	}

	//星星
	if (isStar == 0 && player_x + 32 > width - 25 && player_y + 58 >= height - 32 - 25)
	{
		isStar = 1;
		wchar_t str[] = L"获得了钥匙🔑";
		conversation(player_name, str, isAnswer);
		converpause(isAnswer);
	}
	else if (!isStar)
	{
		if (star_i == 7)
		{
			star_i = 0;
		}
		else
		{
			star_i++;
		}
	}

	//跳跃及下落
	if (isjump && jumpup)//跳跃
	{
		//if ((player_y - 8) < 0)//撞天花板
		//{
		//	jumpup = 0;
		//}
		//else 
		if (player_x % 32 == 0 && block[player_x / 32][(player_y - 8) / 32] != 1)
		{
			player_y -= 8;
			jumpup -= 1;
		}
		else if (block[player_x / 32][(player_y - 8) / 32] == 1 || block[player_x / 32 + 1][(player_y - 8) / 32])//撞墙
		{
			jumpup = 0;
		}
		else
		{
			player_y -= 8;
			jumpup -= 1;
		}
	}
	else if (player_x / 32 + 1 <= 32 && player_y / 32 + 2 < 16)//下落
	{
		if (player_x / 32 + 1 != 32 && block[player_x / 32][(player_y + 58) / 32] != 1 && block[player_x / 32 + 1][(player_y + 58) / 32] != 1)
		{
			player_y += 8;
		}
		else if (player_x % 32 == 0 && block[player_x / 32][(player_y + 58) / 32] != 1)
		{
			player_y += 8;
		}
		else
		{
			isjump = 0;
			if (playermove == 4)
			{
				playermove = 0;
			}
			else if (playermove == -5)
			{
				playermove = -1;
			}
		}
	}
	else if ((player_y + 58) / 32 + 2 >= 16)
	{
		player_y += 8;
		if (player_y / 32 >= 16)//掉下去
		{
			hurt();
		}
	}
	//地刺判定
	if (block[player_x / 32][(player_y + 57) / 32] >= 2)
	{
		if (block[player_x / 32][(player_y + 57) / 32] == 2 && bl && (int)(duration * 10) % 30 > 1)
		{
			hurt();
		}
		else if (block[player_x / 32][(player_y + 57) / 32] == 3 && !bl && (int)(duration * 10) % 30 > 1)
		{
			hurt();
		}
	}
	else if (block[player_x / 32][player_y / 32 + 1] >= 2)
	{
		if (block[player_x / 32][player_y / 32 + 1] == 2 && bl && (int)(duration * 10) % 30 > 1)
		{
			hurt();
		}
		else if (block[player_x / 32][player_y / 32 + 1] == 3 && !bl && (int)(duration * 10) % 30 > 1)
		{
			hurt();
		}
	}
	else if (player_x % 32 != 0)
	{
		if (block[player_x / 32 + 1][player_y / 32 + 1] >= 2)
		{
			if (block[player_x / 32 + 1][player_y / 32 + 1] == 2 && bl && (int)(duration * 10) % 30 > 1)
			{
				hurt();
			}
			else if (block[player_x / 32 + 1][player_y / 32 + 1] == 3 && !bl && (int)(duration * 10) % 30 > 1)
			{
				hurt();
			}
		}
		else if (block[player_x / 32 + 1][(player_y + 57) / 32] >= 2)
		{
			if (block[player_x / 32 + 1][(player_y + 57) / 32] == 2 && bl && (int)(duration * 10) % 30 > 1)
			{
				hurt();
			}
			else if (block[player_x / 32][(player_y + 57) / 32] == 3 && !bl && (int)(duration * 10) % 30 > 1)
			{
				hurt();
			}
		}
	}
}

void Fir::hurt()
{
	//背景音乐
	mciSendString(L"close bkmusic", NULL, 0, NULL);//关闭背景音乐

	//扣血
	mciSendString(L"close deadmusic", NULL, 0, NULL);//关闭扣血音乐
	mciSendString(L"open dead.mp3 alias deadmusic", NULL, 0, NULL); // 打开扣血
	mciSendString(L"play deadmusic", NULL, 0, NULL); // 仅播放一次

	Sleep(500);
	int i = 3;
	while (player_y <= height)
	{
		if (i > 0)
		{
			player_y -= 20;
			i--;
		}
		else
		{
			player_y += 30;
		}
		show();
		Sleep(10);
	}

	heart--;

	if (heart)
	{
		mciSendString(L"open bkmusic.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
		mciSendString(L"play bkmusic repeat", NULL, 0, NULL);  // 循环播放
	}

	if (isStar)//复位角色
	{
		player_x = 31 * 32;
		player_y = 14 * 32 - 58;
	}
	else
	{
		player_x = 0;
		player_y = 32 * 6 - 58;
	}

	//复位陷阱
	track3_y = -32;
	track3 = 0;
	track4_y = -32;
	track4 = 0;
}