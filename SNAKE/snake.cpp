#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#pragma warning(disable:4996)// 去除警告
#pragma warning(disable:28159)// 去除警告
#pragma warning(disable:4018)// 去除警告
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768// 窗口宽高
#define MAX_SHAKE 32767// 定义一个足够大的最大长度
#define MAX_SNAKE 100// 定义一个足够大的数量
const int RUSH = 30;
const int NORMAL = 60;
const int INTERVAL = 500;
const double hatred = 1.0;
int conf[100] = {0};
int ainum = 5;
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
	int live[50];// 决定蛇的存活
	int leng[50];// 蛇的长度节，决定蛇身由多少圆形构成
	int dir[50];// 蛇的方向
	int score[50];// 成绩
	int rsize[50];// 尺寸
	int speed[50];
	int r[50] = {0};
	int g[50] = {0};
	int b[50] = {0};// 定义颜色
	POINT coor[MAX_SHAKE][50];// 位置

}snake;
struct Food_tlg// 食物的结构体
{
	POINT fd[101];
	int flag[100];
	int rsize;
	DWORD color[100];
}food;
void SnakeKill(int i) {
	snake.leng[i] = 0;
}
void SnakeDraw(int j) {
	for (int i = 0; i < snake.leng[j]; i++) {
		setfillcolor(RGB(snake.r[j], snake.g[j], snake.b[j]));// 填充
		if (i != 0) {
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 10);
		}
		else {
			setfillcolor(RGB(166, 75, 98));
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 12);
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 2);
		}// 标记头
	}
}
bool WallJudge(int i, int j) {
	if (WIN_WIDTH - snake.coor[0][i].x <= j || snake.coor[0][i].x <= j)
		return true;
	if (WIN_HEIGHT - snake.coor[0][i].y <= j || snake.coor[0][i].y <= j)
		return true;
	return false;
}
double Dcount(POINT n1, POINT n2) {
	int d;
	d = (n1.x - n2.x) ^ 2 + (n1.y - n2.y) ^ 2;
	return d;
}
void Offdir(int i) {
	int wait = 0;
	wait += 10;
	if(wait>=30 || WallJudge(i,15)==true){
		switch (snake.dir[i])
		{
		case UP:snake.dir[i] = DOWN;
			break;
		case DOWN:snake.dir[i] = UP;
			break;
		case LEFT:snake.dir[i] = RIGHT;
			break;
		case RIGHT:snake.dir[i] = LEFT;
			break;
		default:
			break;
		}
		wait = 0;
	}

}
bool HitJudge_Eat(POINT n1, POINT n2) {
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
bool HitJudge_Ru(POINT n1, POINT n2) {
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
void SnakeMove(int j){
	for (int i = snake.leng[j] - 1; i > 0; i--)
	{
		snake.coor[i][j].x = snake.coor[i - 1][j].x;
		snake.coor[i][j].y = snake.coor[i - 1][j].y;
	}// 蛇身继承运动
	switch (snake.dir[j])
	{
	case UP:snake.coor[0][j].y -= 10;
		break;
	case DOWN:snake.coor[0][j].y += 10;
		break;
	case LEFT:snake.coor[0][j].x -= 10;
		break;
	case RIGHT:snake.coor[0][j].x += 10;
		break;
	default:
		break;
	}//移动
}
void EatFood(int j){
	for (int i = 0; i <= 100; i++) {
		if (HitJudge_Eat(food.fd[i], snake.coor[0][j]) == true && food.flag[i] == 1 && 
			snake.leng[j]!=0)//暂时使用这样的碰撞检测
		{
			snake.leng[j]++;
			snake.score[j] += 10;
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
	for (int j = 0; j <= ainum; j++) {
		for (int i = 1; i <= snake.leng[0] - 1; i++)
		{
			if (HitJudge_Ru(snake.coor[0][j], snake.coor[i][j]) == true || WallJudge(j, 5)) 
			{
				if(j==0){
			    snake.live[0]=0;
				SnakeKill(0);
				outtextxy(512, 384, "Game Over!");				
				Sleep(1000);
				_getch();
				exit(233333);}
				
			}
			if (HitJudge_Ru(snake.coor[0][j], snake.coor[i][j]) == true || WallJudge(j, 5)) {
				snake.live[j] = 0;
				SnakeKill(j);
			}
		}
	}
}
void AppearAI() {
	srand(GetTickCount());
	int rand_x = 0;
	int rand_y = 0;
	for(int j=1;j<=ainum;j++)
	{   
		rand_x = rand() % WIN_WIDTH;
		rand_y = rand() % WIN_HEIGHT;
		conf[j] = 1;
		snake.leng[j] = 3;
		snake.dir[j] = LEFT;// 暂定左方向
		snake.score[j] = 0;
		snake.rsize[j] = 10;
		snake.speed[j] = NORMAL+25;
		snake.coor[2][j].x = rand_x + snake.rsize[0] * 2;
		snake.coor[2][j].y = rand_y;
		snake.coor[1][j].x = rand_x + snake.rsize[0];
		snake.coor[1][j].y = rand_y;
		snake.coor[0][j].x = rand_x;
		snake.coor[0][j].y = rand_y;
		snake.r[j] = rand() % 256;
		snake.g[j] = rand() % 256;
		snake.b[j] = rand() % 256;
		snake.live[j] = 1;
		SnakeDraw(j);
	}// AI对象初始化

}
void AIhitSure() {
	int wait = 0;
	for (int i = 1; i <= ainum; i++) {
		for (int j = 1; j <= ainum; j++)
		{
			for(int k =1;k<=snake.leng[j]-1;k++){
				if (i != j) {
					if (Dcount(snake.coor[0][i], snake.coor[k][j]) <= 400)
						Offdir(i);
				}
			}
		}
		if (WallJudge(i,30) == true) {
			Offdir(i);
		}
	}
}
void AIJudgeDir(int j){
	double min;
	double d;
	int f,dx,dy;
	f = 0;
	min = Dcount(snake.coor[0][0],snake.coor[0][j]);
	for (int i = 1; i <= 100; i++)
	{
		d = hatred * Dcount(snake.coor[0][j],food.fd[i]);
		if (min > d) {
			min = d;
			f = i;
		}
	}
	if (f == 0) {
		dx = snake.coor[1][0].x - snake.coor[0][j].x;
		dy = snake.coor[1][0].y - snake.coor[0][j].y;
		if (abs(dx) >= abs(dy)) {
			if (abs(dx) >= abs(dy)) {
				if (dx <= 0)
				{
					if (snake.dir[j] != RIGHT)
						snake.dir[j] = LEFT;
				}
				else
				{
					if (snake.dir[j] != LEFT)
						snake.dir[j] = RIGHT;
				}
			}
			else
			{
				if (dy <= 0)
				{
					if (snake.dir[j] != DOWN)
						snake.dir[j] = UP;
				}
				else
				{
					if (snake.dir[j] != UP)
						snake.dir[j] = DOWN;
				}
			}
		}
	}
	else {
		dx = food.fd[f].x - snake.coor[0][j].x;
		dy = food.fd[f].y - snake.coor[0][j].y;
		if (abs(dx) >= abs(dy)) {
			if (dx <= 0)
			{
				if (snake.dir[j] != RIGHT)
					snake.dir[j] = LEFT;
			}
			else
			{
				if (snake.dir[j] != LEFT)
					snake.dir[j] = RIGHT;
			}
		}
		else
		{
			if (dy <= 0)
			{
				if (snake.dir[j] != DOWN)
					snake.dir[j] = UP;
			}
			else
			{
				if (snake.dir[j] != UP)
					snake.dir[j] = DOWN;
			}
		}
	}
	AIhitSure();
 }
void AIconf() {
	for (int j = 1; j <= ainum; j++) 
	{
		AIJudgeDir(j);
		SnakeMove(j);
		EatFood(j);
	}
}
void Gameinit() {
	srand(GetTickCount());
	int i = 0;
	/* 初始化蛇 */
	conf[0] = 1;
	snake.leng[0] = 3;
	snake.dir[0] = LEFT;// 暂定左方向
	snake.score[0] = 0;
	snake.rsize[0] = 10;
	snake.speed[0] = NORMAL;
	snake.coor[2][0].x = WIN_WIDTH / 2 + snake.rsize[0] * 2;
	snake.coor[2][0].y = WIN_HEIGHT / 2;
	snake.coor[1][0].x = WIN_WIDTH / 2 + snake.rsize[0];
	snake.coor[1][0].y = WIN_HEIGHT;
	snake.coor[0][0].x = WIN_WIDTH / 2;
	snake.coor[0][0].y = WIN_HEIGHT / 2;
	snake.r[0] = 66;
	snake.g[0] = 75;
	snake.b[0] = 98;
	snake.live[0] = 1;
	/* 初始化蛇结束 */

	/* 初始化食物 */
	food.fd[i].x = rand() % WIN_WIDTH;
	food.fd[i].y = rand() % WIN_HEIGHT;
	food.flag[i] = 1;
	food.rsize = 5;
	food.color[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
	/* 初始化食物结束 */

	/* 初始化AI蛇 */
	AppearAI();
	/* 初始化AI蛇结束 */
}
void GameDraw() {
	setbkcolor(RGB(106, 170, 150));// 背景颜色设置
	cleardevice();
	/* 画蛇 */
	for (int j = 0; j <= 100; j++) {
		if (conf[j] == 1)
			SnakeDraw(j);
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
int main() {
	int wait = 0;
	int waitf = 0;
	srand(GetTickCount());
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);// 初始化窗口
	Gameinit();
	DWORD t1, t2,t3,t4;
	t1 = t2 = t3 = t4 = GetTickCount();
	BeginBatchDraw();
	while (1) {
		if ((t2 - t1) > snake.speed[0]) {
			SnakeMove(0);
			t1 = t2;
		}
		t2= GetTickCount();
		for (int j = 1; j <= ainum; j++) {
			if ((t4 - t3) > snake.speed[j]) {
				AIconf();
				t3 = t4;
			}
		}
		t4 = GetTickCount();
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
		EatFood(0);
		KeyControl();
		DeathJudge();
	}
	getchar();
	closegraph();
	return 0;
}