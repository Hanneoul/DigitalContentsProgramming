#include <stdio.h>
#include <windows.h>	//윈도우32 라이브러리

int main()
{
	int iterator = 0;

	//반복문 : while (조건) { 반복내용 }
	while (iterator < 10)
	{
		system("cls");	//시스템 명령어를 사용한다 system(명령어 문자열)
		printf("오늘 점심메뉴 알려줘 %d초 준다.\n", 10 - iterator);
		iterator = iterator + 1;
		Sleep(1000);	//ms 단위로 쉰다. : Sleep(ms)
	}	

	return 0;
}