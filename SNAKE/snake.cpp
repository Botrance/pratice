#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#pragma warning(disable:4996)// ȥ������
#pragma warning(disable:28159)// ȥ������
#pragma warning(disable:4018)// ȥ������
#pragma warning(disable:6031)// ȥ������
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768// ���ڿ��
#define MAX_SHAKE 32767// ����һ���㹻�����󳤶�
#define MAX_SNAKE 100// ����һ���㹻�������
const int RUSH = 30;// ����ٶ�
const int NORMAL = 60;// ��׼�ٶ�
const int INTERVAL = 600;// ʳ����ּ��
const int AIINTERVAL = 100;// AI����ı���
const double hatred = 1;// ���ֵ
const int fnum = 500;// ʳ��������
const int ainum = 10;// AI����
int conf[100] = {0};// ȷ���Ƿ��п���
int lifenum = 0;// ʣ���߸���
int waitAI = 0;// AI�߸ı䷽�򻺳�
int px = WIN_WIDTH/2;// У׼����
int py = WIN_HEIGHT/2;// У׼����
enum DIR // �ߵķ���
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
struct Snake_tlg // �ߵĽṹ��
{
public :
	int conf[50];// ��������Ҳٿػ��ǵ��Բٿ�
	int life[50];// �����ߵĴ��
	int leng[50];// �ߵĳ��Ƚڣ����������ɶ���Բ�ι���
	int dir[50];// �ߵķ���
	int score[50];// �ɼ�
	int rsize[50];// �ߴ�
	int speed[50];// �ٶ�
	int r[50] = {0};
	int g[50] = {0};
	int b[50] = {0};// ������ɫ
	POINT coor[MAX_SHAKE][50];// λ��

}snake;
struct Food_tlg// ʳ��Ľṹ��
{
	POINT fd[fnum];
	int flag[fnum];
	int rsize;
	DWORD color[fnum];// ʳ���������
}food;
DIR rands(DIR A,DIR B){
	int x = 0;
	srand(GetTickCount());
	x = rand() % 1;
	if (x == 0) { return A; }
	if (x == 1) { return B; }
}
void SnakeKill(int i) {
	snake.leng[i] = 0;
	snake.life[i] = 0;
	if (conf[i] == 1 && snake.life[i] == 0) {
		conf[i] = 0;

		lifenum -= 1;
	}// ɱ���ߣ��������Ʊ����Ϊ0
}
void SnakeDraw(int j) {
	for (int i = 0; i < snake.leng[j]; i++) {
		setfillcolor(RGB(snake.r[j], snake.g[j], snake.b[j]));// ���
		if (i != 0) {
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 10);
		}
		else {
			setfillcolor(RGB(166, 75, 98));
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 12);
			fillcircle(snake.coor[i][j].x, snake.coor[i][j].y, 2);
		}// ���ͷ
	}
}
bool WallJudge(int i, int j) {
	if (px + 0.5 * (WIN_WIDTH) - snake.coor[0][i].x  <= j || snake.coor[0][i].x - (px - 0.5 * (WIN_WIDTH)) <= j)
		return true;
	if (py + 0.5 * (WIN_HEIGHT) - snake.coor[0][i].y  <= j || snake.coor[0][i].y - (py - 0.5 * (WIN_HEIGHT)) <= j)
		return true;
	return false;// �ж��Ƿ�ײǽ
}
double Dcount(POINT n1, POINT n2) {
	int d;
	d = (n1.x - n2.x) ^ 2 + (n1.y - n2.y) ^ 2;
	return d;// ��������ƽ��
}
void Offdir(int i) {
	if (WallJudge(i, 60) != true && waitAI >= AIINTERVAL)
	{
		waitAI = 0;
		switch (snake.dir[i])
	 {
	case UP:snake.dir[i] = rands(RIGHT, LEFT);
		break;
	case DOWN:snake.dir[i] = rands(RIGHT, LEFT);
		break;
	case LEFT:snake.dir[i] = rands(UP, DOWN);
		break;
	case RIGHT:snake.dir[i] = rands(UP, DOWN);
		break;
	default:
		break;
     }
	}
	else if(WallJudge(i, 50) == true) {
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
	}

	// �ı䷽��

}
bool HitJudge_Eat(POINT n1, POINT n2) {
	int dsize = (snake.rsize[0] - food.rsize)+3;
	if (n1.x - dsize <= n2.x && n2.x <= n1.x + dsize &&
		n1.y - dsize <= n2.y && n2.y <= n1.y + dsize)// ����֮��
	{
		return true;
	}
	else
	{
		return false;
	}

}// ��ʳ�����ײ���
bool HitJudge_Ru(POINT n1, POINT n2) {
	int dsize =5 ;
	if (n1.x - dsize <= n2.x && n2.x <= n1.x + dsize &&
		n1.y - dsize <= n2.y && n2.y <= n1.y + dsize)// ����֮��
	{
		return true;
	}
	else
	{
		return false;
	}
}// ��֮����ײ���
void AppearFood(){
	int i ;
	srand(GetTickCount());
	i = rand() % fnum;
	if (food.flag[i] != 1) {
		food.flag[i] = 1;
		food.color[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.fd[i].x = rand() % WIN_WIDTH;
		food.fd[i].y = rand() % WIN_HEIGHT;
	}
}// �������ʳ��
void SnakeMove(int j){
	for (int i = snake.leng[j] - 1; i > 0; i--)
	{
			snake.coor[i][j].x = snake.coor[i - 1][j].x;
			snake.coor[i][j].y = snake.coor[i - 1][j].y;

	}// ����̳��˶�
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
	}//�ƶ�
}
void EatFood(int j){
	for (int i = 0; i < fnum; i++) {
		if (HitJudge_Eat(food.fd[i], snake.coor[0][j]) == true && food.flag[i] == 1 && 
			snake.leng[j]!=0)//��ʱʹ����������ײ���
		{
			snake.leng[j]++;
			snake.score[j] += 10;
			food.flag[i] = 0;
		}
	}// ��ʳ
}
void KeyControl(){
	if (GetAsyncKeyState(VK_UP)) {
		snake.dir[0] = UP;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		snake.dir[0] = DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		snake.dir[0] = LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		snake.dir[0] = RIGHT;
	}
	if (GetAsyncKeyState(VK_SPACE)) {
		snake.speed[0] = RUSH; // ����
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{exit(233333); }
}// ʹ����winAPI
void DeathJudge(){
	for (int j = 0; j <= ainum; j++) {
		for (int k = 0; k <= ainum; k++) 
		{
		 for (int i = 1; i <= snake.leng[0] - 1; i++)
		 { if(j !=k){	
			 if (HitJudge_Ru(snake.coor[i][k], snake.coor[0][j]) == true || WallJudge(j, 5))// ��ײ
			{
			      if (j == 0){					
					snake.life[0] = 0;
					SnakeKill(0);
				  }// ��Ϸ����
				  else{ 

					  SnakeKill(j);// AI������
				  }


			}
		 }
		
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
		snake.dir[j] = LEFT;
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
		snake.life[j] = 1;
		SnakeDraw(j);
	}// AI�����ʼ��

}
void AIhitSure(int j) {
		for (int i= 1; i <= ainum; i++)
		{
			for(int k =1;k<=snake.leng[j]-1;k++){
				if (i != j) {
					if (Dcount(snake.coor[0][j], snake.coor[k][i]) <= 400)
						Offdir(j);
				}
			}
		}
		if (WallJudge(j,30) == true) {
			Offdir(j);
		}
}// ����AI����ײ
void AIJudgeDir(int j){
	double min;
	double d;
	int f,dx,dy;
	f = 0;
	min = d = Dcount(snake.coor[0][0],snake.coor[0][j]);
	for (int i = 0; i < fnum; i++)
	{ if (food.flag[i]==1)
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
					if (waitAI >= AIINTERVAL)
					{
						snake.dir[j] = LEFT; waitAI = 0;
					}
				}
				else
				{
					if (waitAI >= AIINTERVAL)
					{
						snake.dir[j] = RIGHT; waitAI = 0;
					}
				}
			}
			else
			{
				if (dy <= 0)
				{
					if ( waitAI >= AIINTERVAL)
					{
						snake.dir[j] = UP; waitAI = 0;
					}
				}
				else
				{
					if (waitAI >= AIINTERVAL)
					{
						snake.dir[j] = DOWN; waitAI = 0;
					}
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
				if (waitAI >= AIINTERVAL)
				{
					snake.dir[j] = LEFT; waitAI = 0;
				}
			}
			else
			{
				if ( waitAI >= AIINTERVAL)
				{
					snake.dir[j] = RIGHT; waitAI = 0;
				}
			}
		}
		else
		{
			if (dy <= 0)
			{
				if ( waitAI >= AIINTERVAL)
				{
					snake.dir[j] = UP; waitAI = 0;
				}
			}
			else
			{
				if ( waitAI >= AIINTERVAL)
				{
					snake.dir[j] = DOWN; waitAI = 0;
				}
			}
		}
	}
	AIhitSure(j);
 }// �����ж�
void AIconf() {
	for (int j = 1; j <= ainum; j++) 
	{
		AIJudgeDir(j);
		SnakeMove(j);
		EatFood(j);
	}
}// AI����
void Gameinit() {
	srand(GetTickCount());
	int i = 0;
	/* ��ʼ���� */
	conf[0] = 1;
	snake.leng[0] = 3;
	snake.dir[0] = rands(rands(LEFT, RIGHT), rands(UP, DOWN));
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
	snake.life[0] = 1;
	lifenum = 1;
	/* ��ʼ���߽��� */

	/* ��ʼ��ʳ�� */
	for (int k = 0; k <= 0.05 * fnum; k++) {
		food.fd[k].x = rand() % WIN_WIDTH;
		food.fd[k].y = rand() % WIN_HEIGHT;
		food.flag[k] = 1;
		food.rsize = 5;
		food.color[k] = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	/* ��ʼ��ʳ����� */

	/* ��ʼ��AI�� */
	AppearAI();
	lifenum += ainum;
	/* ��ʼ��AI�߽��� */
}
void GameDraw() {
	setbkcolor(RGB(106, 170, 150));// ������ɫ����
	cleardevice();
	/* ���� */
	for (int j = 0; j <= ainum; j++) {
		if (conf[j] == 1)
			SnakeDraw(j);
	}
	/* ���߽��� */

	/* ��ʳ�� */
	for (int i = 0; i < fnum; i++) {
		if (food.flag[i] == 1) {
			setfillcolor(food.color[i]);
			fillcircle(food.fd[i].x, food.fd[i].y, food.rsize);
		}
	}
	/* ��ʳ����� */

	/* ��ʾ������ʼ */
	char tmp_score[20] = "";
	sprintf(tmp_score, "����:%d", snake.score[0]);
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, tmp_score);
	char tmp_num[20] = "";
	sprintf(tmp_num, "ʣ��:%d", lifenum);
	setbkmode(TRANSPARENT);
	outtextxy(20, 40, tmp_num);
	/* ��ʾ�������� */

	/* ��ʾ��ʾ */
	if (lifenum == 1 && snake.life[0] != 0)
	{	
		outtextxy(WIN_WIDTH / 2 - 25, WIN_HEIGHT / 2 - 10, "YOU WIN!");
		Sleep(1000);
	}
	if (snake.life[0] == 0) {
		outtextxy(WIN_WIDTH / 2 - 30, WIN_HEIGHT / 2 - 10,"GAME OVER!");
		Sleep(1000);
	}
	/* ��ʾ��ʾ */
		
}
int main() {
	int wait = 0;
	int waitf = 0;
	srand(GetTickCount());
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);// ��ʼ������
	Gameinit();
	DWORD t1, t2,t3,t4;
	t1 = t2 = t3 = t4 = GetTickCount();
	BeginBatchDraw();
	while (1) {
		waitAI += 10;
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
					wait = 0;// �ٶȻظ�
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