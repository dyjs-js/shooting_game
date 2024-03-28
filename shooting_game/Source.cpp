#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

//미사일 구조체
struct BULLET
{
	int x; //미사일 x좌표
	int y; //미사일 y좌표
	bool fire; //미사일 발사상태
};

//배열로 배경의 좌표를 만든다
char bg[25][80];

//플레이어 좌표
int playerX;
int playerY;

//적 좌표
int enemyX;
int enemyY;

//미사일 갯수
BULLET playerBullet[20];
BULLET enemyBullet[20];

//함수 설계
void ClearScreen();//화면을 지운다
void GameMain(); //게임 전체를 관리
void PrintScreen(); //화면을 그려주는 함수
void KeyControl(); //키 관련된 함수
void PlayerBulletDraw(); //플레이어의 미사일을 그려주는 함수
void EnemyBulletDraw(); //적의 미사일을 그려주는 함수
void PlayerDraw(); //플레이어 그리는 함수
void EnemyDraw(); //적을 그리는 함수
void EnemyMove(); //적을 움직이는 함수
void ClashEnemyAndBullet(); //충돌 처리 함수
void CursorView(); //콘솔 커서 안보이게 하는 함수

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void main()
{
	CursorView();

	//랜덤 함수 준비
	srand((unsigned)time(NULL));
	//플레이어 좌표 위치
	playerX = 0;
	playerY = 12;

	//적 좌표 위치
	enemyX = 77;
	enemyY = 12;

	//플레이어 미사일 초기화 (미사일 발사전 준비 상태)
	for (int i = 0; i < 20; i++)
	{
		playerBullet[i].x = 0; 
		playerBullet[i].y = 0;
		playerBullet[i].fire = false;
	}
	//적 미사일 초기화 (미사일 발사전 준비 상태)
	for (int i = 0; i < 20; i++)
	{
		enemyBullet[i].x = 0;
		enemyBullet[i].y = 0;
		enemyBullet[i].fire = false;
	}

	//초기화면 설정
	gotoxy(20, 6);
	printf("Shooting Game");
	gotoxy(20, 10);
	printf("방향: 이동키");
	gotoxy(20, 11);
	printf("공격: 스페이스바");
	gotoxy(20, 13);
	printf("시작하시려면 아무키나눌러주세요");
	_getch();

	//현재 초단위 시간을 받아온다.
	int dwTime = GetTickCount();

	//게임 실행 무한 루프
	while (true)
	{
		//0.05초 지연
		if (dwTime + 50 < GetTickCount())
		{
			dwTime = GetTickCount();
			//지우고
			ClearScreen();
			//플레이어나 적이 움직이고
			GameMain();
			//그려주고
			PrintScreen();
			
		}




	}



}

void CursorView()
{
	//커서 안보이게 하는 함수
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

}

void ClearScreen()
{
	//콘솔창에 그려진 모든것을 지움
	system("cls");
	
	//배열에 공백으로 넣어서 지움
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 80; x++)
		{
			bg[y][x] = ' ';
		}
	}
}

void GameMain()
{
	//키를 입력하는 부분
	KeyControl();
	//미사일을 그려준다.
	PlayerBulletDraw();
	//플레이어를 그려준다.
	PlayerDraw();
	//적의 움직임
	EnemyMove();
	//적의 미사일을 그려준다
	EnemyBulletDraw();
	//적을 그려주는 부분
	EnemyDraw();
	//미사일 적의 충돌
	ClashEnemyAndBullet();


}

void PrintScreen()
{
	bg[24][79] = '\0'; //맨 마지막 배열에 널문자를 넣어준다
	printf_s("%s", bg); //문자열로 전체 배경을 표현한다.

}

void KeyControl()
{

	char pressKey;
	
	//키가 입력되면 잡아준다.
	if (_kbhit())
	{
		pressKey = _getch(); //키가 입력되어 변수에 저장

		switch(pressKey)
		{
		case 72:
			//위쪽방향
			playerY--;
			if (playerY < 1)
			{
				playerY = 1;
			}
			break;

		case 75:
			//왼쪽방향
			playerX--;
			if (playerX < 0)
			{
				playerX = 0;
			}
			break;

		case 77:
			//오른쪽 방향
			playerX++;
			if (playerX > 75)
			{
				playerX = 75;
			}
			break;

		case 80:
			//아래 방향
			playerY++;
			if (playerY > 23)
			{
				playerY = 23;
			}
			break;

		case 32:
			//스페이스 키가 눌리면 미사일 발사
			for (int i = 0; i < 20; i++)
			{
				//미사일이 준비되어 있는 상태인 경우에만 발사
				if (playerBullet[i].fire == false)
				{
					playerBullet[i].fire = true;
					//플레이어 앞에서 미사일 쏘기 +5
					playerBullet[i].x = playerX + 5;
					playerBullet[i].y = playerY;
					//한발만 발사하기
					break;
				}
			}

		}
	}
}

void PlayerBulletDraw()
{
	//플레이어 미사일 그리기
	//미사일 전체 20개 중 
	for (int i = 0; i < 20; i++)
	{
		//플레이어 날아가고 있는 상태 true
		if (playerBullet[i].fire == true)
		{
			//미사일 그리기
			bg[playerBullet[i].y][playerBullet[i].x - 1] = '-';
			bg[playerBullet[i].y][playerBullet[i].x - 0] = '@';

			//미사일 앞으로 +1
			playerBullet[i].x++;

			//미사일이 80을 넘어가는 경우
			if (playerBullet[i].x > 79)
			{
				//미사일 준비 상태로 전환
				playerBullet[i].fire = false;
			}
		}
	}

}

void EnemyBulletDraw()
{
	//적 미사일 그리기
	//미사일 전체 20개 중 
	for (int i = 0; i < 1; i++)
	{
		//플레이어 날아가고 있는 상태 true
		if (enemyBullet[i].fire == true)
		{ 
			//미사일 그리기
			bg[enemyBullet[i].y][enemyBullet[i].x - 1] = '<';
			bg[enemyBullet[i].y][enemyBullet[i].x - 0] = '-';

			//미사일 앞으로 +1
			enemyBullet[i].x-=1.2;

			//미사일이 2을 넘어가는 경우
			if (enemyBullet[i].x < 2)
			{
				//미사일 준비 상태로 전환
				enemyBullet[i].fire = false;
			}
		}
	}

}

void PlayerDraw()
{
	//플레이어 그리기
	
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
	//적 그리기
	bg[enemyY][enemyX - 2] = '<';
	bg[enemyY][enemyX - 1] = '-';
	bg[enemyY][enemyX + 0] = 'O';
	bg[enemyY][enemyX + 1] = '-';
	bg[enemyY][enemyX + 2] = '>';

}

void EnemyMove()
{
	// 적의 움직임을 위아래로도 추가
	//int direction = rand() % 3; // 0, 1, 2 중 하나의 값을 가짐

	//if (direction == 0) // 위로 이동
	//{
	//	enemyY--;
	//	enemyX--;
	//	if (enemyY < 2)
	//		enemyY = 2;
	//}
	//else if (direction == 1) // 아래로 이동
	//{
	//	enemyY++;
	//	enemyX--;
	//	if (enemyY > 22)
	//		enemyY = 22;
	//}
	//else // 왼쪽으로 이동
	//{
		enemyX--;

		// 적은 자동으로 미사일 발사
		for (int i = 0; i < 20; i++)
		{
			//미사일이 준비되어 있는 상태인 경우에만 발사
			if (enemyBullet[i].fire == false)
			{
				enemyBullet[i].fire = true;
				//적의 위치에서 미사일 쏘기 -5
				enemyBullet[i].x = enemyX -5;
				enemyBullet[i].y = enemyY;
				//한발만 발사하기
				break;
			}
		}

		if (enemyX < 2)
		{
			// 적을 랜덤하게 y쪽 좌표를 바꿔준다.
			enemyX = 77;
			enemyY = (rand() % 20) + 2;
		}
	//}
}

//충돌처리함수
void ClashEnemyAndBullet()
{
	//미사일 20개 전체 판별
	for (int i = 0; i < 20; i++)
	{
		//미사일이 살아있고
		if (playerBullet[i].fire == true)
		{
			//적과 미사일의 y값이 같고
			if (playerBullet[i].y == enemyY)
			{
				//플레이어 미사일 x값이 같은지 판별
				//x값은 적의 크기 만큼 x-1, 0 , x+1 까지 세개 좌표로 잘 충돌되게 판별
				if (playerBullet[i].x >= (enemyX - 1) && playerBullet[i].x <= (enemyX + 1))
				{
					//충돌되면 적 y값을 바꿔주기
					enemyX = 77;
					enemyY = (rand() % 20) + 2;
					playerBullet[i].fire = false;

				}


			}
		}
	}
}
