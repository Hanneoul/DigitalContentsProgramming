/*
 * =========================================================================
 * [개념 학습: 전역 변수 vs 지역 변수 예제]
 * =========================================================================
 * 1. 전역 변수 (Global): 프로그램 어디서나 접근 가능. 게임 데이터 저장소 역할.
 * 2. 지역 변수 (Local): 함수 내부에서만 존재. 잠깐 쓰고 버리는 포스트잇 역할.
 * 3. 함수화: 기능을 Input, Update, Render로 나누어 가독성을 높임.
 * =========================================================================
 */

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

 // --- [전역 변수 (Global Variables)] ---
 // 프로그램 시작부터 끝까지 살아있으며, 모든 함수가 공유함.
int g_x = 10, g_y = 5;      // 캐릭터 좌표 (g_를 붙여 전역임을 명시하는 관습)
bool g_isRunning = true;    // 게임 실행 상태
char g_inputKey = 0;        // 눌린 키 저장용

// --- [함수 선언부] ---

// 1. 입력 단계: 지금 어떤 키가 눌렸는지만 판단함.
void ProcessInput() {
    // _kbhit은 conio.h의 함수로, 키가 눌렸는지 즉시 확인(Non-blocking)
    if (_kbhit()) {
        g_inputKey = _getch(); // 전역 변수 g_inputKey에 저장
    }
    else {
        g_inputKey = 0;
    }
}

// 2. 업데이트 단계: 입력된 데이터에 따라 좌표 계산 및 로직 처리.
void Update() {
    // 전역 변수 g_x, g_y를 수정함
    if (g_inputKey == 'w' || g_inputKey == 'W') g_y--;
    else if (g_inputKey == 's' || g_inputKey == 'S') g_y++;
    else if (g_inputKey == 'a' || g_inputKey == 'A') g_x--;
    else if (g_inputKey == 'd' || g_inputKey == 'D') g_x++;
    else if (g_inputKey == 'q' || g_inputKey == 'Q') g_isRunning = false;

    // 경계 검사 로직
    if (g_x < 0) g_x = 0; if (g_x > 30) g_x = 30;
    if (g_y < 0) g_y = 0; if (g_y > 10) g_y = 10;
}

// 3. 렌더링 단계: 현재 좌표 정보를 화면에 출력함.
void Render() {
    system("cls"); // 화면 지우기

    printf("=== 전역/지역 변수 실습 (WASD 이동 / Q 종료) ===\n");
    printf("현재 좌표 - X: %d, Y: %d\n", g_x, g_y);
    printf("--------------------------------------------\n");

    // --- [지역 변수 (Local Variables) 사용] ---
    // i와 j는 Render 함수 내부에서만 존재하며, 함수가 끝나면 사라짐.
    for (int i = 0; i < g_y; i++) {
        printf("\n");
    }
    for (int j = 0; j < g_x; j++) {
        printf(" ");
    }
    printf("(*_*)\n");
}

// --- [메인 함수] ---
int main() {
    // 메인 루프 구조가 매우 간결해짐
    while (g_isRunning) {
        ProcessInput(); // 입력받기
        Update();       // 계산하기
        Render();       // 그리기

        Sleep(30);      // 초당 약 30프레임 속도 조절
    }

    printf("\n게임을 종료합니다.\n");
    return 0;
}