#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")//已导入静态库，可写可不写
#include <iostream>

int main()
{
	//模板mciSendString(操作指令, 获取返回的信息(字符串), 返回信息的长度, 窗口句柄(一般为NULL));
	mciSendString("open kk.mp3 alias music", 0, 0, 0);//alias是别名的意思，下面就可以去直接用music来代替kk.mp3
	mciSendString("play music", 0, 0, 0);//播放音乐
}