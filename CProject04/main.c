/*
 * [배경지식 요약]
 * 1. ANSI (American National Standards Institute): 미국 표준 협회. 컴퓨터 간 통신 규격을 정함.
 * - ANSI Escape Sequence: "\033["로 시작하는 특수 문자열로, 터미널의 커서 위치나 색상을 제어함.
 * 2. ASCII (American Standard Code for Information Interchange): 미국 정보 교환 표준 부호.
 * - 문자를 숫자로 표현하는 약속 (예: 'w'는 119, 's'는 115).
 * 3. 문자 vs 문자열 표기법:
 * - 'A' (Single Quote): 딱 한 글자(char)를 의미함. 내부적으로는 정수(숫자)로 취급됨.
 * - "A" (Double Quote): 문자열(String)을 의미함. 글자 'A' 뒤에 끝을 알리는 NULL(\0) 문자가 숨어 있음.
 * 4. getchar() vs getch():
 * - getchar(): 표준 함수. 문자를 입력하고 'Enter'를 눌러야 전송됨. 입력한 문자가 화면에 보임.
 * - getch(): 비표준(conio.h). 키를 누르는 즉시 반응함. 화면에 입력한 키가 보이지 않아 게임 조작에 유리함.
 */

#include <stdio.h>

 // 커서 위치를 (x, y) 좌표로 이동시키는 함수 정의
int move_cursor(int x, int y)
{
    /* * \033은 8진수로 ASCII의 ESC(Escape) 문자를 의미함.
     * [%d;%dH 형식은 터미널에게 "커서를 y행 x열로 옮겨라"라고 명령하는 표준 규격임.
     */
    printf("\033[%d;%dH", y, x);
    return 0;
}

int main()
{
    // [변수 선언] 데이터를 저장할 공간 확보
    int posY;    // 캐릭터의 세로 위치를 저장할 정수형 변수
    char input;  // 사용자가 누른 키를 저장할 문자형 변수

    // [초기값 설정] 시작 위치 지정
    posY = 5;

    // [화면 출력] 처음에 '-' 위치를 그림
    move_cursor(5, posY);
    printf("-");

    // [입력 부분] 사용자에게 키 입력을 요청함
    move_cursor(0, 10);
    printf("input key (w: up, s: down)> ");
    input = getchar(); // 사용자가 문자를 치고 Enter를 누를 때까지 대기

    // [로직 처리] 입력받은 문자에 따라 위치 값(데이터)을 변경함
    if (input == 'w') // 위로 이동 (y좌표 감소)
    {
        posY = posY - 1;
    }
    if (input == 's') // 아래로 이동 (y좌표 증가)
    {
        posY = posY + 1;
    }

    // [화면 갱신] 변경된 위치에 새로운 모양 '*'을 출력함
    move_cursor(5, posY);
    printf("*");

    // 프로그램 종료 전 커서를 아래로 치워줌
    move_cursor(0, 11);

    return 0;
}