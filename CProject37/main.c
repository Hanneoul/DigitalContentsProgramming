#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <mmsystem.h> 
#include <conio.h>   
#include <time.h>

// 윈도우 멀티미디어 라이브러리 링크 (PlaySound 함수 구동용)
#pragma comment(lib, "winmm.lib")

// 게임 캐릭터 상태 정의
#define STATE_GUARD  0
#define STATE_ATTACK 1
#define STATE_HIT    2

// ANSI 이스케이프 컬러 매크로
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

/**
 * @brief 콘솔 창의 특정 (X, Y) 좌표로 커서를 이동시키는 함수
 * @param x 가로 좌표
 * @param y 세로 좌표
 */
void gotoxy(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main(void)
{
    // 실행할 때마다 다른 패턴을 위한 난수 시드 초기화
    srand((unsigned int)time(NULL));

    // 게임 핵심 연산용 내부 변수 선언
    int player_hp = 3;
    int ai_hp = 3;

    int player_state = STATE_GUARD;
    int ai_state = STATE_GUARD;

    int current_ai_taunt = 0;
    int current_player_shout = 0;
    int is_player_shouting = 0;

    // 타이머 및 후딜레이 제어 변수들 (<time.h>)
    clock_t current_time;
    clock_t ai_last_action = clock();
    clock_t ai_cooldown = CLOCKS_PER_SEC * 1.5; // 최초 AI 행동 대기 시간
    clock_t last_heartbeat = clock();

    // 공격 지속 및 후딜레이 판정용 독립 타이머 변수
    clock_t player_attack_start_time = 0;
    clock_t ai_attack_start_time = 0;

    // -------------------------------------------------------------------------
    // [밸런스 셋업] 공방 후 빈틈(무가드 상태) 유지 시간 설정
    // -------------------------------------------------------------------------
    // AI의 공격 후 빈틈: 약 500ms (0.3초) 유지
    const clock_t AI_ATTACK_DURATION = (CLOCKS_PER_SEC * 3) / 10;
    // 플레이어의 공격 후 빈틈: 1000ms (1초) 유지하여 실패 시 패널티 부과
    const clock_t PLAYER_ATTACK_DURATION = CLOCKS_PER_SEC;

    // 콘솔 기본 세팅 (텍스트 깜빡임 방지를 위한 커서 완전 숨기기)
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    // 무버퍼 지향 고속 덮어쓰기를 위해 실행 시 최초 1회 화면 전체 청소
    printf("\x1b[2J");

    // 변하지 않는 고정 레이아웃 UI 최초 렌더링
    gotoxy(0, 0);
    printf("=======================================================================\n");
    printf(" [ 1:1 초근접 심리 데스매치 ]  대기=가드 | 스페이스=공격(후딜 10배 주의!)\n");
    printf("=======================================================================\n");
    gotoxy(0, 14);
    printf("=======================================================================\n");
    printf(" Q: 게임 빡종하고 도망치기\n");

    // -------------------------------------------------------------------------
    // 메인 게임 루프 엔진 시작
    // -------------------------------------------------------------------------
    while (player_hp > 0 && ai_hp > 0)
    {
        current_time = clock();

        // [배경 사운드] 0.5초 주기로 긴장감을 주는 기본 심장박동음 출력 (비동기)
        if ((current_time - last_heartbeat) >= (CLOCKS_PER_SEC / 2))
        {
            PlaySound(TEXT("SystemDefault"), NULL, SND_ALIAS | SND_ASYNC);
            last_heartbeat = current_time;
        }

        // 1. 플레이어 상태 제어 및 실시간 키 입력 처리
        if (player_state != STATE_HIT && player_state != STATE_ATTACK)
        {
            player_state = STATE_GUARD;
        }

        // 플레이어의 공격 후딜레이(무가드 허점) 시간 만료 체크 루프
        if (player_state == STATE_ATTACK)
        {
            if ((current_time - player_attack_start_time) >= PLAYER_ATTACK_DURATION)
            {
                player_state = STATE_GUARD; // 지정된 후딜레이 초과 시 가드로 복귀
                is_player_shouting = 0;
            }
        }

        // 비동기 키 입력 감지
        if (_kbhit())
        {
            int key = _getch();
            if (key == 'q' || key == 'Q')
            {
                break;
            }

            // 가드 상태일 때만 스페이스바를 눌러 공격 전환 가능
            if (key == ' ' && player_state == STATE_GUARD)
            {
                player_state = STATE_ATTACK;
                player_attack_start_time = current_time;
                current_player_shout = rand() % 5;
                is_player_shouting = 1;

                // [교차 타격 연산] 내가 공격한 타이밍에 AI가 가드를 풀고 공격/피격 중이었는지 판단
                if (ai_state != STATE_GUARD && ai_state != STATE_HIT)
                {
                    ai_hp--;
                    ai_state = STATE_HIT;

                    // AI 피격 위치에 스파크 이펙트 마킹 및 타격 사운드 출력
                    gotoxy(22, 8);
                    printf(ANSI_MAGENTA "★" ANSI_RESET);
                    PlaySound(TEXT("SystemQuestion"), NULL, SND_ALIAS | SND_ASYNC);
                    Sleep(150);
                }
            }
        }

        // 2. AI 상태 엔진 및 후딜레이 타이머 제어
        if (ai_state != STATE_HIT && ai_state != STATE_ATTACK)
        {
            ai_state = STATE_GUARD;
        }

        // AI의 공격 지속 시간(빈틈) 만료 체크 루프
        if (ai_state == STATE_ATTACK)
        {
            if ((current_time - ai_attack_start_time) >= AI_ATTACK_DURATION)
            {
                ai_state = STATE_GUARD;
            }
        }

        // AI 행동 타이머 검사 및 무작위 패턴 분기
        if ((current_time - ai_last_action) >= ai_cooldown)
        {
            // 60% 확률로 플레이어의 빈틈을 노리는 기습 펀치 가동
            if (rand() % 10 < 6)
            {
                if (ai_state == STATE_GUARD)
                {
                    ai_state = STATE_ATTACK;
                    ai_attack_start_time = current_time;

                    // AI가 침과 동시에 유저가 공격 후딜(무가드) 상태였다면 피격 판정
                    if (player_state == STATE_ATTACK)
                    {
                        player_hp--;
                        player_state = STATE_HIT;

                        // 플레이어 캐릭터 위치에 피격 인디케이터 및 경고음 방출
                        gotoxy(22, 8);
                        printf(ANSI_RED "★" ANSI_RESET);
                        PlaySound(TEXT("SystemQuestion"), NULL, SND_ALIAS | SND_ASYNC);
                        Sleep(150);
                    }
                }
            }
            // 40% 확률로 멘탈을 흔드는 텍스트 도발 갱신
            else
            {
                current_ai_taunt = rand() % 5;
            }
            // 다음 패턴 발동을 위한 변칙 쿨타임 무작위 재설정
            ai_last_action = current_time;
            ai_cooldown = CLOCKS_PER_SEC * (0.8 + (rand() % 10) / 10.0);
        }

        // ---------------------------------------------------------------------
        // 3. 고정 좌표 타겟팅 기반 무버퍼 실시간 프레임 렌더링 영역
        // ---------------------------------------------------------------------

        // [UI 렌더링] 실시간 체력 상황판 스코어 오버레이
        gotoxy(0, 3);
        printf(" PLAYER HP: ");
        for (int i = 0; i < 3; i++)
        {
            printf(i < player_hp ? ANSI_GREEN "★ " ANSI_RESET : "☆ ");
        }
        printf("      |      AI HP: ");
        for (int i = 0; i < 3; i++)
        {
            printf(i < ai_hp ? ANSI_RED "★ " ANSI_RESET : "☆ ");
        }
        printf("        "); // 잔상 소거 패딩

        // [UI 렌더링] AI 전용 상단 도발 대사 출력창 (Y:5, 노란색 컬러 배정)
        gotoxy(20, 5);
        printf(ANSI_YELLOW "[AI 도발]: %s" ANSI_RESET, ai_taunts[current_ai_taunt]);

        // [격투 렌더링] 7라인: 머리 파트 대칭 상태 표정 변화 제어
        gotoxy(15, 7);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "( >_<)" ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "( x_x)" ANSI_RESET);
        else                                   printf(ANSI_GREEN "( o_o)" ANSI_RESET);

        gotoxy(25, 7);
        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "(>_< )" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "(x_x )" ANSI_RESET);
        else                                   printf(ANSI_BLUE "(o_o )" ANSI_RESET);

        // [격투 렌더링] 8라인: 몸통 및 펀치 투사 프레임 제어
        gotoxy(15, 8);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "( ? )-o  " ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "( o )o   " ANSI_RESET);
        else                                   printf(ANSI_GREEN "( ? )?  " ANSI_RESET);

        gotoxy(22, 8); // 충돌 판정 및 타격 지점 공백 자동 보정 좌표
        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "  o-( ? )" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "   o( o )" ANSI_RESET);
        else                                   printf(ANSI_BLUE "   ?( ? )" ANSI_RESET);

        // [격투 렌더링] 9라인: 캐릭터 하체 지지대 아스키아트 드로잉
        gotoxy(15, 9);
        if (player_state == STATE_ATTACK)      printf(ANSI_CYAN "/  \\" ANSI_RESET);
        else if (player_state == STATE_HIT)    printf(ANSI_RED "/  \\" ANSI_RESET);
        else                                   printf(ANSI_GREEN "/  \\" ANSI_RESET);

        gotoxy(27, 9);
        if (ai_state == STATE_ATTACK)          printf(ANSI_RED "/  \\" ANSI_RESET);
        else if (ai_state == STATE_HIT)        printf(ANSI_MAGENTA "/  \\" ANSI_RESET);
        else                                   printf(ANSI_BLUE "/  \\" ANSI_RESET);

        // [UI 렌더링] 플레이어 전용 하단 기합 대사 출력창 (Y:11, 시안색 컬러 배정)
        gotoxy(15, 11);
        if (is_player_shouting)
        {
            printf(ANSI_CYAN "[나의 기합]: %s" ANSI_RESET, player_shouts[current_player_shout]);
        }
        else
        {
            printf("                                                                     "); // 이전 대사 흔적 청소
        }

        // [UI 렌더링] 플레이어 극대 후딜레이(무가드 위험) 시각 인디케이터 경고창
        gotoxy(0, 13);
        if (player_state == STATE_ATTACK)
        {
            printf(ANSI_RED " [경고] 공격 후 극대 딜레이 적용 중! (무가드 상태) " ANSI_RESET);
        }
        else
        {
            printf("                                                                     ");
        }

        // 단발성 프레임 연출(HIT) 복구 시스템 연산
        if (player_state == STATE_HIT)
        {
            player_state = STATE_GUARD;
            is_player_shouting = 0;
        }
        if (ai_state == STATE_HIT)
        {
            ai_state = STATE_GUARD;
        }

        // 메인 프레임 동기화를 위한 물리적 딜레이 제어 (약 30ms 주행)
        Sleep(30);
    }

    // -------------------------------------------------------------------------
    // 4. 게임 루프 종료 연산 및 결과 시그널 멜로디 제어 화면
    // -------------------------------------------------------------------------
    printf("\x1b[2J"); // 화면 최종 클리어
    gotoxy(0, 5);
    printf("====================================================\n");

    if (player_hp <= 0)
    {
        printf(ANSI_RED " [GAME OVER] 처참하게 패배함.\n" ANSI_RESET);
        printf("====================================================\n\n");
        PlaySound(TEXT("SystemExit"), NULL, SND_ALIAS | SND_ASYNC);
        Sleep(1500);
    }
    else if (ai_hp <= 0)
    {
        printf(ANSI_GREEN " [VICTORY] 뎀프시롤!!! 완벽하게 승리함! \n" ANSI_RESET);
        printf("====================================================\n\n");
        PlaySound(TEXT("SystemAsterisk"), NULL, SND_ALIAS | SND_ASYNC);
        Sleep(1500);
    }
    else
    {
        printf(" 비겁하게 도망쳐 무효 처리됨.\n====================================================\n\n");
    }

    return 0;
}