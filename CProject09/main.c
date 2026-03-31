/*
 * [교재 배경지식: 비트와 논리, 그리고 메모리]
 * 1. 비트 플래그 (Bit Flags): bool 변수 8개를 쓸 자리에 char 하나만 써서 8가지 상태를 관리함. 메모리와 성능 최적화의 핵심.
 * 2. 논리 연산자 (Logical Operators): &&(AND), ||(OR)는 '단락 평가(Short-circuit)'를 함. 앞 조건이 거짓이면 뒤는 아예 실행 안 함.
 * 3. Math 라이브러리 사용 예제
 * 4. sizeof 연산자: 함수가 아니라 '컴파일 타임'에 결정되는 연산자임. 메모리 공간의 크기를 바이트 단위로 알려줌.
 * 5. 2의 보수: 컴퓨터는 음수를 '비트 반전 + 1'로 저장함. 그래서 ~0이 -1이 되는 마법이 발생함.
 */

#include <stdio.h>
#include <math.h>   // pow(), sqrt() 등을 위해 포함

int main() {
    // --- 1. [비트 연산] 캐릭터 상태 관리 (State Flags) ---
    printf("----- 독 게임 -----\n");
    
    // 각 비트 자리에 의미를 부여함 (1, 2, 4, 8, 16...)
    unsigned char myState = 0; // 초기 상태: 깨끗함 (0000 0000)
    const unsigned char POISON = 1 << 0; // 1  (0001)
    const unsigned char STUN = 1 << 1; // 2  (0010)
    const unsigned char INVINCIBLE = 1 << 2; // 4  (0100)

    // 상태 부여: 독과 무적을 동시에 (OR 연산)
    myState = POISON | INVINCIBLE; // 0001 | 0100 = 0101 (5)

    // 상태 확인: 지금 마비(STUN) 걸렸나? (AND 연산)
    int isStunned = (myState & STUN) != 0; // 0101 & 0010 = 0 (거짓)

    // 상태 제거: 무적만 해제 (NOT 연산 후 AND)
    myState &= ~INVINCIBLE; // ~0100은 1011, 0101 & 1011 = 0001 (독만 남음)
    //myState = myState & (~INVINCIBLE);
    
    printf("현재 상태 코드: %d (1이면 독 상태 성공)\n", myState);



    // --- 2. [논리/비교] '고백으로 혼내주기' 스킬 발동 조건 ---
    printf("\n----- 고백하는 게임 -----\n");
    int myCharm = 10;          // 내 매력 수치
    int targetLovePoint = -50; // 상대방의 호감도 (이미 마이너스...)
    int hasGold = 1;       // 돈은 좀 있음
    int isDrunk = 0;      // 취했나?

    // 발동 조건: (매력이 80 이상이거나 돈이 많음) '이면서' 상대 호감도가 0 이상일 때
    // 하지만... 취했을 때는 앞뒤 안 가리고 발동됨 (|| 연산의 힘)
    int willSuccess = ((myCharm >= 80 || hasGold) && (targetLovePoint >= 0)) || isDrunk;

    if (willSuccess) {
        printf("결과: 고백 성공! (축하함)\n");
    }
    else {
        // 이 조건에 걸리면 '고백으로 혼내주기'가 발동됨
        printf("결과: 앗...아아..... 사랑해서 혼내줬다.....\n");
    }


    // --- 3. [Math 라이브러리] 넉백(Knockback) 거리 계산 ---
    // 피타고라스 정리를 이용한 두 점 사이의 거리 계산 (비트 시프트와 혼합)
    double enemyX = 10.0, enemyY = 20.0;
    double playerX = 13.0, playerY = 24.0;

    // 거리 구하기: sqrt(dx^2 + dy^2)
    double distance = sqrt(pow(playerX - enemyX, 2) + pow(playerY - enemyY, 2));

    // 비트 시프트를 이용한 대미지 증폭: 대미지 10을 왼쪽으로 2칸 밀기 
    int rawDamage = 10;
    int finalDamage = rawDamage << 2;

    printf("적과의 거리: %.2f, 증폭된 대미지: %d\n", distance, finalDamage);


    // --- 4. [sizeof] '내 인벤토리의 크기' 예제 ---
    // 자료형의 크기를 측정해보자
    int inventory1;
    float inventory2;
    char inventory3;

    printf("\n----- sizeof 통계 -----\n");
    printf("1. 인벤토리1 (정수) 크기: %zu 바이트\n", sizeof(inventory1));
    printf("2. 인벤토리2 (실수) 크기: %zu 바이트\n", sizeof(inventory2));
    printf("3. 인벤토리3 (문자) 크기: %zu 바이트\n", sizeof(inventory3));

    // 재밌는 사실: sizeof('A')는 C언어에서 1이 아니라 4(int)가 나올 때가 많음 (문자 상수는 int 취급)
    printf("4. 문자 'A'의 의외의 크기: %zu\n", sizeof('A'));
    


    // --- 5. 포션 개수를 관리하는 시스템
    signed char potionCount = 0;

    printf("\n----- 포션갯수 예제 ----- \n");
    printf("현재 포션 개수: %d\n", potionCount);

    // [사건 발생] 마법사가 '반전 저주'를 걸어서 포션 개수를 비트 반전 시킴!
    // ~0 연산이 수행됨
    potionCount = ~potionCount;

    printf("저주 걸린 포션 개수 (~0): %d\n", potionCount);
    // 결과: -1이 나옴. (00000000 -> 11111111 인데, signed라 -1로 해석)

    // [버그의 핵심] 만약 게임 서버가 이걸 '개수'니까 당연히 양수겠지? 하고 
    // unsigned(부호 없음)로 강제 변환해서 처리한다면?
    unsigned char buggedCount = (unsigned char)potionCount;

    printf("서버가 인식한 포션 개수: %u\n", buggedCount);
    // 결과: 255개! 0개였는데 저주 한번 맞고 풀템이 되어버림.

    printf("> 원리 설명\n");
    printf("* 1111 1111 비트 패턴은:\n");
    printf("  - %zd 타입(signed)으로 읽으면: %d\n", sizeof(char) , potionCount);
    printf("  - %zu 타입(unsigned)으로 읽으면: %d\n", sizeof(unsigned char) , buggedCount);

    return 0;
}