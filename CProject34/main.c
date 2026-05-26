



/*
=============================================================================
[강의 자료: scanf()를 활용한 문자열 분석과 ANSI 화면 제어]
=============================================================================

1. 문자열을 입력받는 scanf("%s")의 원리
   - %s 서식 지정자는 공백(스페이스, 탭, 엔터)을 '데이터를 구분하는 기준'으로 사용함.
   - 예: scanf("%s %s", action, position); 문장에 "공격 상단"이라고 입력하고 엔터를 치면?
     -> 첫 번째 %s가 공백 앞의 "공격"을 읽어서 action 변수에 저장함.
     -> 두 번째 %s가 공백 뒤의 "상단"을 읽어서 position 변수에 저장함.
   - 이 성질을 이용하면 사용자가 문장 형태로 입력한 명령어를 단어별로 쪼개어 분석할 수 있음.
   - ※ 주의: 문자열을 저장하는 char 배열은 그 자체로 주소(Address) 역할을 하므로,
     scanf에 넘겨줄 때 변수 앞에 '&' 기호들을 붙이지 않음.

2. 입력받은 문자열 비교하기: strcmp()
   - C언어에서는 `if (action == "공격")` 처럼 비교 연산자(==)로 문자열의 내용을 직접 비교할 수 없음.
   - <string.h> 헤더에서 제공하는 strcmp(문자열1, 문자열2) 함수를 사용해야 함.
   - 두 문자열의 내용이 완전히 일치하면 정수 0을 반환함 -> `if (strcmp(action, "공격") == 0)`

3. ANSI Escape Sequence를 이용한 화면 동적 제어
   - 콘솔창은 원래 아래로만 글자가 내려가지만, 특수 기호 묶음(\033[...)을 출력하면
     커서의 위치를 바꾸거나 화면을 지우고 글자 색상을 바꿀 수 있음.
   - \033[2J : 콘솔 화면 전체를 깨끗하게 지움 (Clear Screen)
   - \033[H  : 커서를 화면의 최상단 좌측(0,0 위치)으로 이동시킴. 이를 이용해 화면을 덮어씌움.
   - \033[행;열H : 커서를 특정 행과 열 위치로 다이렉트 이동시킴 (타격 이펙트 연출에 필수)
   - 색상 코드 예시: \033[1;31m (밝은 빨간색 글자), \033[0m (색상 초기화)

4. 게임 시스템 규칙 캡슐화
   - 턴이 시작되면 플레이어의 입력을 받은 후, 컴퓨터(AI)의 행동을 난수(rand)로 결정함.
   - 두 캐릭터의 행동(공격/방어)과 위치(상단/하단)가 매칭되는 조건문을 통해 데미지 공식을 적용함.
=============================================================================
*/
#define _CRT_SECURE_NO_WARNINGS // 최신 Visual Studio에서 보안 경고를 무시함
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

// [ANSI Escape Sequence 매크로 정의] - 가독성을 높이기 위함
#define CLEAR_SCREEN() printf("\033[2J\033[H")
#define MOVE_CURSOR(row, col) printf("\033[%d;%dH", row, col)
#define COLOR_RED()    printf("\033[1;31m")
#define COLOR_BLUE()   printf("\033[1;34m")
#define COLOR_YELLOW() printf("\033[1;33m")
#define COLOR_RESET()  printf("\033[0m")

int main()
{
    // 난수 생성기 초기화 (컴퓨터의 무작위 행동 결정용)
    srand((unsigned int)time(NULL));

    // [체력 상태 변수]
    int player_hp = 100;
    int enemy_hp = 100;

    // [문자열 입력 저장 공간] - 한글은 1글자당 최소 2~3바이트를 차지하므로 넉넉하게 배열 크기 지정
    char action[20];   // "공격" 또는 "방어"가 저장될 공간
    char position[20]; // "상단" 또는 "하단"이 저장될 공간

    // 컴퓨터(AI)의 행동 프로필 변수
    int enemy_action;   // 0: 공격, 1: 방어
    int enemy_position; // 0: 상단, 1: 하단

    // 시스템 메시지 기록용 변수
    char log_player[100] = "전투 준비 완료!";
    char log_enemy[100] = "적이 무기를 겨눕니다.";
    char log_damage[100] = "";

    // 전투 루프 (두 사람 중 한 명의 체력이 0이 될 때까지)
    while (player_hp > 0 && enemy_hp > 0) {

        // 1. 화면 전체를 먼저 비우고 상단부터 아스키 아트와 정보를 리프레시함
        CLEAR_SCREEN();

        printf("===============================================================\n");
        printf("           C언어 텍스트 격투 게임 (scanf 문자열 분석)          \n");
        printf("===============================================================\n\n");

        // 2. 플레이어와 적의 체력 바 시각화
        printf(" [PLAYER] HP: %3d / 100           [ENEMY] HP: %3d / 100\n", player_hp, enemy_hp);
        COLOR_RED();
        printf(" ["); for (int i = 0; i < player_hp / 5; i++) printf("■"); for (int i = player_hp / 5; i < 20; i++) printf(" "); printf("]");
        COLOR_BLUE();
        printf("   ["); for (int i = 0; i < enemy_hp / 5; i++) printf("■"); for (int i = enemy_hp / 5; i < 20; i++) printf(" "); printf("]\n");
        COLOR_RESET();

        // 3. 전투 아스키 아트 (상단 대치 상태 그래픽)
        printf("\n");
        printf("        (o_o)                                 (X_X)        \n");
        printf("        / | \\  [상단]                         / | \\        \n");
        printf("         | |                                   | |         \n");
        printf("        ---------                             ---------\n");
        printf("        /     \\ [하단]                         /     \\       \n");
        printf("       |       |                             |       |     \n");
        printf("\n");

        // 4. 이전 턴의 전투 로그 출력
        printf("---------------------------------------------------------------\n");
        printf(" * 플레이어 행동: %s\n", log_player);
        printf(" * 에너미 행동: %s\n", log_enemy);
        COLOR_YELLOW();
        printf(" [전투 결과] %s\n", log_damage);
        COLOR_RESET();
        printf("---------------------------------------------------------------\n\n");

        // 5. 커맨드 입력 프롬프트
        printf("※ 커맨드 입력 예시: [공격 상단] [공격 하단] [방어 상단] [방어 하단]\n");
        printf(">> 명령을 입력하세요: ");

        // 문자열 두 개를 공백 기준으로 분석하여 각각 action과 position 배열에 채워넣음
        if (scanf("%s %s", action, position) != 2) {
            // 올바르게 두 단어가 입력되지 않았을 경우 입력 버퍼를 비우고 재시도
            while (getchar() != '\n');
            strcpy(log_damage, "올바른 형태로 두 단어(행동 위치)를 입력하십시오.");
            continue;
        }

        // 6. 플레이어 입력 검증 및 정수형 데이터로 내부 치환 (계산 편의성 확보)
        int p_act = -1, p_pos = -1;
        if (strcmp(action, "공격") == 0) p_act = 0;
        else if (strcmp(action, "방어") == 0) p_act = 1;

        if (strcmp(position, "상단") == 0) p_pos = 0;
        else if (strcmp(position, "하단") == 0) p_pos = 1;

        if (p_act == -1 || p_pos == -1) {
            strcpy(log_damage, "잘못된 단어가 포함되어 있습니다. (공격/방어, 상단/하단 조합만 가능)");
            continue;
        }

        // 플레이어 행동을 로그 문자열로 포맷팅 저장
        sprintf(log_player, "%s [%s]", action, position);

        // 7. 적(AI)의 행동 무작위 결정
        enemy_action = rand() % 2;   // 0 또는 1
        enemy_position = rand() % 2; // 0 또는 1
        sprintf(log_enemy, "%s [%s]", (enemy_action == 0 ? "공격" : "방어"), (enemy_position == 0 ? "상단" : "하단"));

        // 기본 데미지 설정 (수치 조율 가능)
        int base_dmg = 10;
        int p_take_dmg = 0;
        int e_take_dmg = 0;
        int effect_trigger = 0; // 이펙트를 어느 위치에 그릴지 판단 (1: 플레이어 피격, 2: 에너미 피격, 3: 둘 다 피격, 4: 방어 효과)

        // 8. 전투 핵심 룰 로직 분기 처리
        if (p_act == 0 && enemy_action == 0) {
            // [룰 1] 둘 다 동시에 공격한 경우
            if (p_pos == enemy_position) {
                // 공격 위치까지 겹치면 크로스 카운터, 둘 다 200% 데미지
                p_take_dmg = base_dmg * 2;
                e_take_dmg = base_dmg * 2;
                sprintf(log_damage, "★크로스 카운터!★ 같은 위치를 정면 공격하여 서로 200%%의 피해(%d)를 입었습니다.", base_dmg * 2);
                effect_trigger = 3;
            }
            else {
                // 공격 위치가 다르면 각자 일반 공격 성공 (서로 100% 데미지)
                p_take_dmg = base_dmg;
                e_take_dmg = base_dmg;
                sprintf(log_damage, "서로 빈 곳을 찔렀습니다. 각자 100%%의 피해(%d)를 입었습니다.", base_dmg);
                effect_trigger = 3;
            }
        }
        else if (p_act == 0 && enemy_action == 1) {
            // [룰 2] 플레이어 공격 vs 적 방어
            if (p_pos == enemy_position) {
                // 적이 방어 방향을 맞춤 -> 적은 데미지 50%만 입음
                e_take_dmg = base_dmg / 2;
                effect_trigger = 4;
                // 50% 확률로 반격 성공 여부 결정
                if (rand() % 2 == 0) {
                    p_take_dmg = (int)(base_dmg * 1.5); // 플레이어 반격 당함 (150% 데미지)
                    sprintf(log_damage, "적의 방어 성공! 데미지가 50%%로 반감되고 역습을 허용하여 플레이어가 150%% 반격 피해(%d)를 입음.", p_take_dmg);
                    effect_trigger = 1;
                }
                else {
                    sprintf(log_damage, "적이 성공적으로 가드했습니다. 데미지 50%% 경감(%d). 반격은 빗나갔습니다.", e_take_dmg);
                }
            }
            else {
                // 적이 방어 방향을 틀림 -> 가드 무너짐, 카운터 판정으로 적이 150% 데미지 받음
                e_take_dmg = (int)(base_dmg * 1.5);
                sprintf(log_damage, "방어 방향 예측 실패! 적의 가드가 깨지며 150%% 카운터 피해(%d)를 입혔습니다.", e_take_dmg);
                effect_trigger = 2;
            }
        }
        else if (p_act == 1 && enemy_action == 0) {
            // [룰 3] 플레이어 방어 vs 적 공격
            if (p_pos == enemy_position) {
                // 플레이어가 가드 방향을 맞춤 -> 플레이어 데미지 50% 경감
                p_take_dmg = base_dmg / 2;
                effect_trigger = 4;
                // 50% 확률로 반격 성공
                if (rand() % 2 == 0) {
                    e_take_dmg = (int)(base_dmg * 1.5); // 적에게 150% 반격 성공
                    sprintf(log_damage, "방어 성공! 철벽 가드 후 번개 같은 반격으로 적에게 150%%의 피해(%d)를 돌려주었습니다.", e_take_dmg);
                    effect_trigger = 2;
                }
                else {
                    sprintf(log_damage, "가드 성공! 적의 공격력을 절반(%d)으로 줄였습니다.", p_take_dmg);
                }
            }
            else {
                // 플레이어가 가드 방향을 틀림 -> 플레이어가 150% 카운터 피해를 입음
                p_take_dmg = (int)(base_dmg * 1.5);
                sprintf(log_damage, "방어 방향 예측 실패! 무방비하게 노출되어 적에게 150%% 카운터 피해(%d)를 허용했습니다.", p_take_dmg);
                effect_trigger = 1;
            }
        }
        else {
            // [룰 4] 둘 다 방어 태세를 취한 경우
            sprintf(log_damage, "두 파이터 모두 숨을 고르며 방어 자세를 유지합니다. 아무 일도 일어나지 않았습니다.");
        }

        // 실제 체력 차감 연산
        player_hp -= p_take_dmg;
        enemy_hp -= e_take_dmg;
        if (player_hp < 0) player_hp = 0;
        if (enemy_hp < 0) enemy_hp = 0;

        // 9. 타격 발생 위치에 ANSI Sequence를 사용하여 이펙트(*) 덮어쓰기 연출
        COLOR_YELLOW();
        if (effect_trigger == 1 || effect_trigger == 3) {
            // 플레이어 피격 위치 (아스키 아트상의 대략적 좌표 9행 부근에 오버레이)
            MOVE_CURSOR(9, 9);  printf("<💥CRASH!!>");
            MOVE_CURSOR(10, 8); printf("* * * *");
        }
        if (effect_trigger == 2 || effect_trigger == 3) {
            // 적 피격 위치 (아스키 아트상의 대략적 좌표 9행 부근 우측 오버레이)
            MOVE_CURSOR(9, 47);  printf("<💥CRASH!!>");
            MOVE_CURSOR(10, 46); printf("* * * *");
        }
        if (effect_trigger == 4) {
            // 방어 스파크 이펙트 위치
            MOVE_CURSOR(9, 28); printf("[🛡️GUARD]");
        }
        COLOR_RESET();

        // 이펙트를 눈으로 확인할 수 있도록 콘솔 대기 시간을 강제 부여
        // (Windows 콘솔 시각 연출용 빈 입력 버퍼 대기 대용 처리)
        fflush(stdout);
        Sleep(1000);
    }

    // 10. 게임 최종 결과 스크린 출력
    CLEAR_SCREEN();
    printf("===============================================================\n");
    printf("                         전 투 종 료                           \n");
    printf("===============================================================\n\n");

    if (player_hp <= 0 && enemy_hp <= 0) {
        COLOR_YELLOW();
        printf(" [결과] 무승부! 두 파이터가 동시에 쓰러졌습니다.\n");
    }
    else if (enemy_hp <= 0) {
        COLOR_BLUE();
        printf(" [결과] ★ 플레이어 승리! ★\n 위대한 전사여, 적을 완벽하게 무찔렀습니다.\n");
    }
    else {
        COLOR_RED();
        printf(" [결과] 💀 적에게 패배했습니다. 💀\n 다시 수련하여 도전하십시오.\n");
    }
    COLOR_RESET();
    printf("\n===============================================================\n");

    return 0;
}