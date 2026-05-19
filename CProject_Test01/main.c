#include <stdio.h>

#define SCR_W 40
#define SCR_H 20
#define WORLD_W 200

// [함수] 화면 좌표계에 24비트 트루 컬러로 점 찍기
void draw_pixel(int x, int y, int r, int g, int b) {
    if (x < 0 || x >= SCR_W || y < 0 || y >= SCR_H) return;
    // 커서 이동 후 배경색 지정 출력[cite: 1]
    printf("\033[%d;%dH\033[48;2;%d;%d;%dm  \033[0m", y + 1, (x * 2) + 1, r, g, b);
}

int main() 
{
    int playerX = 10;
    int cameraX = 0;
    char cmd;

    // 초기 세팅: 화면 지우기, 커서 숨기기[cite: 1]
    printf("\033[2J\033[?25l");

    while (1) 
    {
        // 1. 카메라 위치 계산 (플레이어를 화면 중앙에 유지)
        cameraX = playerX - (SCR_W / 2);
        if (cameraX < 0) cameraX = 0;
        if (cameraX > WORLD_W - SCR_W) cameraX = WORLD_W - SCR_W;

        // 2. 렌더링 (그리기)
        // 화면 전체를 짙은 파란색으로 일단 채움 (하늘)
        for (int y = 0; y < SCR_H; y++) 
        {
            for (int x = 0; x < SCR_W; x++) 
            {
                draw_pixel(x, y, 10, 10, 40);
            }
        }

        // 월드 지형물 그리기 (10칸 간격으로 검정색 기둥 배치)
        // 3비트 0번 검정이 불안정할 수 있으니 24비트 (0,0,0)으로 확실하게 그림
        for (int wx = 0; wx < WORLD_W; wx += 10) 
        {
            int sx = wx - cameraX; // 월드 좌표를 화면 좌표로 변환[cite: 1]
            if (sx >= 0 && sx < SCR_W) {
                for (int sy = 5; sy < SCR_H-5; sy++) 
                {
                    draw_pixel(sx, sy, 80, 80, 80); // "진짜" 검정색 픽셀
                }
            }
        }

        // 플레이어 캐릭터 (노란색)
        draw_pixel(playerX - cameraX, SCR_H / 2, 255, 255, 0);

        // UI 출력
        printf("\033[%d;1H", SCR_H + 2);
        printf(" [World X: %3d] (a:좌, d:우, q:종료) ", playerX);

        // 3. 입력 및 로직
        cmd = getch();
        if (cmd == 'a' && playerX > 0) playerX--;
        if (cmd == 'd' && playerX < WORLD_W - 1) playerX++;
        if (cmd == 'q') break;

        // 화면 갱신을 위해 지우기 (성능을 위해 위치를 직접 덮어쓰는 게 좋으나 여기선 간단히)
        printf("\033[2J");
    }

    printf("\033[?25h\033[2J");
    return 0;
}