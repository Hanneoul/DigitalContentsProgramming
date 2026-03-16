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
	move_cursor(5, 5);
	puts("Cursor is Moved!!!");

	/* Escape Sequence example
	* \n : 엔터 (줄바꿈)
	* \\ : '\' 백슬래시 문자 입력
	* \" : '"' 따옴표 문자 입력
	* \' : 작은따옴표 문자 입력
	* \t : 탭
	* \b : backspace
	* ...
	*/
	puts("\n\n\n\" Escape Sequence Sample \" ");
	return 0;
}