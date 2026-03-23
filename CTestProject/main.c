#include <stdio.h>

int move_cursor(int x, int y)
{
	printf("\033[%d;%dH", y, x);	//printf : print function
	return 0;
}

int main()
{
	int x = 0;
	int y = 0;
	int isRunning = 1;

	while (isRunning)
	{
		char a = getch();
		if (a == ' ')
		{
			x = x + 1;
		}
		if (a == 'q')
		{
			isRunning = 0;
		}
		system("cls");
		move_cursor(x, y);
		printf("*");
	}
	printf("%c", a);

	return 0;
}