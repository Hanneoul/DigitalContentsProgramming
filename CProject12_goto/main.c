#include <stdio.h>
#include <Windows.h>

int main()
{
	goto BADEND;

HAPPYEND:
	printf("학생들 : 너무 괴로워요\n");
	Sleep(1000);
	printf("교수 : 시험을 안보겠다\n");
	Sleep(1000);
	goto END;
	
BADEND:
	printf("교수 : 시험을 보겠다\n");
	Sleep(1000);
	printf("학생들 : ㅅㅂ\n");
	Sleep(1000);
	goto HAPPYEND;

END:
	return 0;
}