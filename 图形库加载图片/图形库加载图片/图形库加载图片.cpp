//头文件
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>//清屏函数等
#include <time.h>//clock函数
#include <conio.h>//输入
#include <graphics.h>//图形库
int main()
{	
// 绘图窗口初始化
	initgraph(640, 480);

	// 读取图片至绘图窗口
	loadimage(NULL, _T("D:\\test.jpg"));

	// 按任意键退出
	_getch();
	closegraph();

	return 0;
}