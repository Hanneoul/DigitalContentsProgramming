/*
 * [반복문 핵심 정리]
 * 1. while: 조건식을 먼저 검사하고 참(true)인 동안 블록 내부를 반복함. (선 처리 후 검사)
 * 2. do-while: 블록 내부를 최소 1회 실행한 후 조건을 검사함. (최소 1회 실행 보장)
 * 3. for: 초기식, 조건식, 증감식을 한 줄에 명시하여 반복 횟수가 명확할 때 주로 사용함.
 * 4. break: 반복문의 조건과 상관없이 즉시 해당 반복문을 탈출함.
 * 5. continue: 반복문 내부에서 continue를 만나면, 아래 코드를 실행하지 않고 즉시 다음 반복 회차로 넘어감.
 */

#include <stdio.h>
#include <conio.h> // getch() 함수 사용을 위해 Windows.h 대신 또는 함께 사용됨
#include <Windows.h>

int main()
{
    printf("아무키나 눌러주세요\n");
    // 사용자로부터 키보드 입력 하나를 받음 (엔터 없이 즉시 입력)
    char input;

    int i = 0; // 반복 횟수를 제어할 변수(카운터) 초기화

    // [while문] 조건식(w < 10)이 참인지 먼저 확인
    while (i < 10)
    {
        input = _getch();

        // 입력받은 값이 'a'라면 반복문을 즉시 빠져나감 (탈출 조건)
        if (input == 'a')
        {
            break;
        }
        printf("(while)a키를 눌러야 종료된대.\n");

        // 반복 제어 변수 증가 (무한 루프 방지)
        i = i + 1;
    }

    i = 0;

    // [do-while문] 조건과 상관없이 일단 중괄호 {} 내부를 먼저 실행
    do
    {
        input = _getch(); // 다음 루프를 위해 새로운 입력 받음

        // 입력값이 'A'일 경우 break를 통해 루프 종료
        if (input == 'A')
        {
            break;
        }
        printf("(do while)A키를 눌러야 종료된대.\n");

        i += 1; // 변수 k를 1씩 증가 (k = k + 1과 동일)

    } while (i < 10); // 실행 후 조건을 검사하여 참이면 위로 돌아가 반복


 

    // [for문] (초기식; 조건식; 증감식) 순서로 구성됨
    // 1. i=0 초기화 -> 2. i<10 조건검사 -> 3. 내부 실행 -> 4. i++ 증감식 수행
    // for로 무한루프 -> for(;;)   //while(1)이랑 동일
    for (i = 0; i < 10; i+=1)
    {
        input = _getch();

        // ASCII 코드 27번은 ESC 키를 의미함
        if (input == 27)
            break; // ESC가 입력되었다면 for문을 즉시 종료
    }
    printf("(for)ESC를 눌러야 종료된대.\n");

    // for문의 루프가 끝날 때마다 자동으로 i가 증가함





    printf("--- continue 테스트 (홀수만 출력) ---\n");

    // [for문에서의 continue]
    for (i = 1; i <= 10; i++)
    {
        // i가 짝수라면?
        if (i % 2 == 0)
        {
            // 아래의 printf를 무시하고 즉시 i++(증감식)로 이동함
            continue;
        }

        // continue를 만나지 않은 홀수들만 이 라인에 도달함
        printf("현재 숫자: %d (홀수임)\n", i);
    }

    printf("\n--- 특정 키 입력 건너뛰기 ---\n");

    int w = 0;
    while (w < 5)
    {
        w++; // continue 사용 시 무한 루프 방지를 위해 증감식을 위로 올림

        printf("[%d번째] 's'를 누르면 메시지를 건너뜀: ", w);
        char input = getch();
        printf("%c\n", input);

        if (input == 's')
        {
            printf(">> 's'를 입력해서 출력을 건너뜀.\n\n");
            continue; // 아래 printf를 실행하지 않고 바로 while 조건문으로 복귀
        }

        printf(">> 이 메시지는 's'를 누르지 않았을 때만 보임.\n\n");
    }

    return 0;
}