#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h> 
#include <mmsystem.h> 
#include <conio.h>   
#include <time.h>

#pragma comment(lib, "winmm.lib")

// 게임 캐릭터 상태 정의
#define STATE_GUARD 0
#define STATE_ATTACK 1
#define STATE_HIT 2

// ANSI 컬러 매크로
#define ANSI_RESET   "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"

// AI 킹받는 도발 대사 리스트 (5가지)
const char* ai_taunts[5] = {
    "포스만 보면 실버인데 손가락은 브론즈냐? ㅋㅋ        ",
    "스페이스바 부서지겠다 임마 타이밍 개못 맞추네!     ",
    "야야, 가만히 서있으니까 마네킹인 줄 알았잖아~      ",
    "너 혹시 공격 키가 뭔지 모르는 거 아니지?           ",
    "하품 나온다 하품 나와... 형 피지컬 장난 아니다?    "
};

// 플레이어 공격 시 출력될 대사 리스트 (5가지)
const char* player_shouts[5] = {
    "이게 바로 신의 컨트롤이다!! 받아라!                ",
    "뚝배기 깨지는 소리 여기까지 들리쥬?                ",
    "방어 풀렸다 임마! 일로와!                           ",
    "지방대 불주먹의 매운맛을 보여주마!!                 ",
    "어딜 도망가? 확 그냥!                               "
};

// 고정 좌표 이동 함수
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main(void) {
    srand((unsigned int)time(NULL));

    int player_hp = 3;
    int ai_hp = 3;

    int player_state = STATE_GUARD;
    int ai_state = STATE_GUARD;

    int current_ai_taunt = 0;
    int current_player_shout = 0;
    bool is_player_shouting = false;

    // 타이머 및 후딜레이 제어 변수들 (<time.h>)
    clock_t current_time;
    clock_t ai_last_action = clock();
    clock_t ai_cooldown = CLOCKS_PER_SEC * 1.5;
    clock_t last_heartbeat = clock();

    // 공격 지속 및 후딜레이 판정용 독립 타이머
    clock_t player_attack_start_time = 0;
    clock_t ai_attack_start_time = 0;

    // 밸런스 핵심 상수의 밀리초(ms) 환산 틱 정의
    // AI의 공격 후 빈틈: 기존 대비 2배 느려짐 (약 500ms 유지)
    const clock_t AI_ATTACK_DURATION = (CLOCKS_PER_SEC * 5) / 10;
    // 플레이어의 공격 후 빈틈: AI의 10배에 달하는 초거대 후딜레이 (약 1000ms = 1초 유지)
    const clock_t PLAYER_ATTACK_DURATION = CLOCKS_PER_SEC;

    // 콘솔 기본 세팅 (커서 완전 숨기기)
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // 무버퍼 지향을 위한 화면 최초 1회 초기화
    printf("\x1b[2J");

    // 고정 인터페이스 UI 드로잉
    gotoxy(0, 0);
    printf("=======================================================================\n");
    printf(" [ 1:1 초근접 심리 데스매치 ]  대기=가드 | 스페이스=공격(후딜 10배 주의!)\n");
    printf("=======================================================================\n");
    gotoxy(0, 14);
    printf("=======================================================================\n");
    printf(" Q: 게임 빡종하고 도망치기\n");

    // 메인 루프 진입
    while (player_hp > 0 && ai_hp > 0) {
        current_time = clock();

        // ---------------------------------------------------------------------
        // [사운드 시스템] 0.5초 주기 배경 심장박동음 가동 (비동기)
        // ---------------------------------------------------------------------
        if ((current_time - last_heartbeat) >= (CLOCKS_PER_SEC / 2)) {
            PlaySound(TEXT("SystemDefault"), NULL, SND_ALIAS | SND_ASYNC);
            last_heartbeat = current_time;
        }

        // 1. 실시간 키보드 입력 및 유저 상태 엔진
        if (player_state != STATE_HIT && player_state != STATE_ATTACK) {
            player_state = STATE_GUARD;
        }

        // 플레이어의 공격 후딜레이(무가드) 시간 만료 체크
        if (player_state == STATE_ATTACK) {
            if ((current_time - player_attack_start_time) >= PLAYER_ATTACK_DURATION) {
                player_state = STATE_GUARD; // 긴 후딜레이가 끝나야 가드로 복귀
                is_player_shouting = false;
            }
        }

        if (_kbhit()) {
            int key = _getch();
            if (key == 'q' || key == 'Q') break;

            // 선행 공격 상태나 피격 상태가 아닐 때만 공격 가능
            if (key == ' ' && player_state == STATE_GUARD) {
                player_state = STATE_ATTACK;
                player_attack_start_time = current_time;
                current_player_shout = rand() % 5;
                is_player_shouting = true;

                // 플레이어 자력 공격 시 쿨타임 중인 AI와 교차 판정 체크
                if (ai_state != STATE_GUARD && ai_state != STATE_HIT) {
                    // AI가 무가드 상태(공격 후딜)일 때 내가 쳤으므로 적중!
                    ai_hp--;
                    ai_state = STATE_HIT;

                    // 타격 연출 오버레이 및 사운드
                    gotoxy(22, 8); printf(ANSI_MAGENTA "★" ANSI_RESET);
                    PlaySound(TEXT("SystemQuestion"), NULL, SND_ALIAS | SND_ASYNC);
                    Sleep(150);
                    
                }
            }
        }

        // 2. AI 상태 엔진 및 후딜레이 제어
        if (ai_state != STATE_HIT && ai_state != STATE_ATTACK) {
            ai_state = STATE_GUARD;
        }

        // AI의 공격 지속 시간(무가드) 만료 체크
        if (ai_state == STATE_ATTACK) {
            if ((current_time - ai_attack_start_time) >= AI_ATTACK_DURATION) {
                ai_state = STATE_GUARD;
            }
        }

        // AI 공격 타이머 및 도발 주기 제어
        if ((current_time - ai_last_action) >= ai_cooldown) {
            // 60% 확률로 기습 돌진 공격, 40% 확률로 도발 갱신
            if (rand() % 10 < 6) {
                if (ai_state == STATE_GUARD) {
                    ai_state = STATE_ATTACK;
                    ai_attack_start_time = current_time;

                    // AI가 주먹을 뻗는 순간 플레이어가 후딜레이(STATE_ATTACK) 상태라면 적중!
                    if (player_state == STATE_ATTACK) {
                        player_hp--;
                        player_state = STATE_HIT;

                        // 피격 연출 오버레이 및 사운드
                        gotoxy(22, 8); printf(ANSI_RED "★" ANSI_RESET);
                        PlaySound(TEXT("SystemQuestion"), NULL, SND_ALIAS | SND_ASYNC);
                        Sleep(150);
                        
                    }
                }
            }
            else {
                current_ai_taunt = rand() % 5; // 도발 대사 변경
            }
            ai_last_action = current_time;
            ai_cooldown = CLOCKS_PER_SEC * (0.8 + (rand() % 10) / 10.0);
        }

        // 3. 무버퍼 고정 좌표 실시간 갱신 드로잉 (fflush 완전 제거 버전)

        // HP 스코어보드 갱신
        gotoxy(0, 3);
        printf(" PLAYER HP: ");
        for (int i = 0; i < 3; i++) printf(i < player_hp ? ANSI_GREEN "★ " ANSI_RESET : "☆ ");
        printf("      |      AI HP: ");
        for (int i = 0; i < 3; i++) printf(i < ai_hp ? ANSI_RED "★ " ANSI_RESET : "☆ ");
        printf("        ");

        // [컴퓨터 상단 도발 대사창 구현 (Y:5, 노란색)]
        gotoxy(20, 5);
        printf(ANSI_YELLOW "[AI 도발]: %s" ANSI_RESET, ai_taunts[current_ai_taunt]);

        // [초근접 마주보기 격투 렌더링 영역 (Y: 7~9)]

        // 7라인: 머리 및 표정 연출
        gotoxy(15, 7);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "( >_<)" ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "( x_x)" ANSI_RESET);
        else                                   printf(ANSI_GREEN "( o_o)" ANSI_RESET);
        
        gotoxy(25, 7);
        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "(>_< )" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "(x_x )" ANSI_RESET);
        else                                   printf(ANSI_BLUE "(o_o )" ANSI_RESET);

        // 8라인: 몸통 및 주먹 구조 드로잉
        gotoxy(15, 8);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "( ? )-o  " ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "( o )o   " ANSI_RESET);
        else                                   printf(ANSI_GREEN "( ? )?  " ANSI_RESET);

        gotoxy(22, 8);

        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "  o-( ? )" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "   o( o )" ANSI_RESET);
        else                                   printf(ANSI_BLUE "   ?( ? )" ANSI_RESET);

        // 9라인: 하체 드로잉
        gotoxy(15, 9);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "/  \\" ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "/  \\" ANSI_RESET);
        else                                   printf(ANSI_GREEN "/  \\" ANSI_RESET);
        
        gotoxy(27, 9);
        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "/  \\" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "/  \\" ANSI_RESET);
        else                                   printf(ANSI_BLUE "/  \\" ANSI_RESET);
        

        // [플레이어 하단 기합 대사창 구현 (Y:11, 시안색)]
        gotoxy(15, 11);
        if (is_player_shouting) {
            printf(ANSI_CYAN "[나의 기합]: %s" ANSI_RESET, player_shouts[current_player_shout]);
        }
        else {
            printf("                                                                     "); // 공백 청소
        }

        // 후딜레이 상태 연출용 인디케이터 게이지 보정
        gotoxy(0, 13);
        if (player_state == STATE_ATTACK) {
            printf(ANSI_RED " [경고] 공격 후 극대 딜레이 적용 중! (무가드 상태) " ANSI_RESET);
        }
        else {
            printf("                                                     ");
        }

        // 상태 회복 연산 알고리즘 처리
        if (player_state == STATE_HIT) { player_state = STATE_GUARD; is_player_shouting = false; }
        if (ai_state == STATE_HIT) ai_state = STATE_GUARD;

        Sleep(30); // 20~30 FPS 내부 물리 주행 타이밍 고정
    }

    // 4. 게임 종료 스크린 및 시그널 멜로디 제어
    printf("\x1b[2J");
    gotoxy(0, 5);
    printf("====================================================\n");
    if (player_hp <= 0) {
        printf(ANSI_RED " [GAME OVER] 처참하게 패배함.\n" ANSI_RESET);
        printf("====================================================\n\n");
        PlaySound(TEXT("SystemExit"), NULL, SND_ALIAS | SND_ASYNC);
        Sleep(1500);
    }
    else if (ai_hp <= 0) {
        printf(ANSI_GREEN " [VICTORY] 뎀프시롤!!! 완벽하게 승리함! \n" ANSI_RESET);
        printf("====================================================\n\n");
        PlaySound(TEXT("SystemAsterisk"), NULL, SND_ALIAS | SND_ASYNC);
        Sleep(1500);
    }
    else {
        printf(" 비겁하게 도망쳐 무효 처리됨.\n====================================================\n\n");
    }

    return 0;
}