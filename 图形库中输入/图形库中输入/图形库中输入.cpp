// 编译环境：VC2008 及以上版本，Unicode 字符集
//
#include <graphics.h>
#include <conio.h>

int main()
{
	// 初始化绘图窗口
	initgraph(640, 480);

	// 定义字符串缓冲区，并接收用户输入
	wchar_t s[10];
	InputBox(s, 10, L"请输入半径");

	// 将用户输入转换为数字
	int r = _wtoi(s);

	// 画圆
	circle(320, 240, r);

	// 按任意键退出
	_getch();
	closegraph();

	return 0;
}