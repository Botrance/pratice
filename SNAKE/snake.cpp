#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768 // 窗口宽高
#define MAX_SHAKE 32767// 定义一个足够大的最大长度
#define MAX_SNAKE 100// 定义一个足够大的数量
const int RUSH = 30;
const int NORMAL = 60;
const int INTERVAL = 1000;
enum DIR // 蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct Snake_tlg // 蛇的结构体
{
public :
	int conf[50];// 决定是玩家操控还是电脑操控
	int leng[50];// 蛇的长度节，决定蛇身由多少圆形构成
	int dir[50];// 蛇的方向
	int score[50];// 成绩
	int rsize[50];// 尺寸
	int speed[50];
	POINT coor[MAX_SHAKE][50];// 位置

}snake;
struct Food_tlg// 食物的结构体
{
	POINT fd[100];
	int flag[100];
	int rsize;
	DWORD color[100];
}food;
void Gameinit() {
	srand(GetTickCount());
	int i = 0;
	/* 初始化蛇 */
	snake.leng[0] = 3;
	snake.dir[0] = LEFT;// 暂定左方向
	snake.score[0] = 0;
	snake.rsize[0] = 10;
	snake.speed[0] = NORMAL;
	snake.coor[2][0].x = WIN_WIDTH/2 + snake.rsize[0] *2;
	snake.coor[2][0].y = WIN_HEIGHT/2;
	snake.coor[1][0].x = WIN_WIDTH/2 + snake.rsize[0];
	snake.coor[1][0].y = WIN_HEIGHT;
	snake.coor[0][0].x = WIN_WIDTH / 2;
	snake.coor[0][0].y = WIN_HEIGHT/2;
	/* 初始化蛇结束 */

	/* 初始化食物 */
	food.fd[i].x = rand() % WIN_WIDTH;
	food.fd[i].y = rand() % WIN_HEIGHT;
	food.flag[i] = 1;
	food.rsize = 5;
	food.color[i] = RGB(rand() % 256,rand() % 256,rand() % 256);
	/* 初始化食物结束 */
}
void GameDraw() {
	setbkcolor(RGB(106, 170, 150));// 背景颜色设置
	cleardevice();
	/* 画蛇 */
	for (int i = 0; i < snake.leng[0]; i++) {
		setfillcolor(RGB(66, 75, 98));
		if (i != 0) {
			fillcircle(snake.coor[i][0].x, snake.coor[i][0].y, 10);
		}
		else {
			setfillcolor(RGB(166, 75, 98));
			fillcircle(snake.coor[i][0].x, snake.coor[i][0].y, 12);
			fillcircle(snake.coor[i][0].x, snake.coor[i][0].y, 2);
		}// 标记头
	}
	/* 画蛇结束 */

	/* 画食物 */
	for (int i = 0; i <= 100; i++) {
		if (food.flag[i] == 1) {
			setfillcolor(food.color[i]);
			fillcircle(food.fd[i].x, food.fd[i].y, food.rsize);
		}
	}
	
	/* 画食物结束 */

	/* 显示分数开始 */
	char tmp[20] = "";
	sprintf(tmp, "分数:%d", snake.score[0]);
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, tmp);
	/* 显示分数结束 */

}
bool HitJudge(POINT n1, POINT n2) {
	int dsize = (snake.rsize[0] - food.rsize)+3;
	if (n1.x - dsize <= n2.x && n2.x <= n1.x + dsize &&
		n1.y - dsize <= n2.y && n2.y <= n1.y + dsize)// 无奈之举
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool HitJudge_ss(POINT n1, POINT n2) {
	int dsize = 3;
	if (n1.x - dsize <= n2.x && n2.x <= n1.x + dsize &&
		n1.y - dsize <= n2.y && n2.y <= n1.y + dsize)// 无奈之举
	{
		return true;
	}
	else
	{
		return false;
	}
}
void AppearFood(){
	int i ;
	srand(GetTickCount());
	i = rand() % 100;
	if (food.flag[i] != 1) {
		food.flag[i] = 1;
		food.color[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.fd[i].x = rand() % WIN_WIDTH;
		food.fd[i].y = rand() % WIN_HEIGHT;
	}
}
void SnakeMove(){
	for (int i = snake.leng[0] - 1; i > 0; i--)
	{
		snake.coor[i][0].x = snake.coor[i - 1][0].x;
		snake.coor[i][0].y = snake.coor[i - 1][0].y;
	}// 蛇身继承运动
	switch (snake.dir[0])
	{
	case UP:snake.coor[0][0].y -= 10;
		break;
	case DOWN:snake.coor[0][0].y += 10;
		break;
	case LEFT:snake.coor[0][0].x -= 10;
		break;
	case RIGHT:snake.coor[0][0].x += 10;
		break;


	default:
		break;
	}//移动
}
void EatFood(){
	for (int i = 0; i <= 100; i++) {
		if (HitJudge(food.fd[i], snake.coor[0][0]) == true && food.flag[i] == 1)//暂时使用这样的碰撞检测
		{
			snake.leng[0]++;
			snake.score[0] += 10;
			food.flag[i] = 0;
		}
	}
}
void KeyControl(){
	if (GetAsyncKeyState(VK_UP) && snake.dir[0] != DOWN) {
		snake.dir[0] = UP;
	}
	if (GetAsyncKeyState(VK_DOWN) && snake.dir[0] != UP) {
		snake.dir[0] = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) && snake.dir[0] != RIGHT) {
		snake.dir[0] = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) && snake.dir[0] != LEFT) {
		snake.dir[0] = RIGHT;
	}
	if (GetAsyncKeyState(VK_SPACE)) {
		snake.speed[0] = RUSH; // 加速
	}
}// 使用了winAPI
void DeathJudge(){
	for (int i = 1; i <= snake.leng[0] - 1; i++) {
		if (HitJudge_ss(snake.coor[0][0], snake.coor[i][0]) == true) {
			outtextxy(512, 384, "Game Over!");
			_getch();
			exit(233333);
		}
	}
}
int main() {
	int wait = 0;
	int waitf = 0;
	srand(GetTickCount());
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);// 初始化窗口
	Gameinit();
	DWORD t1, t2;
	t1 = t2 = GetTickCount();
	BeginBatchDraw();
	while (1) {
		if ((t2 - t1) > snake.speed[0]) {
			SnakeMove();
			t1 = t2;
		}
		t2= GetTickCount();
		FlushBatchDraw();
		if (snake.speed[0] == RUSH){
			wait += 10;
				if (wait >= 100) {
					snake.speed[0] = NORMAL;
					wait = 0;// 速度回复
				}
		}
		waitf += rand() % 10;
			if (waitf >= INTERVAL) {
				AppearFood();
				waitf = 0;
			}
		GameDraw();
		EatFood();
		KeyControl();
		DeathJudge();
	}
	

	getchar();
	closegraph();
	return 0;
}