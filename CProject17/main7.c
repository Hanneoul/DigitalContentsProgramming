////고양이가 5열로 배치된 의자에 앉아있다.
////홀수 행에는 고양이가 없다.
////각 줄마다 고양이는 아래조건과같이 등장한다.
////i번째 줄에서 고양이는 j == i 위치에만 있다.
//
//#include <stdio.h>
//
//int main()
//{
//    int score = 0;
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//            if (j % 2 == 0)
//                continue;
//
//            if (j == i)
//            {
//                score += 1;
//                break;
//            }
//        }
//    }
//    printf("최종 고양이 갯수: %d\n", score);
//        return 0;
//}
//
//
