#include <stdio.h>
#include <Windows.h>

int main()
{
    char input = getch();

    int w = 0;
    while (w<10)
    {
        if (input == 'a')
        {
            break;
        }
        printf("(while)a키를 눌러야 종료된대.\n");
        
        w = w + 1;
    }

    input = getch();

    int k = 0;
    do
    {
        if (input == 'A')
        {
            break;
        }
        printf("(do while)A키를 눌러야 종료된대.\n");
        k += 1 ;
    } while (k<10);

    input = getch();

    int i = 0;
    for (i=0; i<10; i++) //++i
    {
        if (input == 27)
        {
            break;
        }
        printf("(for)ESC를 눌러야 종료된대.\n");
        
    }

    



    return 0;
}
