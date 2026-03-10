#include <stdio.h>

void moveCursor(int x,int y)
{
	/* ＡＮＳＩ 코드 ： \033[<row>;<col>H  */
	printf("\033[%d;%dH",y, x);  // 10행 20열로 커서 이동 후 출력	
	return;
}

int main()
{
	
	moveCursor(20, 10);
	printf("%%I love pork\\\ttab\n\"Also sogogi\"");
	return 0;
}