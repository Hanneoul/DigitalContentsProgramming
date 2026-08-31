#include <stdio.h>

/*
 * ANSI Escape Sequence 색상 코드 가이드 (16색 표준)
 * ---------------------------------------------------------
 * [폰트 색상 (FG)]      [배경 색상 (BG)]      [색상명]
 * 30                  40                  Black
 * 31                  41                  Red
 * 32                  42                  Green
 * 33                  43                  Yellow
 * 34                  44                  Blue
 * 35                  45                  Magenta
 * 36                  46                  Cyan
 * 37                  47                  White
 * ---------------------------------------------------------
 * [밝은 폰트 (FG)]      [밝은 배경 (BG)]      [색상명]
 * 90                  100                 Bright Black (Gray)
 * 91                  101                 Bright Red
 * 92                  102                 Bright Green
 * 93                  103                 Bright Yellow
 * 94                  104                 Bright Blue
 * 95                  105                 Bright Magenta
 * 96                  106                 Bright Cyan
 * 97                  107                 Bright White
 * ---------------------------------------------------------
 * [기타 제어]
 * 0 : 모든 속성 초기화 (Reset)
 * 1 : 굵게 (Bold)
 * 4 : 밑줄 (Underline)
 * ---------------------------------------------------------
 * 사용법: printf("\x1b[%dm", 코드);
 */

 // 모든 서식을 기본값으로 되돌리는 매크로
#define COLOR_RESET "\x1b[0m"

/**
 * 폰트(글자) 색상을 변경하는 함수
 * @param code: 30-37 (기본), 90-97 (밝은 색)
 */
void set_font_color(int code) 
{
    printf("\x1b[%dm", code);
}

/**
 * 배경 색상을 변경하는 함수
 * @param code: 40-47 (기본), 100-107 (밝은 색)
 */
void set_bg_color(int code) 
{
    printf("\x1b[%dm", code);
}

int main() 
{
    // 예제 1: 밝은 파란색 글자 출력
    set_font_color(94);
    printf("이것은 Bright Blue 폰트임.\n");

    // 예제 2: 빨간색 배경에 밝은 흰색 글자 출력
    set_bg_color(41);
    set_font_color(97);
    printf("빨간색 배경 + 밝은 흰색 글자 조합임.");

    // 서식 초기화 필수
    printf(COLOR_RESET "\n서식이 초기화되었음.\n");

    // 예제 3: 반복문을 이용한 16색 폰트 테스트
    printf("\n--- 16색 폰트 테스트 ---\n");
    for (int i = 30; i <= 37; i++) 
    {
        set_font_color(i);
        printf("Color %d ", i);
    }
    printf("\n");
    for (int i = 90; i <= 97; i++) 
    {
        set_font_color(i);
        printf("Color %d ", i);
    }
    printf(COLOR_RESET "\n");

    return 0;
}