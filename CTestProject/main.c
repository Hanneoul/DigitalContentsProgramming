/*
 * =========================================================================
 * [고급 최적화: ANSI Escape Code & Partial Update]
 * =========================================================================
 * 1. 성능 분석: system("cls") vs ANSI Sequence
 *  - cls: 외부 프로그램 실행(무거움) + 화면 전체 삭제(깜빡임 유발).
 *  - ANSI: 출력 스트림 명령(가벼움) + 필요한 부분만 수정(깜빡임 없음).
 *
 * 2. 전역 변수(Global) vs 지역 변수(Local)
 *  - 전역: g_x, g_y처럼 게임의 '상태'를 유지하며 모든 함수가 공유함.
 *  - 지역: i, j처럼 함수 내에서 잠깐 계산용(Loop 등)으로 쓰고 사라짐.
 *
 * 3. 부분 업데이트 원리 (Dirty Rect)
 *  - 캐릭터가 '있었던' 자리를 기억(g_prevX, g_prevY)했다가 그곳만 지우는 방식.
 * =========================================================================
 */

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

 // --- [전역 변수: 모든 함수가 공유하는 데이터] ---
int g_x = 10, g_y = 5;          // 현재 캐릭터 좌표
int g_prevX = 10, g_prevY = 5;  // 이전 프레임의 캐릭터 좌표 (지우기 용도)
int g_isRunning = 1;            // 루프 제어
char g_inputKey = 0;            // 입력된 키 저장

// --- [기능별 함수화 (I-U-R 구조)] ---

// 특정 위치로 커서를 즉시 이동시키는 함수 (ANSI 방식)
void MoveCursor(int row, int col)
{
    // \x1b[ : ANSI 시작 / %d;%dH : 행;열 이동 / H : 명령 끝
    printf("\x1b[%d;%dH", row + 1, col + 1);
}

// 1. 입력 (Input)
void ProcessInput()
{
    if (_kbhit()) 
    { 
        if(!g_inputKey)
            g_inputKey = _getch(); 
    }
    else { g_inputKey = 0; }
}

// 2. 업데이트 (Update)
void Update()
{
    // 이동 전 위치를 저장 (나중에 이곳만 콕 집어서 지우기 위해)
    g_prevX = g_x;
    g_prevY = g_y;

    if (g_inputKey == 'w' || g_inputKey == 'W') g_y--;
    else if (g_inputKey == 's' || g_inputKey == 'S') g_y++;
    else if (g_inputKey == 'a' || g_inputKey == 'A') g_x--;
    else if (g_inputKey == 'd' || g_inputKey == 'D') g_x++;
    else if (g_inputKey == 'q' || g_inputKey == 'Q') g_isRunning = 0;

    // 경계 검사 (Wall Collision)
    if (g_x < 0) g_x = 0; if (g_x > 45) g_x = 45;
    if (g_y < 0) g_y = 0; if (g_y > 20) g_y = 20;
}

// 3. 렌더링 (Render)
void Render() {

    // UI 영역 (고정 위치 렌더링)
    MoveCursor(0, 0);
    printf("=== 게임루프 최적화 실습 (WASD 이동 / Q 종료) ===\n");
    printf(" 현재 좌표 - X: %d, Y: %d \n", g_x, g_y);
    printf("-------------------------------------------------\n");

    // [최적화 핵심] 위치가 변했을 때만 이전 위치를 공백으로 지움
    if (g_x != g_prevX || g_y != g_prevY)
    {
        MoveCursor(g_prevY + 3, g_prevX);
        printf("     "); // 이전 캐릭터 자리를 지우기
    }

    // 새 위치에 캐릭터 그리기
    MoveCursor(g_y + 3, g_x);
    printf("(*_*)\n");




}

int main() {

    system("cls"); // 시작할 때 딱 한 번만 화면을 깨끗이 지움

    /*
    * =========================================================================
    * [ANSI Escape Sequence: 콘솔 커서 제어 스택]
    * =========================================================================
    * 1. \x1b (또는 \033): ESC(Escape) 문자를 의미하는 16진수 코드.
    *  - 제어 시퀀스의 시작을 알리는 신호탄임.
    * 2. [?25: 커서(Cursor)와 관련된 설정을 건드리겠다는 ID 번호.
    * 3. l (Lowercase L): 'Low' 혹은 'Lock'의 약자로, 기능을 끄거나 숨긴다는 뜻.
    *  - \x1b[?25l -> 커서 숨기기 (Hide Cursor)
    * 4. h (Lowercase H): 'High'의 약자로, 기능을 켜거나 다시 보인다는 뜻.
    *  - \x1b[?25h -> 커서 보이기 (Show Cursor)
    *
    * 주의사항:
    *  - 프로그램이 비정상 종료되면 커서가 숨겨진 상태로 유지될 수 있음.
    *  - 반드시 프로그램 종료 직전이나 return 0 전에는 \x1b[?25h를 출력해줘야 함.
    * =========================================================================
    */
    printf("\x1b[?25l"); // 커서가 깜빡거리면 보기 싫으므로 숨김

    // [메인 게임 루프]
    while (g_isRunning) {
        ProcessInput(); // 1. 듣기
        Update();       // 2. 생각하기
        Render();       // 3. 말하기(그리기)

        Sleep(10);
    }

    // 종료 시 설정 복구
    printf("\x1b[?25h"); // 커서 다시 보이기
    system("cls");
    printf("게임을 종료합니다.\n");

    return 0;
}