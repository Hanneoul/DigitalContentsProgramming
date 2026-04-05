#include <stdio.h>
#include <Windows.h>

void set_color(int code) {
	printf("\x1b[%dm", code);
}

int main()
{
	printf("\x1b[33m   고양이와 나\n\n");
	Sleep(2000);
	set_color(35);
	printf("고양이 : 냐옹!\n");
	Sleep(2000);
	set_color(36);
	printf("주인공 : 건방진 고양이놈! 눈깔아!\n");
	Sleep(2000);
	set_color(35);
	printf("고양이 : 집사주제에 밥이나 가져와라\n");
	Sleep(2000);

	set_color(33);
	printf("\n  !!!행동을 선택해 주세요!!!\n\n");

	set_color(37);
	printf("1. 츄르를 주면서 유인한다.\n");
	printf("2. 근황을 질문한다.\n");
	printf("\n 행동을 입력해 주세요\n\n");

	char input = 0;

	input = getch();

	if (input == '1')
	{
		
		set_color(35);
		printf("고양이 : 앗... 흠흠... 앞으론 츄르좀 데워서 주면 더 좋을꺼 같다냥... \n\t특별히 오늘은 날 쓰다듬도록 해라냥!\n");
		Sleep(2000);
		set_color(36);
		printf("주인공 : 으헤헤헤헤헤헤.....\n");
		Sleep(2000);
		set_color(35);
		printf("고양이 : 아...안돼!!!\n");
		set_color(37);
		printf("\n   Happy Ending\n");
	}
	else if (input == '2')
	{
		
		set_color(36);
		printf("주인공 : 네 근황을 보고해라\n");
		Sleep(2000);
		set_color(35);
		printf("고양이 : 흥! 오늘 니놈방에서 거미를 봤다\n");
		Sleep(2000);
		set_color(36);
		printf("주인공 : 으아아아아아아 잡아줘... 으아아아아아아아\n");
		set_color(37);
		printf("\n   Sad Ending\n");
	}
	else
	{
		printf("잘못 누르셨습니다. 고양이가 빈정댑니다\n");
		Sleep(2000);
		set_color(35);
		printf("고양이 : 메뉴도 못고르냥? 넌 오늘부로 집사 해고다.\n");		
		set_color(37);
		printf("\n   Bad Ending\n");
	}

	return 0;
}