#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768 // ���ڿ��
#define MAX_SHAKE 32767// ����һ���㹻�����󳤶�
#define MAX_SNAKE 100// ����һ���㹻�������
const int RUSH = 30;
const int NORMAL = 60;
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
	int conf;// ��������Ҳٿػ��ǵ��Բٿ�
	int leng;// �ߵĳ��Ƚڣ����������ɶ���Բ�ι���
	int dir;// �ߵķ���
	int score;// �ɼ�
	int rsize;// �ߴ�
	int speed;
	POINT coor[MAX_SHAKE];// λ��

}snake;
struct Food_tlg// ʳ��Ľṹ��
{
	POINT fd;
	int flag;
	int rsize;
	DWORD color;
}food;
void Gameinit() {
	srand(GetTickCount());
	/* ��ʼ���� */
	snake.leng = 3;
	snake.dir = LEFT;// �ݶ�����
	snake.score = 0;
	snake.rsize = 10;
	snake.speed = NORMAL;
	snake.coor[2].x = 512 + snake.rsize*2;
	snake.coor[2].y = 384;
	snake.coor[1].x = 512 + snake.rsize;
	snake.coor[1].y = 384;
	snake.coor[0].x = 512;
	snake.coor[0].y = 384;
	/* ��ʼ���߽��� */

	/* ��ʼ��ʳ�� */
	food.fd.x = rand() % WIN_WIDTH;
	food.fd.y = rand() % WIN_HEIGHT;
	food.flag = 1;
	food.rsize = 5;
	food.color = RGB(rand() % 256,rand() % 256,rand() % 256);
	/* ��ʼ��ʳ����� */
}
void GameDraw() {
	setbkcolor(RGB(106, 170, 150));// ������ɫ����
	cleardevice();
	/* ���� */
	for (int i = 0; i < snake.leng; i++) {
		setfillcolor(RGB(66, 75, 98));
		if (i != 0) {
			fillcircle(snake.coor[i].x, snake.coor[i].y, 10);
		}
		else {
			setfillcolor(RGB(166, 75, 98));
			fillcircle(snake.coor[i].x, snake.coor[i].y, 12);
			fillcircle(snake.coor[i].x, snake.coor[i].y, 2);
		}// ���ͷ
	}
	/* ���߽��� */

	/* ��ʳ�� */
	if (food.flag == 1) {
     setfillcolor(food.color);
	 fillcircle(food.fd.x, food.fd.y, food.rsize);
	}
	
	/* ��ʳ����� */

	/* ��ʾ������ʼ */
	char tmp[20] = "";
	sprintf(tmp, "����:%d", snake.score);
	setbkmode(TRANSPARENT);
	outtextxy(20, 20, tmp);
	/* ��ʾ�������� */

}
bool HitJudge(POINT n1, POINT n2) {
	int dsize = (snake.rsize - food.rsize)+3;
	if (n1.x - dsize <= n2.x && n2.x <= n1.x + dsize &&
		n1.y - dsize <= n2.y && n2.y <= n1.y + dsize)// ����֮��
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool HitJudge_ss(POINT n1, POINT n2) {
	return false;
}
void SnakeMove(){
	for (int i = snake.leng - 1; i > 0; i--)
	{
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}// ����̳��˶�
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
	}//�ƶ�
}
void EatFood(){
	if ( HitJudge(food.fd,snake.coor[0])==true && food.flag == 1)//��ʱʹ����������ײ���
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
	if (GetAsyncKeyState(VK_SPACE)) {
		snake.speed = RUSH; // ����
	}
}// ʹ����winAPI
void DeathJudge(){
	for (int i = 1; i <= snake.leng - 1; i++) {
		if (HitJudge_ss(snake.coor[0], snake.coor[i]) == true) {
			outtextxy(512, 384, "Game Over!");
			_getch();
			exit(233333);
		}
	}
}
int main() {
	int wait = 0;
	initgraph(WIN_WIDTH, WIN_HEIGHT,SHOWCONSOLE);// ��ʼ������
	Gameinit();
	DWORD t1, t2;
	t1 = t2 = GetTickCount();
	BeginBatchDraw();
	while (1) {
		if ((t2 - t1) > snake.speed) {
			SnakeMove();
			t1 = t2;
		}
		t2= GetTickCount();
		FlushBatchDraw();
		if (snake.speed == RUSH){
			wait += 10;
				if (wait >= 100) {
					snake.speed = NORMAL;
					wait = 0;// �ٶȻظ�
				}
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