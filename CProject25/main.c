/*
======================================================================
[배경지식 설명]

1. Echo 명령어란?
   - 컴퓨팅에서 'echo'는 입력받은 문자열을 그대로 화면에 다시 출력(반향)하는 도구임.
   - 주로 시스템의 설정값이나 간단한 메시지를 확인할 때 사용함.

2. %date% 키워드 (환경 변수 흉내내기)
   - 윈도우 CMD나 터미널에서는 %DATE%라고 입력하면 시스템의 날짜로 자동 치환됨.
   - 이 코드에서는 사용자가 입력한 문장 속에 "%date%"라는 특정 문자열이 있으면,
     프로그램이 이를 인식하여 실제 날짜 데이터로 "갈아 끼우는" 로직을 구현함.

3. time.h 헤더와 strftime 함수
   - time.h: 시스템의 현재 시간 정보를 가져오기 위한 도구 모음임.
   - time_t: '초' 단위로 된 시간 데이터를 담는 자료형임.
   - struct tm: 초 단위 데이터를 년, 월, 일, 시, 분, 초로 쪼개서 담는 구조체임.
   - strftime: "Year-Month-Day" 같은 형식에 맞춰 날짜를 문자열로 예쁘게 만들어줌.
======================================================================
*/

#include <stdio.h>
#include <time.h>

int main() {
    char input[200];
    char date_str[20];
    int i = 0;

    while (1)
    {
        // 1. 현재 날짜 준비하기 (time.h 활용)
        time_t t = time(NULL);                // 현재 시간을 초 단위로 가져옴
        //time(&t);                           //이런방식으로도 활용가능
        struct tm* tm_info = localtime(&t);   // 사람이 보기 편하게 년/월/일 등으로 변환

        // strftime(결과저장소, 최대길이, 포맷, 시간구조체)
        // %Y: 4자리 년도, %m: 월, %d: 일을 의미함
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);

        printf("명령어 입력 (예: echo 오늘 날짜는 %%date%%야): \n> ");

        // 공백을 포함해서 한 줄 전체를 입력받음
        fgets(input, sizeof(input), stdin);

        // 2. 입력받은 문장에서 "echo " 부분 건너뛰기 (선택 사항)
        // 만약 맨 앞 5글자가 'e', 'c', 'h', 'o', ' ' 라면 그 다음부터 출력 시작
        int start = 0;
        if (input[0] == 'e' && input[1] == 'c' && input[2] == 'h' && input[3] == 'o' && input[4] == ' ')
        {
            start = 5;

            // 3. 한 글자씩 검사하며 %date% 치환 출력하기
            // string.h를 안 쓰기 위해 직접 반복문으로 처리함
            for (i = start; input[i] != '\0' && input[i] != '\n' && i < 200; i++)
            {
                if (input[i] == '/' && input[i + 1] == 'q')
                {
                    return 0;
                }

                if (i + 5 < 200)
                {
                    // 현재 위치부터 "%date%"라는 6글자가 나타나는지 확인
                    if (input[i] == '%' && input[i + 1] == 'd' && input[i + 2] == 'a' &&
                        input[i + 3] == 't' && input[i + 4] == 'e' && input[i + 5] == '%')
                    {

                        // 맞다면 "%date%" 대신 미리 만들어둔 날짜 문자열(date_str) 출력
                        printf("%s", date_str);

                        // %date%는 6글자이므로, 인덱스를 5칸 더 점프 시킴 (반복문 i++ 합쳐서 6칸)
                        i += 5;
                    }
                    else
                    {
                        // 키워드가 아니면 그냥 현재 글자 하나 출력
                        putchar(input[i]);
                    }
                }
                else
                {
                    // 키워드가 아니면 그냥 현재 글자 하나 출력
                    putchar(input[i]);
                }
            }
        }        
        printf("\n");
    }

    return 0;
}