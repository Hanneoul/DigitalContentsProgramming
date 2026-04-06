#include <stdio.h>

int main()
{
	while (1)
	{
		char input = _getch();
		switch (input)
		{
		case 'a': 
			printf("a가 눌렸다\n");
			break;
		case 'b': 
			printf("b가 눌렸다\n");
			break;
		case 'c': 
			printf("c가 눌렸다\n");
			break;
		default : 
			printf("다른 키가 눌렸다\n");
			break;
		case 27 :
			goto EXIT;
		}
	}
	EXIT:
	return 0;
}