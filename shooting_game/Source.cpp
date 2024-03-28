#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

//�̻��� ����ü
struct BULLET
{
	int x; //�̻��� x��ǥ
	int y; //�̻��� y��ǥ
	bool fire; //�̻��� �߻����
};

//�迭�� ����� ��ǥ�� �����
char bg[25][80];

//�÷��̾� ��ǥ
int playerX;
int playerY;

//�� ��ǥ
int enemyX;
int enemyY;

//�̻��� ����
BULLET playerBullet[20];
BULLET enemyBullet[20];

//�Լ� ����
void ClearScreen();//ȭ���� �����
void GameMain(); //���� ��ü�� ����
void PrintScreen(); //ȭ���� �׷��ִ� �Լ�
void KeyControl(); //Ű ���õ� �Լ�
void PlayerBulletDraw(); //�÷��̾��� �̻����� �׷��ִ� �Լ�
void EnemyBulletDraw(); //���� �̻����� �׷��ִ� �Լ�
void PlayerDraw(); //�÷��̾� �׸��� �Լ�
void EnemyDraw(); //���� �׸��� �Լ�
void EnemyMove(); //���� �����̴� �Լ�
void ClashEnemyAndBullet(); //�浹 ó�� �Լ�
void CursorView(); //�ܼ� Ŀ�� �Ⱥ��̰� �ϴ� �Լ�

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void main()
{
	CursorView();

	//���� �Լ� �غ�
	srand((unsigned)time(NULL));
	//�÷��̾� ��ǥ ��ġ
	playerX = 0;
	playerY = 12;

	//�� ��ǥ ��ġ
	enemyX = 77;
	enemyY = 12;

	//�÷��̾� �̻��� �ʱ�ȭ (�̻��� �߻��� �غ� ����)
	for (int i = 0; i < 20; i++)
	{
		playerBullet[i].x = 0; 
		playerBullet[i].y = 0;
		playerBullet[i].fire = false;
	}
	//�� �̻��� �ʱ�ȭ (�̻��� �߻��� �غ� ����)
	for (int i = 0; i < 20; i++)
	{
		enemyBullet[i].x = 0;
		enemyBullet[i].y = 0;
		enemyBullet[i].fire = false;
	}

	//�ʱ�ȭ�� ����
	gotoxy(20, 6);
	printf("Shooting Game");
	gotoxy(20, 10);
	printf("����: �̵�Ű");
	gotoxy(20, 11);
	printf("����: �����̽���");
	gotoxy(20, 13);
	printf("�����Ͻ÷��� �ƹ�Ű�������ּ���");
	_getch();

	//���� �ʴ��� �ð��� �޾ƿ´�.
	int dwTime = GetTickCount();

	//���� ���� ���� ����
	while (true)
	{
		//0.05�� ����
		if (dwTime + 50 < GetTickCount())
		{
			dwTime = GetTickCount();
			//�����
			ClearScreen();
			//�÷��̾ ���� �����̰�
			GameMain();
			//�׷��ְ�
			PrintScreen();
			
		}




	}



}

void CursorView()
{
	//Ŀ�� �Ⱥ��̰� �ϴ� �Լ�
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

}

void ClearScreen()
{
	//�ܼ�â�� �׷��� ������ ����
	system("cls");
	
	//�迭�� �������� �־ ����
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 80; x++)
		{
			bg[y][x] = ' ';
		}
	}
}

void GameMain()
{
	//Ű�� �Է��ϴ� �κ�
	KeyControl();
	//�̻����� �׷��ش�.
	PlayerBulletDraw();
	//�÷��̾ �׷��ش�.
	PlayerDraw();
	//���� ������
	EnemyMove();
	//���� �̻����� �׷��ش�
	EnemyBulletDraw();
	//���� �׷��ִ� �κ�
	EnemyDraw();
	//�̻��� ���� �浹
	ClashEnemyAndBullet();


}

void PrintScreen()
{
	bg[24][79] = '\0'; //�� ������ �迭�� �ι��ڸ� �־��ش�
	printf_s("%s", bg); //���ڿ��� ��ü ����� ǥ���Ѵ�.

}

void KeyControl()
{

	char pressKey;
	
	//Ű�� �ԷµǸ� ����ش�.
	if (_kbhit())
	{
		pressKey = _getch(); //Ű�� �ԷµǾ� ������ ����

		switch(pressKey)
		{
		case 72:
			//���ʹ���
			playerY--;
			if (playerY < 1)
			{
				playerY = 1;
			}
			break;

		case 75:
			//���ʹ���
			playerX--;
			if (playerX < 0)
			{
				playerX = 0;
			}
			break;

		case 77:
			//������ ����
			playerX++;
			if (playerX > 75)
			{
				playerX = 75;
			}
			break;

		case 80:
			//�Ʒ� ����
			playerY++;
			if (playerY > 23)
			{
				playerY = 23;
			}
			break;

		case 32:
			//�����̽� Ű�� ������ �̻��� �߻�
			for (int i = 0; i < 20; i++)
			{
				//�̻����� �غ�Ǿ� �ִ� ������ ��쿡�� �߻�
				if (playerBullet[i].fire == false)
				{
					playerBullet[i].fire = true;
					//�÷��̾� �տ��� �̻��� ��� +5
					playerBullet[i].x = playerX + 5;
					playerBullet[i].y = playerY;
					//�ѹ߸� �߻��ϱ�
					break;
				}
			}

		}
	}
}

void PlayerBulletDraw()
{
	//�÷��̾� �̻��� �׸���
	//�̻��� ��ü 20�� �� 
	for (int i = 0; i < 20; i++)
	{
		//�÷��̾� ���ư��� �ִ� ���� true
		if (playerBullet[i].fire == true)
		{
			//�̻��� �׸���
			bg[playerBullet[i].y][playerBullet[i].x - 1] = '-';
			bg[playerBullet[i].y][playerBullet[i].x - 0] = '@';

			//�̻��� ������ +1
			playerBullet[i].x++;

			//�̻����� 80�� �Ѿ�� ���
			if (playerBullet[i].x > 79)
			{
				//�̻��� �غ� ���·� ��ȯ
				playerBullet[i].fire = false;
			}
		}
	}

}

void EnemyBulletDraw()
{
	//�� �̻��� �׸���
	//�̻��� ��ü 20�� �� 
	for (int i = 0; i < 1; i++)
	{
		//�÷��̾� ���ư��� �ִ� ���� true
		if (enemyBullet[i].fire == true)
		{ 
			//�̻��� �׸���
			bg[enemyBullet[i].y][enemyBullet[i].x - 1] = '<';
			bg[enemyBullet[i].y][enemyBullet[i].x - 0] = '-';

			//�̻��� ������ +1
			enemyBullet[i].x-=1.2;

			//�̻����� 2�� �Ѿ�� ���
			if (enemyBullet[i].x < 2)
			{
				//�̻��� �غ� ���·� ��ȯ
				enemyBullet[i].fire = false;
			}
		}
	}

}

void PlayerDraw()
{
	//�÷��̾� �׸���
	
	bg[playerY + -1][playerX + 1] = '>';
	bg[playerY + -1][playerX + 0] = '-';
	bg[playerY + 0][playerX + 1] = '>';
	bg[playerY + 0][playerX + 2] = '>';
	bg[playerY + 0][playerX + 3] = '>';
	bg[playerY + 1][playerX + 0] = '-';
	bg[playerY + 1][playerX + 1] = '>';

}

void EnemyDraw()
{
	//�� �׸���
	bg[enemyY][enemyX - 2] = '<';
	bg[enemyY][enemyX - 1] = '-';
	bg[enemyY][enemyX + 0] = 'O';
	bg[enemyY][enemyX + 1] = '-';
	bg[enemyY][enemyX + 2] = '>';

}

void EnemyMove()
{
	// ���� �������� ���Ʒ��ε� �߰�
	//int direction = rand() % 3; // 0, 1, 2 �� �ϳ��� ���� ����

	//if (direction == 0) // ���� �̵�
	//{
	//	enemyY--;
	//	enemyX--;
	//	if (enemyY < 2)
	//		enemyY = 2;
	//}
	//else if (direction == 1) // �Ʒ��� �̵�
	//{
	//	enemyY++;
	//	enemyX--;
	//	if (enemyY > 22)
	//		enemyY = 22;
	//}
	//else // �������� �̵�
	//{
		enemyX--;

		// ���� �ڵ����� �̻��� �߻�
		for (int i = 0; i < 20; i++)
		{
			//�̻����� �غ�Ǿ� �ִ� ������ ��쿡�� �߻�
			if (enemyBullet[i].fire == false)
			{
				enemyBullet[i].fire = true;
				//���� ��ġ���� �̻��� ��� -5
				enemyBullet[i].x = enemyX -5;
				enemyBullet[i].y = enemyY;
				//�ѹ߸� �߻��ϱ�
				break;
			}
		}

		if (enemyX < 2)
		{
			// ���� �����ϰ� y�� ��ǥ�� �ٲ��ش�.
			enemyX = 77;
			enemyY = (rand() % 20) + 2;
		}
	//}
}

//�浹ó���Լ�
void ClashEnemyAndBullet()
{
	//�̻��� 20�� ��ü �Ǻ�
	for (int i = 0; i < 20; i++)
	{
		//�̻����� ����ְ�
		if (playerBullet[i].fire == true)
		{
			//���� �̻����� y���� ����
			if (playerBullet[i].y == enemyY)
			{
				//�÷��̾� �̻��� x���� ������ �Ǻ�
				//x���� ���� ũ�� ��ŭ x-1, 0 , x+1 ���� ���� ��ǥ�� �� �浹�ǰ� �Ǻ�
				if (playerBullet[i].x >= (enemyX - 1) && playerBullet[i].x <= (enemyX + 1))
				{
					//�浹�Ǹ� �� y���� �ٲ��ֱ�
					enemyX = 77;
					enemyY = (rand() % 20) + 2;
					playerBullet[i].fire = false;

				}


			}
		}
	}
}
