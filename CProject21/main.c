
/*
 * =========================================================================
 * [강의 자료: C언어 메모리 관리의 핵심 - 주소, 포인터, 그리고 void]
 * =========================================================================
 * 1. 주소(Address)란?
 *    - 메모리(RAM)의 각 1바이트 칸마다 붙여진 고유 번호.
 *    - '&' 연산자를 통해 변수가 위치한 시작 방 번호를 얻을 수 있음.
 *
 * 2. 포인터(Pointer)와 타입(Type):
 *    - 포인터는 '주소'를 담는 변수임. 모든 포인터는 주소값 크기(8바이트)로 동일함.
 *    - 주소만 있을 때 사이즈를 가늠하는 방법: 포인터의 '타입'이 기준임.
 *    - 예: int*는 "이 주소부터 4바이트 읽어라", char*는 "1바이트 읽어라"는 뜻.
 *
 * 3. void와 void* (만능 주소):
 *    - void: "정보가 없음"을 의미함.
 *    - void*: "타입 정보가 없는 순수한 주소". 무엇이든 담을 수 있지만,
 *      사이즈(길이) 정보가 없어서 그 주소의 실제 값에는 접근 불가함.
 *
 * 4. 캐스팅(Casting, 형변환):
 *    - 강제로 타입을 바꾸는 작업. (타입*)변수 형식으로 사용.
 *    - void* 주소를 다시 읽으려면 반드시 원래의 타입으로 '캐스팅' 해야 함.
 *
 * 5. printf %p와 (void*)의 관계:
 *    - %p는 주소값을 16진수로 출력하는 전용 서식 지정자임.
 *    - C 표준은 %p가 인자를 받을 때 (void*) 타입을 기대함.
 *    - 따라서 어떤 타입의 주소든 (void*)로 캐스팅해서 넘겨주는 것이 정석임.
 * =========================================================================
 */

#include <stdio.h>

int main()
{
    // [1] 데이터 선언 (메모리 공간 확보)
    int   nData = 0x12345678; // 4바이트 정수 
    float fData = 3.14f;      // 4바이트 실수
    char  cData = 'A';        // 1바이트 문자

    // [2] 포인터 변수 (주소 저장)
    // 포인터의 타입은 "도착지에서 몇 칸(size)을 읽을 것인가"를 결정함.
    int* pInt = &nData;
    float* pFloat = &fData;
    char* pChar = &cData;

    // [3] 주소값 출력 (%p와 void* 캐스팅)
    // (void*) 캐스팅을 통해 "순수한 주소 정보"로 전달함을 명시함.
    printf("[ADDRESS MAP]\n");
    printf("- nData Address : %p (Type: int*, Size: %d)\n", (void*)pInt, sizeof(*pInt));
    printf("- fData Address : %p (Type: float*, Size: %d)\n", (void*)pFloat, sizeof(*pFloat));
    printf("- cData Address : %p (Type: char*, Size: %d)\n", (void*)pChar, sizeof(*pChar));
    printf("--------------------------------------------------\n\n");

    // [4] void* (만능 주소) 활용과 캐스팅(Casting)
    void* pAny = NULL; // 아직 어디를 가리킬지 모르는 무색무취의 주소

    pAny = &nData; // 정수 주소를 담음 (가능)
    // printf("%d", *pAny); // [ERROR] void*는 사이즈 정보가 없어 역참조 불가능!

    // 해석: "pAny 주소를 int*로 취급해서(캐스팅), 그 방의 내용을 읽어라(*)"
    printf("[VOID POINTER & CASTING]\n");
    printf("- Value from void* (as int)   : 0x%X\n", *(int*)pAny);
    
    pAny = &cData; // 문자 주소를 담음 (가능)
    printf("- Value from void* (as char)  : %c\n", *(char*)pAny);
    printf("--------------------------------------------------\n\n");

    // [5] 주소만 있을 때 사이즈 가늠 시뮬레이션
    // 똑같은 주소라도 타입을 어떻게 캐스팅하느냐에 따라 읽어오는 양이 달라짐.
    pAny = &nData;
    printf("[TYPE INTERPRETATION]\n");
    printf("- Read 4 bytes (int*)  : 0x%X\n", *(int*)pAny);
    printf("- Read 1 byte  (char*) : 0x%X\n", *(char*)pAny); // 하위 1바이트만 읽음

    return 0;
}