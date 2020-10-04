#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768 // 窗口宽高
#define MAX_SHAKE 32767// 定义一个足够大的最大长度
enum DIR // 蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct Snake_tlg // 蛇的结构体
{
	int conf;// 决定是玩家操控还是电脑操控
	int leng;// 蛇的长度节，决定蛇身由多少圆形构成
	int dir;// 蛇的方向
	int score;// 成绩
	int rsize;// 尺寸
	POINT coor[MAX_SHAKE];// 位置

}snake;
struct Food_tlg// 食物的结构体
{
	POINT fd;
	int flag;
	int size;
	DWORD color;
}food;
void Gameinit() {
	srand(GetTickCount());
	/* 初始化蛇 */
	snake.leng = 3;
	snake.dir = LEFT;// 暂定左方向
	snake.score = 0;
	snake.rsize = 10;
	snake.coor[2].x = 512 + snake.rsize*2;
	snake.coor[2].y = 384;
	snake.coor[1].x = 512 + snake.rsize;
	snake.coor[1].y = 384;
	snake.coor[0].x = 512;
	snake.coor[0].y = 384;
	/* 初始化蛇结束 */

	/* 初始化食物 */
	food.fd.x = rand() % WIN_WIDTH;
	food.fd.y = rand() % WIN_HEIGHT;
	food.flag = 1;
	food.size = 12;
	food.color = RGB(rand() % 256,rand() % 256,rand() % 256);
	/* 初始化食物结束 */
}
void GameDraw() {
	setbkcolor(RGB(106, 170, 150));// 背景颜色设置
	cleardevice();
	/* 画蛇 */
	for (int i = 0; i < snake.leng; i++) {
		setfillcolor(RGB(66, 75, 98));
		if (i != 0) {
			fillcircle(snake.coor[i].x, snake.coor[i].y, 10);
		}
		else {
			setfillcolor(RGB(166, 75, 98));
			fillcircle(snake.coor[i].x, snake.coor[i].y, 12);
			fillcircle(snake.coor[i].x, snake.coor[i].y, 2);
		}// 标记头
	}
	/* 画蛇结束 */

	/* 画食物 */
	if (food.flag == 1) {
     setfillcolor(food.color);
	 fillrectangle(food.fd.x, food.fd.y, food.fd.x + food.size, food.fd.y + food.size);
	}
	
	/* 画食物结束 */

	/* 显示分数开始 */
	char tmp[20] = "";
	sprintf(tmp, "分数:%d", snake.score);
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, tmp);
	/* 显示分数结束 */

}
void SnakeMove(){
	for (int i = snake.leng - 1; i > 0; i--)
	{
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}// 蛇身继承运动
	switch (snake.dir)
	{
	case UP:snake.coor[0].y -= 10;
		break;
	case DOWN:snake.coor[0].y += 10;
		break;
	case LEFT:snake.coor[0].x -= 10;
		break;
	case RIGHT:snake.coor[0].x += 10;
		break;


	default:
		break;
	}//移动
}
void SnakeRush(){}
void EatFood(){
	if ( HitJudge(food.fd,snake.coor[0]) && food.flag == 1)//暂时使用这样的碰撞检测
	{
		snake.leng++;
		snake.score += 10;
		food.flag = 0;
	}
	if (food.flag == 0) {
		food.fd.x = rand() % WIN_WIDTH;
		food.fd.y = rand() % WIN_HEIGHT;
		food.flag = 1;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}
void KeyControl(){
	if (GetAsyncKeyState(VK_UP) && snake.dir != DOWN) {
		snake.dir = UP;
	}
	if (GetAsyncKeyState(VK_DOWN) && snake.dir != UP) {
		snake.dir = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) && snake.dir != RIGHT) {
		snake.dir = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake.dir != LEFT) {
		snake.dir = RIGHT;
	}
	}// 使用了winAPI
void DeathJudge() {
	for (int i = 1; i <= snake.leng - 1; i++) {
		if (HitJudge(snake.coor[0], snake.coor[i]) == true) {
			outtextxy(512, 384, "Game Over!");
			_getch();
			exit(233333);
		}
	}
}
bool HitJudge(POINT n1, POINT n2) {

}
int main() {
	
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);// 初始化窗口
	Gameinit();
	while (1) {
		GameDraw();
		SnakeMove();
		EatFood();
		KeyControl();
		DeathJudge();
		Sleep(50);
	}
	

	getchar();
	closegraph();
	return 0;
}