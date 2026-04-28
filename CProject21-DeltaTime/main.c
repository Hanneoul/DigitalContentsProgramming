/*
 * =========================================================================
 * [고급 최적화: ANSI Escape Code & Partial Update]
 * =========================================================================
 * 1. 성능 분석: system("cls") vs ANSI Sequence
 * - cls: 외부 프로그램 실행(무거움) + 화면 전체 삭제(깜빡임 유발).
 * - ANSI: 출력 스트림 명령(가벼움) + 필요한 부분만 수정(깜빡임 없음).
 * * 2. 전역 변수(Global) vs 지역 변수(Local)
 * - 전역: g_x, g_y처럼 게임의 '상태'를 유지하며 모든 함수가 공유함.
 * - 지역: i, j처럼 함수 내에서 잠깐 계산용(Loop 등)으로 쓰고 사라짐.
 * * 3. 부분 업데이트 원리 (Dirty Rect)
 * - 캐릭터가 '있었던' 자리를 기억(g_prevX, g_prevY)했다가 그곳만 지우는 방식.
 * =========================================================================
 */

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

 // --- [전역 변수: 모든 함수가 공유하는 데이터] ---
int g_x = 10, g_y = 5;          // 현재 캐릭터 좌표
int g_prevX = 10, g_prevY = 5;  // 이전 프레임의 캐릭터 좌표 (지우기 용도)
bool g_isRunning = true;        // 루프 제어
char g_inputKey = 0;            // 입력된 키 저장

// --- [기능별 함수화 (I-U-R 구조)] ---

// 특정 위치로 커서를 즉시 이동시키는 함수 (ANSI 방식)
void MoveCursor(int row, int col) {
    // \x1b[ : ANSI 시작 / %d;%dH : 행;열 이동 / H : 명령 끝
    printf("\x1b[%d;%dH", row + 1, col + 1);
}

// 1. 입력 (Input)
void ProcessInput() {
    if (_kbhit()) g_inputKey = _getch();
    else g_inputKey = 0;
}

// 2. 업데이트 (Update)
void Update() {
    // 이동 전 위치를 저장 (나중에 이곳만 콕 집어서 지우기 위해)
    g_prevX = g_x;
    g_prevY = g_y;

    if (g_inputKey == 'w' || g_inputKey == 'W') g_y--;
    else if (g_inputKey == 's' || g_inputKey == 'S') g_y++;
    else if (g_inputKey == 'a' || g_inputKey == 'A') g_x--;
    else if (g_inputKey == 'd' || g_inputKey == 'D') g_x++;
    else if (g_inputKey == 'q' || g_inputKey == 'Q') g_isRunning = false;

    // 경계 검사 (Wall Collision)
    if (g_x < 0) g_x = 0; if (g_x > 60) g_x = 60;
    if (g_y < 0) g_y = 0; if (g_y > 20) g_y = 20;
}

// 3. 렌더링 (Render)
void Render() {
    // [최적화 핵심] 위치가 변했을 때만 이전 위치를 공백으로 지움
    if (g_x != g_prevX || g_y != g_prevY) {
        MoveCursor(g_prevY, g_prevX);
        printf("     "); // 이전 캐릭터 자리를 지우기
    }

    // 새 위치에 캐릭터 그리기
    MoveCursor(g_y, g_x);
    printf("(*_*)\n");

    // UI 영역 (고정 위치 렌더링)
    MoveCursor(22, 0);
    printf("--------------------------------------------\n");
    printf(" 좌표: %02d, %02d | system(\"cls\") 없는 부드러운 이동\n", g_x, g_y);
    printf(" WASD: 이동, Q: 종료\n");
    printf("--------------------------------------------");
}

int main() {
    
    system("cls"); // 시작할 때 딱 한 번만 화면을 깨끗이 지움
    printf("\x1b[?25l"); // 커서가 깜빡거리면 보기 싫으므로 숨김

    // [메인 게임 루프]
    while (g_isRunning) {
        ProcessInput(); // 1. 듣기
        Update();       // 2. 생각하기
        Render();       // 3. 말하기(그리기)

        Sleep(10); // 초당 약 100프레임 (매우 부드러움)
    }

    // 종료 시 설정 복구
    printf("\x1b[?25h"); // 커서 다시 보이기
    system("cls");
    printf("게임을 종료합니다.\n");

    return 0;
}