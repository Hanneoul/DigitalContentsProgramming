#include <stdio.h>

// 모든 서식을 기본값으로 되돌리는 매크로
#define COLOR_RESET "\x1b[0m"

#define FONT_COLOR_BLACK 30
#define BG_COLOR_BLACK 40
#define FONT_COLOR_RED 31
#define BG_COLOR_RED 41
#define FONT_COLOR_GREEN 32
#define BG_COLOR_GREEN 42
#define FONT_COLOR_YELLOW 33
#define BG_COLOR_YELLOW 43
#define FONT_COLOR_BLUE 34
#define BG_COLOR_BLUE 44
#define FONT_COLOR_MAGENTA 35
#define BG_COLOR_MAGENTA 45
#define FONT_COLOR_BRIGHTMAGENTA 95
#define BG_COLOR_BRIGHTMAGENTA 105
#define FONT_COLOR_WHITE 37
#define BG_COLOR_WHITE 47

#define KEY_ESC 27

/*
 * ANSI Escape Sequence 색상 코드 가이드 (16색 표준)
 * ---------------------------------------------------------
 * [폰트 색상 (FG)]      [배경 색상 (BG)]      [색상명]
 * 30                  40                  Black
 * 31                  41                  Red
 * 32                  42                  Green
 * 33                  43                  Yellow
 * 34                  44                  Blue
 * 35                  45                  Magenta
 * 36                  46                  Cyan
 * 37                  47                  White
 * ---------------------------------------------------------
 * [밝은 폰트 (FG)]      [밝은 배경 (BG)]      [색상명]
 * 90                  100                 Bright Black (Gray)
 * 91                  101                 Bright Red
 * 92                  102                 Bright Green
 * 93                  103                 Bright Yellow
 * 94                  104                 Bright Blue
 * 95                  105                 Bright Magenta
 * 96                  106                 Bright Cyan
 * 97                  107                 Bright White
 * ---------------------------------------------------------
 * [기타 제어]
 * 0 : 모든 속성 초기화 (Reset)
 * 1 : 굵게 (Bold)
 * 4 : 밑줄 (Underline)
 * ---------------------------------------------------------
 * 사용법: printf("\x1b[%dm", 코드);
 */



/**
 * 폰트(글자) 색상을 변경하는 함수
 * @param code: 30-37 (기본), 90-97 (밝은 색)
 
 * 배경 색상을 변경하는 함수
 * @param code: 40-47 (기본), 100-107 (밝은 색)
 */

void set_color(int code) 
{
	printf("\x1b[%dm", code);
}

int move_cursor(int x, int y)
{
	printf("\033[%d;%dH", y, x);	//printf : print function
	return 0;
}

int main()
{
	unsigned short menuCursor = 1;
	int isRunning = 1;


	while (isRunning)
	{
		
		set_color(BG_COLOR_BRIGHTMAGENTA);
		set_color(FONT_COLOR_WHITE);
		move_cursor(50, 9);
		printf("                     ");
		move_cursor(50, 10);
		printf(" ♥ 임서진의 첫사랑 ♥ ");
		move_cursor(50, 11);
		printf("                     ");

		set_color(BG_COLOR_BLACK);
		

		
		if (menuCursor == 1)
		{
			set_color(BG_COLOR_YELLOW);
		}
		move_cursor(52, 13);
		printf("  1. 게임  시작  ");
		if (menuCursor == 1)
		{
			set_color(BG_COLOR_BLACK);
		}
		if (menuCursor == 2)
		{
			set_color(BG_COLOR_YELLOW);
		}
		move_cursor(52, 15);
		printf("  2. 만든  사람  ");
		if (menuCursor == 2)
		{
			set_color(BG_COLOR_BLACK);
		}
		if (menuCursor == 3)
		{
			set_color(BG_COLOR_YELLOW);
		}
		
		move_cursor(52, 17);
		printf("  3. 게임  종료  ");
		if (menuCursor == 3)
		{
			set_color(BG_COLOR_BLACK);
		}		
		char a = getch();
		if (a == 'w')
		{
			if (menuCursor > 1)
				menuCursor = menuCursor - 1;
		}
		if (a == 's')
		{
			if (menuCursor < 3)
				menuCursor = menuCursor + 1;
		}
		if (a == KEY_ESC)
		{
			isRunning = 0;
		}
		system("cls");
		
	}


	return 0;
}