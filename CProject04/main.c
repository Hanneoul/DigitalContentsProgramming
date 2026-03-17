#include <stdio.h>

int move_cursor(int x, int y)
{
	/* ANSI Escape Sequence
	* 커서의 이동 : \033[<row>;<col>H
	*/
	printf("\033[%d;%dH", y, x);	//printf : print function
	return 0;
}

int main()
{
	//변수 선언
	int posY;
	char input;

	//변수 값 지정
	posY = 5;

	//화면 출력
	move_cursor(5, posY);
	printf("-");

	//입력 부분
	move_cursor(0, 10);
	printf("input key>");
	input = getchar();

	if (input == 'w')
	{
		posY = posY - 1;
	}
	if (input == 's')
	{
		posY = posY + 1;
	}

	move_cursor(5, posY);
	printf("*");

	move_cursor(0, 11);

	return 0;
}