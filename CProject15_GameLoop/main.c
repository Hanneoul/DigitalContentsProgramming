//Game Loop
//

#include <stdio.h>
#include <Windows.h>
#define GAMESTATE_QUIT -1
#define GAMESTATE_START 0
#define GAMESTATE_TITLE 1
#define GAMESTATE_TITLE_ENTER 2
#define GAMESTATE_TITLE_EXIT 3

//함수 미리 정의
int ProcessInput();
int Update();
int Render();

/*
 * -1: 게임종료
 * 0 : 게임 시작
 * 1 : 게임 타이틀 화면
 * 2 : 게임 타이틀 화면으로 전환
 * 3 : 게임 타이틀 화면에서 탈출
 */
int gameState = 0;

int main()
{	
	gameState = 0;
	while (gameState != GAMESTATE_QUIT)
	{
		ProcessInput();
		Update();
		Render();
		Sleep(300);
	}
	return 0;
}

int ProcessInput()
{
	if (gameState == GAMESTATE_TITLE)
	{
		int input = getch();
		if (input == 27)		//esc
		{
			gameState = GAMESTATE_TITLE_EXIT;
		}
	}
	return 0;
}

int Update()
{
	if (gameState == GAMESTATE_START)
	{
		gameState = GAMESTATE_TITLE_ENTER;
	}

	return 0;
}

int Render()
{
	if (gameState == GAMESTATE_TITLE_ENTER)
	{
		system("cls");
		puts("             타이틀 화면이다!!!! \n");
		puts("     ESC 키를 누르면 게임이 종료됩니다. \n");
		gameState = GAMESTATE_TITLE;
	}
	if (gameState == GAMESTATE_TITLE)
	{
		printf("\n입력을 하세요(ESC키!)>");
		
	}
	if (gameState == GAMESTATE_TITLE_EXIT)
	{
		printf(" 타이틀 화면에서 떠납니다.");
		gameState = GAMESTATE_QUIT;
	}
	if (gameState == GAMESTATE_QUIT)
	{
		printf("\n\n 게임을 종료합니다.");

	}
	
	return 0;
}