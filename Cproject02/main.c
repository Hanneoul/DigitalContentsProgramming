#include <stdio.h>

int move_cursor(int x, int y)
{
	/* ANSI(American National Standards Institute) Escape Sequence
	* - 컴퓨터 프로그래밍에서 글자 그대로 출력할 수 없거나, 줄 바꿈 같은 특별한 제어가 필요할 때 사용하는 문자 조합
	* - 보통 백슬래시(\) 기호 뒤에 특정 문자를 붙여서 만듬. 
	* - 원래 가진 문자 그대로의 의미에서 '탈출(Escape)'하여 특별한 기능을 수행한다는 뜻.
	* 
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