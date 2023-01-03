#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

#define ROW_SIZE 55     //미로 크기 홀수로만 설정
#define COL_SIZE 55
#define UP    1         //우선법 쉬프트 연산자를 이용한 상하좌우
#define RIGHT 2
#define DOWN  4
#define LEFT  8

#define BACK  4         //DFS 되돌아가는 변수

typedef struct {        //행과 열을 저장하는 구조체
    int row;
    int col;
} Coord;

clock_t start, end;     //시간 측정 변수 clock





void gotoxy(int x, int rows);                                                                       //gotoxy
void CursorView();                                                                                  //커서숨기기
void setColor(int color, int bgcolor);                                                              //색상 설정
void resetColor();                                                                                  //색상 초기화
void PrintMaze(int** maze, int rows, int cols);                                                     //맵 출력 (시각화)
int** CopyMaze(int** maze, int rows, int cols);                                                     //맵 복사

//Maze_Generation
//Aldous_Broder Algorithm
int** Alodus_Reset(int rows, int cols);                                                             //Aldous_Broder 맵 초기화
void Aldous_Broder(int** maze, int rows, int cols);                                                 //Aldous_Broder main

//Prim's Algorithm
int** Prim_Reset(int rows, int cols);                                                               //Prim 맵 초기화
int Prim_Contains(Coord* array, int length, int row, int col);                                      //배열에 해당 좌표가 저장되어있는지 확인하는 함수
int Prim_AddWalls(int** maze, int rows, int cols, Coord* array, int length, Coord aCoord);          //근처의 벽을 배열에 추가하는 함수
void Prim(int** maze, int rows, int cols);                                                          //Prim main

//Maze_Solve
//A* Algorithm
double Astar(int** maze, int** printmaze);                                                          //Astar main

//우선법
int RightHand_WallAhead(int** maze, int row_index, int col_index, int dir);                         //앞에 벽이 있는지 비교하는 함수
double RightHand(int** maze, int row_index, int col_index, int dir);                                //우선법 main
void RightHand_Visualize(int** maze, int row_index, int col_index, int dir);                        //우선법 시각화

//DFS Algorithm
void DFS_push(Coord* mStack, int* top, int row, int col);                                           //push
Coord DFS_pop(Coord* mStack, int* top);                                                             //pop
double DFS(int** maze);                                                                             //DFS main
int DFS_CurrentPosition(int** maze, int* row, int* col);                                            //미로안에 있는지 비교하는 함수
void DFS_Visualize(int** maze);                                                                     //DFS 시각화




//main
int main(void) {
    int** maze = 0, ** maze_AStar = 0, ** maze_RightHand = 0, ** maze_DFS = 0, ** maze_DFS2 = 0;    //생성된 미로를 복사하는 map
    int num = 0;                                                                                    //생성 알고리즘을 선택하는 변수
    int row_index = 1, col_index = 1, i = 0;
    double pst = 0;                                                                                 //알고리즘 실행시간 측정변수

    srand((unsigned)time(NULL));                                                                    //랜덤 Seed 값을 초기화
    system("mode con cols=130 lines=70");                                                           //콘솔 사이즈 정의
    SetConsoleTitle(TEXT("MAZE SOLVE"));                                                            //콘솔 제목 정의

    //사용자 인터페이스(1. 색상추가 버전)
    USER_INTERFACE:
    gotoxy(10, 18);
    setColor(0, 8);
    printf("                                                                                                              \n");
    gotoxy(10, 33);
    printf("                                                                                                              \n");
    for (i = 18; i < 34; i++) {
        gotoxy(10, i);
        printf("  ");
    };
    for (i = 18; i < 34; i++) {
        gotoxy(118, i);
        printf("  ");
    };

    resetColor();
    gotoxy(1, 18);
    setColor(11, 0);
    printf("\n\n\n\n\n");
    printf("                ■      ■  ■■■■  ■■■■  ■■■■        ■■■■  ■■■■  ■     ■      ■  ■■■■           \n");
    printf("                ■■  ■■  ■    ■       ■   ■              ■        ■    ■  ■      ■    ■   ■                 \n");
    printf("                ■  ■  ■  ■■■■     ■     ■■■■        ■■■■  ■    ■  ■       ■  ■    ■■■■           \n");
    printf("                ■      ■  ■    ■   ■       ■                    ■  ■    ■  ■        ■■     ■                 \n");
    printf("                ■      ■  ■    ■  ■■■■  ■■■■        ■■■■  ■■■■  ■■■■   ■      ■■■■           \n");
    printf("\n\n\n\n\n\n\n\n\n\n");
    resetColor();
    printf("                                     ┌────────────  < 미로 생성을 시작합니다 > ────────────┐\n");
    printf("                                     │                                                     │\n");
    printf("                                     │               알고리즘을 선택하십시오.              │\n");
    printf("                                     │                                                     │\n");
    printf("                                     │               ▶ Aldous_Broder Algorithm = 1        │\n");
    printf("                                     │               ▶ Prim's Algorithm = 2               │\n");
    printf("                                     │                                                     │\n");
    printf("                                     │                                                     │\n");
    printf("                                     │               번호를 입력하십시오 :                 │\n");
    printf("                                     └─────────────────────────────────────────────────────┘\n");

    gotoxy(10, 18);
    setColor(0, 8);
    printf("                                                                                                              \n");
    gotoxy(10, 33);
    printf("                                                                                                              \n");
    for (i = 18; i < 34; i++) {
        gotoxy(10, i);
        printf("  ");
    };
    for (i = 18; i < 34; i++) {
        gotoxy(118, i);
        printf("  ");
    };
    resetColor();

    //사용자 인터페이스(2. 색상 미추가 버전)
    //gotoxy(1, 17);
    //printf("          ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■   \n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■     ■      ■  ■■■■  ■■■■  ■■■■        ■■■■  ■■■■  ■     ■      ■  ■■■■      ■   \n");
    //printf("          ■     ■■  ■■  ■    ■       ■   ■              ■        ■    ■  ■      ■    ■   ■            ■   \n");
    //printf("          ■     ■  ■  ■  ■■■■     ■     ■■■■        ■■■■  ■    ■  ■       ■  ■    ■■■■      ■   \n");
    //printf("          ■     ■      ■  ■    ■   ■       ■                    ■  ■    ■  ■        ■■     ■            ■   \n");
    //printf("          ■     ■      ■  ■    ■  ■■■■  ■■■■        ■■■■  ■■■■  ■■■■   ■      ■■■■      ■   \n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■                                                                                                          ■\n");
    //printf("          ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■   \n");
    //printf("\n\n\n\n\n");
    //printf("                                     ┌────────────  < 미로 생성을 시작합니다 > ────────────┐\n");
    //printf("                                     │                                                     │\n");
    //printf("                                     │               알고리즘을 선택하십시오.              │\n");
    //printf("                                     │                                                     │\n");
    //printf("                                     │               ▶ Aldous_Broder Algorithm = 1        │\n");
    //printf("                                     │               ▶ Prim's Algorithm = 2               │\n");
    //printf("                                     │                                                     │\n");
    //printf("                                     │                                                     │\n");
    //printf("                                     │               번호를 입력하십시오 :                 │\n");
    //printf("                                     └─────────────────────────────────────────────────────┘\n");

    gotoxy(76, 46);
    scanf_s("%d", &num);

    //사용자가 잘못 입력한 경우
    if (num != 1 && num != 2)
        goto USER_INTERFACE;

    system("cls");
    switch (num) {
    case 1:
        maze = Alodus_Reset(ROW_SIZE, COL_SIZE);
        Aldous_Broder(maze, ROW_SIZE, COL_SIZE);
        PrintMaze(maze, ROW_SIZE, COL_SIZE);
        break;
    case 2:
        maze = Prim_Reset(ROW_SIZE, COL_SIZE);
        Prim(maze, ROW_SIZE, COL_SIZE);
        PrintMaze(maze, ROW_SIZE, COL_SIZE);
        break;
    }

    //A* Algorithm
    maze_AStar = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    printf("A* 알고리즘을 이용한 미로해결을 시작합니다\n아무키나 입력하십시오\n");
    _getch();

    system("cls");
    pst = Astar(maze_AStar, maze);
    printf("\n\n");
    printf("해결 time : %f\n\n", pst);


    //우선법
    maze_RightHand = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    printf("우선법을 이용한 미로해결을 시작합니다\n아무키나 입력하십시오\n");
    _getch();

    system("cls");
    pst = RightHand(maze_RightHand, row_index, col_index, RIGHT);
    PrintMaze(maze_RightHand, ROW_SIZE, COL_SIZE);
    RightHand_Visualize(maze_RightHand, row_index, col_index, RIGHT);
    printf("\n\n");
    printf("해결 time : %f\n\n", pst);


    //DFS Algorithm
    maze_DFS = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    maze_DFS2 = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    printf("DFS를 이용한 미로해결을 시작합니다\n아무키나 입력하십시오\n");
    _getch();

    system("cls");
    pst = DFS(maze_DFS);
    DFS_Visualize(maze_DFS2);
    printf("\n\n");
    printf("해결 time : %f\n\n", pst);
    printf("모든 미로해결을 종료합니다\n아무키나 입력하십시오\n");
    _getch();

    return 0;
}





//gotoxy
void gotoxy(int x, int y) {
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//커서숨기기
void CursorView()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//색상 설정
void setColor(int color, int bgcolor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | (color & 0xf));
}

//색상 초기화
void resetColor() {
    setColor(15, 0);
}

//맵 출력 (시각화)
void PrintMaze(int** maze, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        resetColor();
        for (j = 0; j < cols; j++) {
            switch (maze[i][j]) {
            case 0:
                setColor(0, 0);
                printf("  ");
                break;
            case 1:
                setColor(0, 8);
                printf("  ");
                break;
                /*case 2:
                    setColor(0, 14);
                    printf("  ");
                    break;*/
            case 3:
                //case BACK:
                setColor(12, 14);
                printf("★");
                break;
            }
        }
        printf("\n");
    }
    resetColor();
}

//맵 복사
int** CopyMaze(int** maze, int rows, int cols) {
    int i, j;

    int** maze_temp = (int**)(malloc(sizeof(int*) * rows));

    for (i = 0; i < rows; i++) {
        maze_temp[i] = (int*)(malloc(sizeof(int) * cols));
        for (j = 0; j < cols; j++) {
            maze_temp[i][j] = maze[i][j];
        }
    }
    return maze_temp;
}





//Aldous_Broder Algorithm
//Aldous_Broder 맵 초기화
int** Alodus_Reset(int rows, int cols) {
    int i, j;

    int** maze = (int**)(malloc(sizeof(int*) * rows));          //미로를 맵의 크기만큼 동적할당

    for (i = 0; i < rows; i++) {
        maze[i] = (int*)(malloc(sizeof(int) * cols));
        for (j = 0; j < cols; j++) {
            maze[i][j] = 1;                                     //벽으로 모든 배열요소 초기화
        }
    }

    return maze;
}

//ALdous_Broder main
void Aldous_Broder(int** maze, int rows, int cols) {
    int visited = 0;                                            //모든 노드를 다 방문했는지 확인하는 변수
    int rotate;                                                 //길을 내는 방향을 뜻하는 변수
    int row_index = 1, col_index = 1;                           //초기의 시작지점을 (1,1)로 설정
    int i, j;

    for (i = 0; i < rows / 2; i++) {
        for (j = 0; j < cols / 2; j++) {
            visited++;                                          //노드의 수만큼 visited 변수 증가
        }
    }

    PrintMaze(maze, rows, cols);
    CursorView();                                               //커서를 제거

    while (visited > 0) {
        rotate = rand() % 4 + 1;                                //길을 내는 방향을 랜덤으로 설정
        if (rotate == 1) {                  //상
            if (row_index == 1)
                continue;                                       //위로 길을 내려하는데 만약 윗부분이 외벽에 해당한다면 continue
            else if (maze[row_index - 2][col_index] == 1) {     //위의 노드가 아직 벽이라면
                row_index--; maze[row_index][col_index] = 0;    //두 노드 사이의 벽을 길로 만들고 그 자리로 이동
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");                                   //미로의 생성을 시각화(gotoxy)
                row_index--; maze[row_index][col_index] = 0;    //위의 노드도 길로 만들고 그 자리로 이동
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");                                   //미로의 생성을 시각화(gotoxy)
                visited--;                                      //노드를 방문했으므로 visited 변수값을 1만큼 줄임
            }
            else {                                              //위의 노드가 길이라면
                row_index -= 2;                                 //위의 노드로 이동만 함
            }
        }
        if (rotate == 2) {                  //우
            if (col_index == cols - 2)
                continue;
            else if (maze[row_index][col_index + 2] == 1) {
                col_index++; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                col_index++; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                visited--;
            }
            else {
                col_index += 2;
            }
        }
        if (rotate == 3) {                  //하
            if (row_index == rows - 2)
                continue;
            else if (maze[row_index + 2][col_index] == 1) {
                row_index++; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                row_index++; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                visited--;
            }
            else {
                row_index += 2;
            }
        }
        if (rotate == 4) {                  //좌
            if (col_index == 1)
                continue;
            else if (maze[row_index][col_index - 2] == 1) {
                col_index--; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                col_index--; maze[row_index][col_index] = 0;
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");
                visited--;
            }
            else {
                col_index -= 2;
            }
        }
        Sleep(1);                                               //미로의 생성을 시각화(system("cls"))
        /*PrintMaze(maze, rows, cols); */
        /*system("cls");*/
    }
    system("cls");
}





//Prim's Algorithm
//Prim 맵 초기화
int** Prim_Reset(int rows, int cols) {
    int i, j;
    int** maze = (int**)(malloc(sizeof(int*) * rows));      //미로를 맵의 크기만큼 동적할당

    for (i = 0; i < rows; i++) {
        maze[i] = (int*)(malloc(sizeof(int) * cols));

        for (j = 0; j < cols; j++) {
            if (j % 2 != 0 && i % 2 != 0)                   //미로를 그리드 형태로 초기화
                maze[i][j] = 0;                             //노드는 0
            else
                maze[i][j] = 1;                             //벽은 1
        }
    }

    return maze;
}

//배열에 해당 좌표가 저장되어있는지 확인하는 함수
int Prim_Contains(Coord* array, int length, int row, int col) {
    int i = 0;
    Coord temp;                                             //임시 Coord 구조체
    temp.row = row;                                         //매개변수로 넘어온 좌표값을 temp에 저장
    temp.col = col;

    for (i = 0; i < length; i++) {
        if (array[i].row == temp.row && array[i].col == temp.col)
            return 1;                                       //구조체 배열 array에 해당 좌표가 저장되어 있으면 1을 반환
    }
    return 0;                                               //아닐경우 0을 반환
}

//근처의 벽을 배열에 추가하는 함수
int Prim_AddWalls(int** maze, int rows, int cols, Coord* array, int length, Coord aCoord) {
    int original_length = length;                           //넘어온 배열의 길이를 저장하는 변수
    Coord temp_wall;                                        //벽의 좌표를 임시로 저장하는 변수

    if (aCoord.row - 1 >= 0) {          //상 
        temp_wall.row = aCoord.row - 1;                     //한칸 위의 좌표를 임시로 저장
        temp_wall.col = aCoord.col;
        //한칸 위의 좌표가 벽이고 아직 벽 배열에 저장되지 않았다면
        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row - 1, aCoord.col)) {
            array[length] = temp_wall;                      //벽 배열에 해당 벽을 추가
            length++;                                       //벽 배열의 길이를 하나 늘림
        }
    }

    if (aCoord.row + 1 < rows) {        //하
        temp_wall.row = aCoord.row + 1;
        temp_wall.col = aCoord.col;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row + 1, aCoord.col)) {
            array[length] = temp_wall;
            length++;
        }
    }

    if (aCoord.col - 1 >= 0) {          //좌
        temp_wall.row = aCoord.row;
        temp_wall.col = aCoord.col - 1;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row, aCoord.col - 1)) {
            array[length] = temp_wall;
            length++;
        }
    }

    if (aCoord.col + 1 < cols) {        //우
        temp_wall.row = aCoord.row;
        temp_wall.col = aCoord.col + 1;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row, aCoord.col + 1)) {
            array[length] = temp_wall;
            length++;
        }
    }

    return length - original_length;    //몇 개의 벽이 배열에 추가되었는지 반환
}

//Prim main
void Prim(int** maze, int rows, int cols) {
    Coord* wallArr = (Coord*)malloc(sizeof(Coord) * rows * cols);           //벽 배열(구조체 배열로 벽의 좌표값을 저장)
    Coord* unvisitedArr = (Coord*)malloc(sizeof(Coord) * rows * cols);      //방문하지 않은 노드 배열
    Coord temp, firstnode, unvisited_node, wall;                            //임시 좌표구조체, 처음 방문할 노드구조체, 방문하지 않은 특정 노드구조체, 깎아낼 벽 구조체
    int wallArr_length = 0, unvisitedArr_length = 0;                        //벽 배열의 길이 변수, unvisitedArr 배열의 길이 변수
    int firstnode_index, wallIndex;                                         //처음 방문할 노드의 Index 변수, 깎아낼 벽의 Index 변수
    int i = 0, j = 0, k = 0;
    bool near_unvisted_node = false;                                        //방문하지 않은 노드 옆인지 판단하는 변수

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (maze[i][j] == 0) {                                          //노드가 0
                temp.row = i;                                               //임시 좌표구조체에 i와 j로 좌표값을 저장
                temp.col = j;
                unvisitedArr[unvisitedArr_length++] = temp;                 //방문하지 않은 노드 배열에 노드들을 추가시킴
            }
        }
    }

    PrintMaze(maze, rows, cols);                                            //초기화된 미로를 출력
    CursorView();                                                           //콘솔창의 커서를 제거

    firstnode_index = rand() % unvisitedArr_length;                         //처음으로 방문할 노드의 인덱스를 방문하지 않은 노드의 길이로 부터 랜덤으로 가져옴
    firstnode = unvisitedArr[firstnode_index];                              //처음으로 방문할 노드의 좌표값을 저장시킴

    //unvisitedArr 배열에서 방문할 노드의 좌표값을 제거하는 부분             
    for (i = firstnode_index + 1; i < unvisitedArr_length; i++) {           //i값을 방문할 노드의 인덱스로 설정하고 반복문을 돌림
        unvisitedArr[i - 1] = unvisitedArr[i];                              //방문할 노드의 좌표값 위치에 그 이후 좌표값을 덮어씌우고 값들은 한칸씩 앞 당김
    }
    unvisitedArr_length -= 1;                                               //unvisitedArr 배열의 길이를 하나 줄임          

    wallArr_length += Prim_AddWalls(maze, rows, cols, wallArr, wallArr_length, firstnode); /*처음으로 방문할 노드 근처의 벽들을 벽 배열에 추가시키고
                                                                                             추가된 벽 개수만큼 wallArr_length를 증가시켜줌*/

                                                                                             //벽 배열이 다 빌때까지 미로에서 길을 내는 부분
    while (wallArr_length > 0) {
        near_unvisted_node = false;                                         //방문하지 않은 노드 옆인지 판단하는 변수를 방문하지 않은 노드 옆이 아니라고 초기화
        wallIndex = rand() % wallArr_length;                                //깎아낼 벽의 인덱스를 벽 배열의 길이로 부터 랜덤으로 가져옴
        wall = wallArr[wallIndex];                                          //깎아낼 벽의 좌표값을 저장시킴

        //벽 배열에서 깎아낼 벽의 좌표값을 제거하는 부분                        
        for (k = wallIndex + 1; k < wallArr_length; k++) {                  //k값을 깎아낼 벽의 인덱스로 설정하고 반복문을 돌림
            wallArr[k - 1] = wallArr[k];                                    //깍아낼 벽의 좌표 위치에 그 이후 좌표값을 덮어씌우고 값들을 한칸씩 앞 당김
        }
        wallArr_length--;                                                   //벽 배열의 길이를 하나 줄임

        //wall에 저장된 벽의 상하좌우를 확인하여 벽과 연결된 방문하지 않은 노드를 찾아냄
        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row - 1, wall.col)) {     //상
            unvisited_node.row = wall.row - 1;                              //방문하지 않은 노드의 좌표값을 저장
            unvisited_node.col = wall.col;
            near_unvisted_node = true;                                      //방문하지 않은 노드 근처가 맞다고 true 값을 저장
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row + 1, wall.col)) {     //하
            unvisited_node.row = wall.row + 1;
            unvisited_node.col = wall.col;
            near_unvisted_node = true;
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row, wall.col - 1)) {     //좌
            unvisited_node.row = wall.row;
            unvisited_node.col = wall.col - 1;
            near_unvisted_node = true;
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row, wall.col + 1)) {     //우
            unvisited_node.row = wall.row;
            unvisited_node.col = wall.col + 1;
            near_unvisted_node = true;
        }

        if (near_unvisted_node) {                                                       //방문하지 않은 노드 근처일때 실행
            maze[wall.row][wall.col] = 0;                                               //깎아낼 벽을 깎아서 길을 뜻하는 0으로 설정
            gotoxy(wall.col * 2 + 1, wall.row + 1);
            setColor(0, 0);
            printf("  ");                                                               //미로의 생성을 시각화(gotoxy)

            for (i = 0; i < unvisitedArr_length; i++) {                                 //unvisitedArr 배열에서 해당 노드의 좌표를 제거함으로써 해당 노드를 방문시키는 부분
                if (unvisitedArr[i].row == unvisited_node.row && unvisitedArr[i].col == unvisited_node.col) //unvisited 배열에서 해당 노드와 좌표가 일치하는 노드를 찾아
                    for (j = i + 1; j < unvisitedArr_length; j++) {
                        unvisitedArr[j - 1] = unvisitedArr[j];                          //해당 노드의 좌표값 위치에 그 이후 좌표값을 덮어씌우고 값들은 한칸씩 앞 당김
                    }
            }
            unvisitedArr_length -= 1;                                                   //unvisitedArr 배열의 길이를 하나 줄임         

            wallArr_length += Prim_AddWalls(maze, rows, cols, wallArr, wallArr_length, unvisited_node); /*방문한 노드 근처의 벽들을 벽 배열에 추가시키고
                                                                                                          추가된 벽 개수만큼 wallArr_length를 증가시켜줌*/
        }
        //PrintMaze(maze, rows, cols);                                                  //미로의 생성을 시각화(system("cls"))
        Sleep(1);
        //system("cls");
    }
    system("cls");
}





//A* Algorithm
double Astar(int** maze, int** printmaze) {
    typedef struct node {
        int g, h, f;                                            //g는 start부터 이동하는 거리, h는 finish까지에 거리
        int open;                                               //열린 노드인지 확인하는 변수
        int col, row;                                           //행과 열
        int mom_node_col, mom_node_row;                         //부모노드에 행열
        int past_node;                                          //지난벽인지 확인(닫힌 노드) 0이면 안지난 벽
    } open_node;

    start = clock();
    double pst;                                                 //해결 시간 저장 변수
    int i, j, k, l;                                             //반복제어
    int r_len = 0;                                              //해결루트에 대한 배열 길이 변수
    Coord* Coords = NULL;                                       //행과 열을 저장하는 구조체
    open_node** block;                                          //맵의 크기만큼 각 노드 구조체 선언
    block = (open_node**)calloc(ROW_SIZE, sizeof(open_node*));
    for (i = 0; i < ROW_SIZE; i++) {
        block[i] = (open_node*)calloc(COL_SIZE, sizeof(open_node));
    }
    int tmp_g = -1, tmp_f = 100000000;                          //노드 비교 부분을 위한 변수
    block[1][1].past_node = 1;                                  //시작 지점 (1,1)에 대한 값 초기화
    block[1][1].g = 0;
    block[1][1].h = (ROW_SIZE - 2 - 2) * 10 + (COL_SIZE - 2 - 2) * 10;
    block[1][1].f = block[1][1].h + block[1][1].g;
    block[1][1].row = 1;
    block[1][1].col = 1;
    i = 1, j = 1;

    //열린 노드 생성 부분
    while (1) {
        if (block[i][j].open == 1 || (i == 1 && j == 1)) {                                                  //시작지점이거나 열린노드 
            if (maze[i - 1][j] != 1 && block[i - 1][j].open == 0 && block[i - 1][j].past_node == 0) {       //위가 벽이 아니고 열린노드가 아닌데 지난 경로가 아니면
                block[i - 1][j].open = 1;                                                                   //위 row-1
                block[i - 1][j].row = i - 1;                                                                //윗 블록에 행과 열
                block[i - 1][j].col = j;
                block[i - 1][j].mom_node_row = i;                                                           //윗 블록에 이전 노드는 현재 노드
                block[i - 1][j].mom_node_col = j;
                block[i - 1][j].h = (ROW_SIZE - 2 - (i - 1)) * 10 + (COL_SIZE - 2 - j) * 10;                //도착지점까지에 가로 세로 거리 
                block[i - 1][j].g = block[i][j].g + 10;                                                     //이전 노드에 g + 10 값
                block[i - 1][j].f = block[i - 1][j].h + block[i - 1][j].g;
            }
            if (maze[i + 1][j] != 1 && block[i + 1][j].open == 0 && block[i + 1][j].past_node == 0) {       //아래
                block[i + 1][j].open = 1;                                                                   //아래 row+1
                block[i + 1][j].row = i + 1;
                block[i + 1][j].col = j;
                block[i + 1][j].mom_node_row = i;
                block[i + 1][j].mom_node_col = j;
                block[i + 1][j].h = (ROW_SIZE - 2 - (i + 1)) * 10 + (COL_SIZE - 2 - j) * 10;
                block[i + 1][j].g = block[i][j].g + 10;
                block[i + 1][j].f = block[i + 1][j].h + block[i + 1][j].g;
            }
            if (maze[i][j + 1] != 1 && block[i][j + 1].open == 0 && block[i][j + 1].past_node == 0) {       //오른쪽
                block[i][j + 1].open = 1;                                                                   //오른쪽 col+1
                block[i][j + 1].row = i;
                block[i][j + 1].col = j + 1;
                block[i][j + 1].mom_node_row = i;
                block[i][j + 1].mom_node_col = j;
                block[i][j + 1].h = (ROW_SIZE - 2 - i) * 10 + (COL_SIZE - 2 - (j + 1)) * 10;
                block[i][j + 1].g = block[i][j].g + 10;
                block[i][j + 1].f = block[i][j + 1].h + block[i][j + 1].g;
            }
            if (maze[i][j - 1] != 1 && block[i][j - 1].open == 0 && block[i][j - 1].past_node == 0) {       //왼쪽
                block[i][j - 1].open = 1;                                                                   //왼쪽 col-1
                block[i][j - 1].row = i;
                block[i][j - 1].col = j - 1;
                block[i][j - 1].mom_node_row = i;
                block[i][j - 1].mom_node_col = j;
                block[i][j - 1].h = (ROW_SIZE - 2 - i) * 10 + (COL_SIZE - 2 - (j - 1)) * 10;
                block[i][j - 1].g = block[i][j].g + 10;
                block[i][j - 1].f = block[i][j - 1].h + block[i][j - 1].g;
            }
        }

        //열린노드 비교 부분
        for (k = 1; k < ROW_SIZE - 1; k++) {                                        //미로 전체를 확인함
            for (l = 1; l < COL_SIZE - 1; l++) {
                if (block[k][l].open == 1 && block[k][l].past_node == 0) {          //열린노드면서 지난 노드가 아닌 경우
                    if (tmp_f > block[k][l].f) {                                    //저장된 임시 f값보다 작은 f값이 있으면
                        tmp_f = block[k][l].f;                                      //현재 노드에 f,g값으로 tmp값을 변경
                        tmp_g = block[k][l].g;
                        i = k;                                                      //선택된 열린 노드를 현재 노드에 좌표로 설정
                        j = l;
                    }
                    else if (tmp_f == block[k][l].f && tmp_g < block[k][l].g) {     /*저장된 임시 f값과 반복문이 가르키는 노드 f값이 같으면
                                                                                      임시 g값과 block에 g값을 비교, g값이 더 높으면 작동함*/

                        tmp_g = block[k][l].g;                                      //만약 f,g값 모두 같으면 우측(열) 우선으로 이동
                        i = k;
                        j = l;
                    }
                }
            }
        }
        block[i][j].past_node = 1;                                                  //현재 노드로 선택된 것을 지난 노드로 설정
        tmp_g = 0;                                                                  //tmp_g 초기화
        tmp_f = 10000000;                                                           //tmp_f 초기화
        if (i == ROW_SIZE - 2 && j == COL_SIZE - 2) {                               //결과를 찾은 경우
            break;
        }
    }

    //사용자에게 해결 과정을 보여주는 부분
    k = 0;
    while (1) {                                                                     //i는 행,j는 열이 1,1 시작점으로 갈때까지 반복
        r_len++;
        Coords = (Coord*)realloc(Coords, r_len * sizeof(Coord));                    //해결 루트에 대한 구조체배열
        if (k == 0) {
            maze[i][j] = 2;                                                         //k=0일때 i , j는 골인 지점
            printf("과정 [%d] (%d,%d)\n", k, block[i][j].row, block[i][j].col);
        }
        Coords[k].row = block[i][j].mom_node_row;                                   //루트에 row는 부모노드에 행
        Coords[k].col = block[i][j].mom_node_col;                                   //루트에 col는 부모노드에 열
        i = Coords[k].row;                                                          //부모노드에 행 저장
        j = Coords[k].col;                                                          //부모노드에 열 저장
        k++;                                                                        //다음 루트좌표
        maze[i][j] = 2;                                                             //부모노드로 저장된 이전 노드를 최적 경로로 설정
        printf("과정 [%d] (%d,%d)\n", k, block[i][j].row, block[i][j].col);

        if (i == 1 && j == 1) {                                                     //시작 지점까지 돌아오면
            Coords[k].row = 1;
            Coords[k].col = 1;
            break;
        }
    }
    end = clock();

    _getch();
    system("cls");
    PrintMaze(printmaze, ROW_SIZE, COL_SIZE);                       //해결과정이 없는 빈 미로 생성
    for (l = r_len; l > 0; l--) {                                   //최적 경로 시각화
        Sleep(50);
        gotoxy(Coords[l].col * 2 + 1, Coords[l].row + 1);
        setColor(0, 14);
        printf("  ");
        gotoxy(Coords[l - 1].col * 2 + 1, Coords[l - 1].row + 1);
        setColor(12, 14);
        printf("★");
    }
    gotoxy(Coords[l].col * 2 + 1, Coords[l].row + 1);
    setColor(0, 14);
    printf("  ");
    gotoxy(COL_SIZE * 2 - 3, ROW_SIZE - 1);
    setColor(12, 14);
    printf("★");
    resetColor();

    for (i = 0; i < ROW_SIZE; i++) {                                //동적할당 해제
        free(block[i]);
    }
    free(block);
    pst = (double)(end - start) / CLK_TCK;
    return pst;                                                     //측정시간 반환
}





//우선법
//앞에 벽이 있는지 비교하는 함수
int RightHand_WallAhead(int** maze, int row_index, int col_index, int dir) {   //우선법 방향(dir) 앞에 벽이 있는지 확인
    row_index = (dir == LEFT) ? --row_index :                                  //들어온 방향이 LEFT면 --row, RIGHT면 ++row
        (dir == RIGHT) ? ++row_index : row_index;
    col_index = (dir == UP) ? --col_index :                                    //들어온 방향이 UP이면 --col, DOWN이면 ++col
        (dir == DOWN) ? ++col_index : col_index;
    return maze[col_index][row_index];                                         //증감 연산자에 대한 위치에 대하여 값 반환  벽 = 1, 길 = 0
}

//우선법 main
double RightHand(int** maze, int row_index, int col_index, int dir) {
    double pst;                                                                //시간 측정 변수

    start = clock();
    while (row_index > 0 && row_index < COL_SIZE - 1 && col_index>0 && col_index < ROW_SIZE - 1 && (row_index != COL_SIZE - 2 || col_index != ROW_SIZE - 2)) {
        dir <<= 1;                                                             //쉬프트 연산자 bit값을 오른쪽으로 한칸 움직입니다.
        dir = (dir > LEFT) ? UP : dir;                                         //몸을 오른쪽으로 돌린다. UP->RIGHT->DOWN->LEFT
        while (RightHand_WallAhead(maze, row_index, col_index, dir)) {         //dir 방향 앞이 벽인지 확인해서 길(0)이면 통과 벽(1)이면 반복문 시작
            dir >>= 1;                                                         //왼쪽으로 bit 쉬프트
            dir = (dir == 0) ? LEFT : dir;
        }
        //벽이라면 벽이 아닐때까지 몸을 왼쪽으로 돌림. LEFT->DOWN->RIGHT->UP
        row_index = (dir == LEFT) ? --(row_index) : (dir == RIGHT) ? ++(row_index) : row_index;   //현재 dir 값으로 row,col값 이동합니다.
        col_index = (dir == UP) ? --(col_index) : (dir == DOWN) ? ++(col_index) : col_index;
    }
    end = clock();

    pst = (double)(end - start) / CLK_TCK;
    return pst;
}

//우선법 시각화
void RightHand_Visualize(int** maze, int row_index, int col_index, int dir) {  //우선법 함수에 시각화 부분을 추가한 함수
    gotoxy(row_index + 2, col_index + 1);
    setColor(0, 14);
    printf("  ");

    //위의 우선법 main과 동일
    while (row_index > 0 && row_index < COL_SIZE - 1 && col_index>0 && col_index < ROW_SIZE - 1 && (row_index != COL_SIZE - 2 || col_index != ROW_SIZE - 2)) {
        Sleep(30);
        dir <<= 1;
        dir = (dir > LEFT) ? UP : dir;
        while (RightHand_WallAhead(maze, row_index, col_index, dir)) {
            dir >>= 1;
            dir = (dir == 0) ? LEFT : dir;
        }
        gotoxy(row_index * 2 + 1, col_index + 1);
        setColor(0, 14);
        printf("  ");

        row_index = (dir == LEFT) ? --(row_index) :
            (dir == RIGHT) ? ++(row_index) : row_index;
        col_index = (dir == UP) ? --(col_index) :
            (dir == DOWN) ? ++(col_index) : col_index;

        gotoxy(row_index * 2 + 1, col_index + 1);
        setColor(12, 14);
        printf("★");                                                           //시각화에서 현재 위치를 나타냄
    }
    resetColor();
}





//DFS Algorithm
//push
void DFS_push(Coord* mStack, int* top, int row, int col)
{
    if (*top < ROW_SIZE * COL_SIZE - 1)
    {
        mStack[++ * top].row = row;     //Stack 구조체를 row와 col값에 대하여 push()하고 top + 1
        mStack[*top].col = col;
    }
    else puts("Stack Overflow");        //스택이 가득 찬 경우
}

//pop
Coord DFS_pop(Coord* mStack, int* top)
{
    if (*top < 0)                       //top이 -1 즉, 빈 경우
    {
        puts("Stack Underflow");
        mStack[ROW_SIZE * COL_SIZE].row = -1;
        mStack[ROW_SIZE * COL_SIZE].col = -1;
        return mStack[ROW_SIZE * COL_SIZE];
    }
    else return mStack[(*top)--];       //구조체 스택 반환 후 top - 1
}

//DFS main
double DFS(int** maze)
{
    double pst;                         //시간 측정 변수
    int col, row;                       //행렬

    //스택관련 부분
    Coord mStack[ROW_SIZE * COL_SIZE];
    int top = 0;                        //top에 첫번째에 0과 (1,1)을 저장
    mStack[0].col = 1;
    mStack[0].row = 1;
    Coord tmpStack;                     //Stack 비교를 위한 tmp값
    tmpStack.row = -1;                  //행 스택
    tmpStack.col = -1;                  //열 스택

    maze[1][1] = 3;                     //초기화(현재 위치)


    start = clock();

    while (1)
    {
        if (!DFS_CurrentPosition(maze, &row, &col))                             //현재 위치가 미로 안에 있는지 확인
        {
            if (row == ROW_SIZE - 2 && col == COL_SIZE - 2)                     // 도착
            {
                break;
            }

            if (row < ROW_SIZE - 1 && maze[row + 1][col] == 0)                  // 하
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);   //BACK이면 push
                maze[row][col] = 2;                                             //현재 위치는 경로
                maze[++row][col] = 3;                                           //현재 위치를 변경 (하)
                DFS_push(mStack, &top, row, col);                               //현재 위치 push
            }
            else if (col < COL_SIZE - 1 && maze[row][col + 1] == 0)             //우
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[row][++col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (row > 0 && maze[row - 1][col] == 0)                        // 상
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[--row][col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (col > 0 && maze[row][col - 1] == 0)                        // 좌
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[row][--col] = 3;
                DFS_push(mStack, &top, row, col);
            }

            else
            {
                maze[row][col] = 2;                                             //현재 위치는 경로
                tmpStack = DFS_pop(mStack, &top);                               //stack을 pop()
                row = tmpStack.row;
                col = tmpStack.col;
                maze[row][col] = BACK;                                          //pop시킨 행과 열에 BACK(4)값 저장
            }

        }
    }

    end = clock();
    pst = (double)(end - start) / CLK_TCK;
    return pst;
}

//미로안에 있는지 비교하는 함수
int DFS_CurrentPosition(int** maze, int* row, int* col)     //현재 위치
{
    int i, j;
    for (i = 0; i < ROW_SIZE; i++)
    {
        for (j = 0; j < COL_SIZE; j++)
        {
            if (maze[i][j] == 3 || maze[i][j] == BACK)      //현재위치나 BACK이 나오면 그 값을 현재 위치로 return
            {
                *row = i;
                *col = j;
                return 0;
            }
        }
    }
    return 1;                                               //BACK이나 현재위치(3)값이 없으면 return 1로 반복 종료
}

//DFS 시각화
void DFS_Visualize(int** maze)                              //위의 DFS main과 동일
{
    int col, row;

    Coord mStack[ROW_SIZE * COL_SIZE];
    int top = 0;
    mStack[0].col = 1;
    mStack[0].row = 1;
    Coord tmpStack;
    tmpStack.row = -1;
    tmpStack.col = -1;

    maze[1][1] = 3;
    PrintMaze(maze, ROW_SIZE, COL_SIZE);
    while (1)
    {
        if (!DFS_CurrentPosition(maze, &row, &col))
        {
            if (row == ROW_SIZE - 2 && col == COL_SIZE - 2)
            {
                break;
            }

            if (row < ROW_SIZE - 1 && maze[row + 1][col] == 0)
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                gotoxy(col * 2 + 1, row + 1);
                setColor(0, 14);
                printf("  ");                               //미로의 해결을 시각화(gotoxy)
                maze[++row][col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (col < COL_SIZE - 1 && maze[row][col + 1] == 0)
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                gotoxy(col * 2 + 1, row + 1);
                setColor(0, 14);
                printf("  ");
                maze[row][++col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (row > 0 && maze[row - 1][col] == 0)
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                gotoxy(col * 2 + 1, row + 1);
                setColor(0, 14);
                printf("  ");
                maze[--row][col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (col > 0 && maze[row][col - 1] == 0)
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                gotoxy(col * 2 + 1, row + 1);
                setColor(0, 14);
                printf("  ");
                maze[row][--col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else
            {
                maze[row][col] = 2;
                gotoxy(col * 2 + 1, row + 1);
                setColor(0, 14);
                printf("  ");
                tmpStack = DFS_pop(mStack, &top);
                row = tmpStack.row;
                col = tmpStack.col;
                maze[row][col] = BACK;
            }
            gotoxy(col * 2 + 1, row + 1);
            setColor(12, 14);
            printf("★");
            Sleep(30);
        }
        resetColor();
    }
}