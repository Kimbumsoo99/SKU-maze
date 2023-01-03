#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

#define ROW_SIZE 55     //�̷� ũ�� Ȧ���θ� ����
#define COL_SIZE 55
#define UP    1         //�켱�� ����Ʈ �����ڸ� �̿��� �����¿�
#define RIGHT 2
#define DOWN  4
#define LEFT  8

#define BACK  4         //DFS �ǵ��ư��� ����

typedef struct {        //��� ���� �����ϴ� ����ü
    int row;
    int col;
} Coord;

clock_t start, end;     //�ð� ���� ���� clock





void gotoxy(int x, int rows);                                                                       //gotoxy
void CursorView();                                                                                  //Ŀ�������
void setColor(int color, int bgcolor);                                                              //���� ����
void resetColor();                                                                                  //���� �ʱ�ȭ
void PrintMaze(int** maze, int rows, int cols);                                                     //�� ��� (�ð�ȭ)
int** CopyMaze(int** maze, int rows, int cols);                                                     //�� ����

//Maze_Generation
//Aldous_Broder Algorithm
int** Alodus_Reset(int rows, int cols);                                                             //Aldous_Broder �� �ʱ�ȭ
void Aldous_Broder(int** maze, int rows, int cols);                                                 //Aldous_Broder main

//Prim's Algorithm
int** Prim_Reset(int rows, int cols);                                                               //Prim �� �ʱ�ȭ
int Prim_Contains(Coord* array, int length, int row, int col);                                      //�迭�� �ش� ��ǥ�� ����Ǿ��ִ��� Ȯ���ϴ� �Լ�
int Prim_AddWalls(int** maze, int rows, int cols, Coord* array, int length, Coord aCoord);          //��ó�� ���� �迭�� �߰��ϴ� �Լ�
void Prim(int** maze, int rows, int cols);                                                          //Prim main

//Maze_Solve
//A* Algorithm
double Astar(int** maze, int** printmaze);                                                          //Astar main

//�켱��
int RightHand_WallAhead(int** maze, int row_index, int col_index, int dir);                         //�տ� ���� �ִ��� ���ϴ� �Լ�
double RightHand(int** maze, int row_index, int col_index, int dir);                                //�켱�� main
void RightHand_Visualize(int** maze, int row_index, int col_index, int dir);                        //�켱�� �ð�ȭ

//DFS Algorithm
void DFS_push(Coord* mStack, int* top, int row, int col);                                           //push
Coord DFS_pop(Coord* mStack, int* top);                                                             //pop
double DFS(int** maze);                                                                             //DFS main
int DFS_CurrentPosition(int** maze, int* row, int* col);                                            //�̷ξȿ� �ִ��� ���ϴ� �Լ�
void DFS_Visualize(int** maze);                                                                     //DFS �ð�ȭ




//main
int main(void) {
    int** maze = 0, ** maze_AStar = 0, ** maze_RightHand = 0, ** maze_DFS = 0, ** maze_DFS2 = 0;    //������ �̷θ� �����ϴ� map
    int num = 0;                                                                                    //���� �˰����� �����ϴ� ����
    int row_index = 1, col_index = 1, i = 0;
    double pst = 0;                                                                                 //�˰��� ����ð� ��������

    srand((unsigned)time(NULL));                                                                    //���� Seed ���� �ʱ�ȭ
    system("mode con cols=130 lines=70");                                                           //�ܼ� ������ ����
    SetConsoleTitle(TEXT("MAZE SOLVE"));                                                            //�ܼ� ���� ����

    //����� �������̽�(1. �����߰� ����)
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
    printf("                ��      ��  �����  �����  �����        �����  �����  ��     ��      ��  �����           \n");
    printf("                ���  ���  ��    ��       ��   ��              ��        ��    ��  ��      ��    ��   ��                 \n");
    printf("                ��  ��  ��  �����     ��     �����        �����  ��    ��  ��       ��  ��    �����           \n");
    printf("                ��      ��  ��    ��   ��       ��                    ��  ��    ��  ��        ���     ��                 \n");
    printf("                ��      ��  ��    ��  �����  �����        �����  �����  �����   ��      �����           \n");
    printf("\n\n\n\n\n\n\n\n\n\n");
    resetColor();
    printf("                                     ��������������������������  < �̷� ������ �����մϴ� > ��������������������������\n");
    printf("                                     ��                                                     ��\n");
    printf("                                     ��               �˰����� �����Ͻʽÿ�.              ��\n");
    printf("                                     ��                                                     ��\n");
    printf("                                     ��               �� Aldous_Broder Algorithm = 1        ��\n");
    printf("                                     ��               �� Prim's Algorithm = 2               ��\n");
    printf("                                     ��                                                     ��\n");
    printf("                                     ��                                                     ��\n");
    printf("                                     ��               ��ȣ�� �Է��Ͻʽÿ� :                 ��\n");
    printf("                                     ��������������������������������������������������������������������������������������������������������������\n");

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

    //����� �������̽�(2. ���� ���߰� ����)
    //gotoxy(1, 17);
    //printf("          ��������������������������������������������������������   \n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��     ��      ��  �����  �����  �����        �����  �����  ��     ��      ��  �����      ��   \n");
    //printf("          ��     ���  ���  ��    ��       ��   ��              ��        ��    ��  ��      ��    ��   ��            ��   \n");
    //printf("          ��     ��  ��  ��  �����     ��     �����        �����  ��    ��  ��       ��  ��    �����      ��   \n");
    //printf("          ��     ��      ��  ��    ��   ��       ��                    ��  ��    ��  ��        ���     ��            ��   \n");
    //printf("          ��     ��      ��  ��    ��  �����  �����        �����  �����  �����   ��      �����      ��   \n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��                                                                                                          ��\n");
    //printf("          ��������������������������������������������������������   \n");
    //printf("\n\n\n\n\n");
    //printf("                                     ��������������������������  < �̷� ������ �����մϴ� > ��������������������������\n");
    //printf("                                     ��                                                     ��\n");
    //printf("                                     ��               �˰����� �����Ͻʽÿ�.              ��\n");
    //printf("                                     ��                                                     ��\n");
    //printf("                                     ��               �� Aldous_Broder Algorithm = 1        ��\n");
    //printf("                                     ��               �� Prim's Algorithm = 2               ��\n");
    //printf("                                     ��                                                     ��\n");
    //printf("                                     ��                                                     ��\n");
    //printf("                                     ��               ��ȣ�� �Է��Ͻʽÿ� :                 ��\n");
    //printf("                                     ��������������������������������������������������������������������������������������������������������������\n");

    gotoxy(76, 46);
    scanf_s("%d", &num);

    //����ڰ� �߸� �Է��� ���
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
    printf("A* �˰����� �̿��� �̷��ذ��� �����մϴ�\n�ƹ�Ű�� �Է��Ͻʽÿ�\n");
    _getch();

    system("cls");
    pst = Astar(maze_AStar, maze);
    printf("\n\n");
    printf("�ذ� time : %f\n\n", pst);


    //�켱��
    maze_RightHand = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    printf("�켱���� �̿��� �̷��ذ��� �����մϴ�\n�ƹ�Ű�� �Է��Ͻʽÿ�\n");
    _getch();

    system("cls");
    pst = RightHand(maze_RightHand, row_index, col_index, RIGHT);
    PrintMaze(maze_RightHand, ROW_SIZE, COL_SIZE);
    RightHand_Visualize(maze_RightHand, row_index, col_index, RIGHT);
    printf("\n\n");
    printf("�ذ� time : %f\n\n", pst);


    //DFS Algorithm
    maze_DFS = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    maze_DFS2 = CopyMaze(maze, ROW_SIZE, COL_SIZE);
    printf("DFS�� �̿��� �̷��ذ��� �����մϴ�\n�ƹ�Ű�� �Է��Ͻʽÿ�\n");
    _getch();

    system("cls");
    pst = DFS(maze_DFS);
    DFS_Visualize(maze_DFS2);
    printf("\n\n");
    printf("�ذ� time : %f\n\n", pst);
    printf("��� �̷��ذ��� �����մϴ�\n�ƹ�Ű�� �Է��Ͻʽÿ�\n");
    _getch();

    return 0;
}





//gotoxy
void gotoxy(int x, int y) {
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//Ŀ�������
void CursorView()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//���� ����
void setColor(int color, int bgcolor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | (color & 0xf));
}

//���� �ʱ�ȭ
void resetColor() {
    setColor(15, 0);
}

//�� ��� (�ð�ȭ)
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
                printf("��");
                break;
            }
        }
        printf("\n");
    }
    resetColor();
}

//�� ����
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
//Aldous_Broder �� �ʱ�ȭ
int** Alodus_Reset(int rows, int cols) {
    int i, j;

    int** maze = (int**)(malloc(sizeof(int*) * rows));          //�̷θ� ���� ũ�⸸ŭ �����Ҵ�

    for (i = 0; i < rows; i++) {
        maze[i] = (int*)(malloc(sizeof(int) * cols));
        for (j = 0; j < cols; j++) {
            maze[i][j] = 1;                                     //������ ��� �迭��� �ʱ�ȭ
        }
    }

    return maze;
}

//ALdous_Broder main
void Aldous_Broder(int** maze, int rows, int cols) {
    int visited = 0;                                            //��� ��带 �� �湮�ߴ��� Ȯ���ϴ� ����
    int rotate;                                                 //���� ���� ������ ���ϴ� ����
    int row_index = 1, col_index = 1;                           //�ʱ��� ���������� (1,1)�� ����
    int i, j;

    for (i = 0; i < rows / 2; i++) {
        for (j = 0; j < cols / 2; j++) {
            visited++;                                          //����� ����ŭ visited ���� ����
        }
    }

    PrintMaze(maze, rows, cols);
    CursorView();                                               //Ŀ���� ����

    while (visited > 0) {
        rotate = rand() % 4 + 1;                                //���� ���� ������ �������� ����
        if (rotate == 1) {                  //��
            if (row_index == 1)
                continue;                                       //���� ���� �����ϴµ� ���� ���κ��� �ܺ��� �ش��Ѵٸ� continue
            else if (maze[row_index - 2][col_index] == 1) {     //���� ��尡 ���� ���̶��
                row_index--; maze[row_index][col_index] = 0;    //�� ��� ������ ���� ��� ����� �� �ڸ��� �̵�
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");                                   //�̷��� ������ �ð�ȭ(gotoxy)
                row_index--; maze[row_index][col_index] = 0;    //���� ��嵵 ��� ����� �� �ڸ��� �̵�
                gotoxy(col_index * 2 + 1, row_index + 1);
                printf("  ");                                   //�̷��� ������ �ð�ȭ(gotoxy)
                visited--;                                      //��带 �湮�����Ƿ� visited �������� 1��ŭ ����
            }
            else {                                              //���� ��尡 ���̶��
                row_index -= 2;                                 //���� ���� �̵��� ��
            }
        }
        if (rotate == 2) {                  //��
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
        if (rotate == 3) {                  //��
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
        if (rotate == 4) {                  //��
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
        Sleep(1);                                               //�̷��� ������ �ð�ȭ(system("cls"))
        /*PrintMaze(maze, rows, cols); */
        /*system("cls");*/
    }
    system("cls");
}





//Prim's Algorithm
//Prim �� �ʱ�ȭ
int** Prim_Reset(int rows, int cols) {
    int i, j;
    int** maze = (int**)(malloc(sizeof(int*) * rows));      //�̷θ� ���� ũ�⸸ŭ �����Ҵ�

    for (i = 0; i < rows; i++) {
        maze[i] = (int*)(malloc(sizeof(int) * cols));

        for (j = 0; j < cols; j++) {
            if (j % 2 != 0 && i % 2 != 0)                   //�̷θ� �׸��� ���·� �ʱ�ȭ
                maze[i][j] = 0;                             //���� 0
            else
                maze[i][j] = 1;                             //���� 1
        }
    }

    return maze;
}

//�迭�� �ش� ��ǥ�� ����Ǿ��ִ��� Ȯ���ϴ� �Լ�
int Prim_Contains(Coord* array, int length, int row, int col) {
    int i = 0;
    Coord temp;                                             //�ӽ� Coord ����ü
    temp.row = row;                                         //�Ű������� �Ѿ�� ��ǥ���� temp�� ����
    temp.col = col;

    for (i = 0; i < length; i++) {
        if (array[i].row == temp.row && array[i].col == temp.col)
            return 1;                                       //����ü �迭 array�� �ش� ��ǥ�� ����Ǿ� ������ 1�� ��ȯ
    }
    return 0;                                               //�ƴҰ�� 0�� ��ȯ
}

//��ó�� ���� �迭�� �߰��ϴ� �Լ�
int Prim_AddWalls(int** maze, int rows, int cols, Coord* array, int length, Coord aCoord) {
    int original_length = length;                           //�Ѿ�� �迭�� ���̸� �����ϴ� ����
    Coord temp_wall;                                        //���� ��ǥ�� �ӽ÷� �����ϴ� ����

    if (aCoord.row - 1 >= 0) {          //�� 
        temp_wall.row = aCoord.row - 1;                     //��ĭ ���� ��ǥ�� �ӽ÷� ����
        temp_wall.col = aCoord.col;
        //��ĭ ���� ��ǥ�� ���̰� ���� �� �迭�� ������� �ʾҴٸ�
        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row - 1, aCoord.col)) {
            array[length] = temp_wall;                      //�� �迭�� �ش� ���� �߰�
            length++;                                       //�� �迭�� ���̸� �ϳ� �ø�
        }
    }

    if (aCoord.row + 1 < rows) {        //��
        temp_wall.row = aCoord.row + 1;
        temp_wall.col = aCoord.col;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row + 1, aCoord.col)) {
            array[length] = temp_wall;
            length++;
        }
    }

    if (aCoord.col - 1 >= 0) {          //��
        temp_wall.row = aCoord.row;
        temp_wall.col = aCoord.col - 1;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row, aCoord.col - 1)) {
            array[length] = temp_wall;
            length++;
        }
    }

    if (aCoord.col + 1 < cols) {        //��
        temp_wall.row = aCoord.row;
        temp_wall.col = aCoord.col + 1;

        if (maze[temp_wall.row][temp_wall.col] && !Prim_Contains(array, length, aCoord.row, aCoord.col + 1)) {
            array[length] = temp_wall;
            length++;
        }
    }

    return length - original_length;    //�� ���� ���� �迭�� �߰��Ǿ����� ��ȯ
}

//Prim main
void Prim(int** maze, int rows, int cols) {
    Coord* wallArr = (Coord*)malloc(sizeof(Coord) * rows * cols);           //�� �迭(����ü �迭�� ���� ��ǥ���� ����)
    Coord* unvisitedArr = (Coord*)malloc(sizeof(Coord) * rows * cols);      //�湮���� ���� ��� �迭
    Coord temp, firstnode, unvisited_node, wall;                            //�ӽ� ��ǥ����ü, ó�� �湮�� ��屸��ü, �湮���� ���� Ư�� ��屸��ü, ��Ƴ� �� ����ü
    int wallArr_length = 0, unvisitedArr_length = 0;                        //�� �迭�� ���� ����, unvisitedArr �迭�� ���� ����
    int firstnode_index, wallIndex;                                         //ó�� �湮�� ����� Index ����, ��Ƴ� ���� Index ����
    int i = 0, j = 0, k = 0;
    bool near_unvisted_node = false;                                        //�湮���� ���� ��� ������ �Ǵ��ϴ� ����

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (maze[i][j] == 0) {                                          //��尡 0
                temp.row = i;                                               //�ӽ� ��ǥ����ü�� i�� j�� ��ǥ���� ����
                temp.col = j;
                unvisitedArr[unvisitedArr_length++] = temp;                 //�湮���� ���� ��� �迭�� ������ �߰���Ŵ
            }
        }
    }

    PrintMaze(maze, rows, cols);                                            //�ʱ�ȭ�� �̷θ� ���
    CursorView();                                                           //�ܼ�â�� Ŀ���� ����

    firstnode_index = rand() % unvisitedArr_length;                         //ó������ �湮�� ����� �ε����� �湮���� ���� ����� ���̷� ���� �������� ������
    firstnode = unvisitedArr[firstnode_index];                              //ó������ �湮�� ����� ��ǥ���� �����Ŵ

    //unvisitedArr �迭���� �湮�� ����� ��ǥ���� �����ϴ� �κ�             
    for (i = firstnode_index + 1; i < unvisitedArr_length; i++) {           //i���� �湮�� ����� �ε����� �����ϰ� �ݺ����� ����
        unvisitedArr[i - 1] = unvisitedArr[i];                              //�湮�� ����� ��ǥ�� ��ġ�� �� ���� ��ǥ���� ������ ������ ��ĭ�� �� ���
    }
    unvisitedArr_length -= 1;                                               //unvisitedArr �迭�� ���̸� �ϳ� ����          

    wallArr_length += Prim_AddWalls(maze, rows, cols, wallArr, wallArr_length, firstnode); /*ó������ �湮�� ��� ��ó�� ������ �� �迭�� �߰���Ű��
                                                                                             �߰��� �� ������ŭ wallArr_length�� ����������*/

                                                                                             //�� �迭�� �� �������� �̷ο��� ���� ���� �κ�
    while (wallArr_length > 0) {
        near_unvisted_node = false;                                         //�湮���� ���� ��� ������ �Ǵ��ϴ� ������ �湮���� ���� ��� ���� �ƴ϶�� �ʱ�ȭ
        wallIndex = rand() % wallArr_length;                                //��Ƴ� ���� �ε����� �� �迭�� ���̷� ���� �������� ������
        wall = wallArr[wallIndex];                                          //��Ƴ� ���� ��ǥ���� �����Ŵ

        //�� �迭���� ��Ƴ� ���� ��ǥ���� �����ϴ� �κ�                        
        for (k = wallIndex + 1; k < wallArr_length; k++) {                  //k���� ��Ƴ� ���� �ε����� �����ϰ� �ݺ����� ����
            wallArr[k - 1] = wallArr[k];                                    //��Ƴ� ���� ��ǥ ��ġ�� �� ���� ��ǥ���� ������ ������ ��ĭ�� �� ���
        }
        wallArr_length--;                                                   //�� �迭�� ���̸� �ϳ� ����

        //wall�� ����� ���� �����¿츦 Ȯ���Ͽ� ���� ����� �湮���� ���� ��带 ã�Ƴ�
        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row - 1, wall.col)) {     //��
            unvisited_node.row = wall.row - 1;                              //�湮���� ���� ����� ��ǥ���� ����
            unvisited_node.col = wall.col;
            near_unvisted_node = true;                                      //�湮���� ���� ��� ��ó�� �´ٰ� true ���� ����
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row + 1, wall.col)) {     //��
            unvisited_node.row = wall.row + 1;
            unvisited_node.col = wall.col;
            near_unvisted_node = true;
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row, wall.col - 1)) {     //��
            unvisited_node.row = wall.row;
            unvisited_node.col = wall.col - 1;
            near_unvisted_node = true;
        }

        if (Prim_Contains(unvisitedArr, unvisitedArr_length, wall.row, wall.col + 1)) {     //��
            unvisited_node.row = wall.row;
            unvisited_node.col = wall.col + 1;
            near_unvisted_node = true;
        }

        if (near_unvisted_node) {                                                       //�湮���� ���� ��� ��ó�϶� ����
            maze[wall.row][wall.col] = 0;                                               //��Ƴ� ���� ��Ƽ� ���� ���ϴ� 0���� ����
            gotoxy(wall.col * 2 + 1, wall.row + 1);
            setColor(0, 0);
            printf("  ");                                                               //�̷��� ������ �ð�ȭ(gotoxy)

            for (i = 0; i < unvisitedArr_length; i++) {                                 //unvisitedArr �迭���� �ش� ����� ��ǥ�� ���������ν� �ش� ��带 �湮��Ű�� �κ�
                if (unvisitedArr[i].row == unvisited_node.row && unvisitedArr[i].col == unvisited_node.col) //unvisited �迭���� �ش� ���� ��ǥ�� ��ġ�ϴ� ��带 ã��
                    for (j = i + 1; j < unvisitedArr_length; j++) {
                        unvisitedArr[j - 1] = unvisitedArr[j];                          //�ش� ����� ��ǥ�� ��ġ�� �� ���� ��ǥ���� ������ ������ ��ĭ�� �� ���
                    }
            }
            unvisitedArr_length -= 1;                                                   //unvisitedArr �迭�� ���̸� �ϳ� ����         

            wallArr_length += Prim_AddWalls(maze, rows, cols, wallArr, wallArr_length, unvisited_node); /*�湮�� ��� ��ó�� ������ �� �迭�� �߰���Ű��
                                                                                                          �߰��� �� ������ŭ wallArr_length�� ����������*/
        }
        //PrintMaze(maze, rows, cols);                                                  //�̷��� ������ �ð�ȭ(system("cls"))
        Sleep(1);
        //system("cls");
    }
    system("cls");
}





//A* Algorithm
double Astar(int** maze, int** printmaze) {
    typedef struct node {
        int g, h, f;                                            //g�� start���� �̵��ϴ� �Ÿ�, h�� finish������ �Ÿ�
        int open;                                               //���� ������� Ȯ���ϴ� ����
        int col, row;                                           //��� ��
        int mom_node_col, mom_node_row;                         //�θ��忡 �࿭
        int past_node;                                          //���������� Ȯ��(���� ���) 0�̸� ������ ��
    } open_node;

    start = clock();
    double pst;                                                 //�ذ� �ð� ���� ����
    int i, j, k, l;                                             //�ݺ�����
    int r_len = 0;                                              //�ذ��Ʈ�� ���� �迭 ���� ����
    Coord* Coords = NULL;                                       //��� ���� �����ϴ� ����ü
    open_node** block;                                          //���� ũ�⸸ŭ �� ��� ����ü ����
    block = (open_node**)calloc(ROW_SIZE, sizeof(open_node*));
    for (i = 0; i < ROW_SIZE; i++) {
        block[i] = (open_node*)calloc(COL_SIZE, sizeof(open_node));
    }
    int tmp_g = -1, tmp_f = 100000000;                          //��� �� �κ��� ���� ����
    block[1][1].past_node = 1;                                  //���� ���� (1,1)�� ���� �� �ʱ�ȭ
    block[1][1].g = 0;
    block[1][1].h = (ROW_SIZE - 2 - 2) * 10 + (COL_SIZE - 2 - 2) * 10;
    block[1][1].f = block[1][1].h + block[1][1].g;
    block[1][1].row = 1;
    block[1][1].col = 1;
    i = 1, j = 1;

    //���� ��� ���� �κ�
    while (1) {
        if (block[i][j].open == 1 || (i == 1 && j == 1)) {                                                  //���������̰ų� ������� 
            if (maze[i - 1][j] != 1 && block[i - 1][j].open == 0 && block[i - 1][j].past_node == 0) {       //���� ���� �ƴϰ� ������尡 �ƴѵ� ���� ��ΰ� �ƴϸ�
                block[i - 1][j].open = 1;                                                                   //�� row-1
                block[i - 1][j].row = i - 1;                                                                //�� ��Ͽ� ��� ��
                block[i - 1][j].col = j;
                block[i - 1][j].mom_node_row = i;                                                           //�� ��Ͽ� ���� ���� ���� ���
                block[i - 1][j].mom_node_col = j;
                block[i - 1][j].h = (ROW_SIZE - 2 - (i - 1)) * 10 + (COL_SIZE - 2 - j) * 10;                //�������������� ���� ���� �Ÿ� 
                block[i - 1][j].g = block[i][j].g + 10;                                                     //���� ��忡 g + 10 ��
                block[i - 1][j].f = block[i - 1][j].h + block[i - 1][j].g;
            }
            if (maze[i + 1][j] != 1 && block[i + 1][j].open == 0 && block[i + 1][j].past_node == 0) {       //�Ʒ�
                block[i + 1][j].open = 1;                                                                   //�Ʒ� row+1
                block[i + 1][j].row = i + 1;
                block[i + 1][j].col = j;
                block[i + 1][j].mom_node_row = i;
                block[i + 1][j].mom_node_col = j;
                block[i + 1][j].h = (ROW_SIZE - 2 - (i + 1)) * 10 + (COL_SIZE - 2 - j) * 10;
                block[i + 1][j].g = block[i][j].g + 10;
                block[i + 1][j].f = block[i + 1][j].h + block[i + 1][j].g;
            }
            if (maze[i][j + 1] != 1 && block[i][j + 1].open == 0 && block[i][j + 1].past_node == 0) {       //������
                block[i][j + 1].open = 1;                                                                   //������ col+1
                block[i][j + 1].row = i;
                block[i][j + 1].col = j + 1;
                block[i][j + 1].mom_node_row = i;
                block[i][j + 1].mom_node_col = j;
                block[i][j + 1].h = (ROW_SIZE - 2 - i) * 10 + (COL_SIZE - 2 - (j + 1)) * 10;
                block[i][j + 1].g = block[i][j].g + 10;
                block[i][j + 1].f = block[i][j + 1].h + block[i][j + 1].g;
            }
            if (maze[i][j - 1] != 1 && block[i][j - 1].open == 0 && block[i][j - 1].past_node == 0) {       //����
                block[i][j - 1].open = 1;                                                                   //���� col-1
                block[i][j - 1].row = i;
                block[i][j - 1].col = j - 1;
                block[i][j - 1].mom_node_row = i;
                block[i][j - 1].mom_node_col = j;
                block[i][j - 1].h = (ROW_SIZE - 2 - i) * 10 + (COL_SIZE - 2 - (j - 1)) * 10;
                block[i][j - 1].g = block[i][j].g + 10;
                block[i][j - 1].f = block[i][j - 1].h + block[i][j - 1].g;
            }
        }

        //������� �� �κ�
        for (k = 1; k < ROW_SIZE - 1; k++) {                                        //�̷� ��ü�� Ȯ����
            for (l = 1; l < COL_SIZE - 1; l++) {
                if (block[k][l].open == 1 && block[k][l].past_node == 0) {          //�������鼭 ���� ��尡 �ƴ� ���
                    if (tmp_f > block[k][l].f) {                                    //����� �ӽ� f������ ���� f���� ������
                        tmp_f = block[k][l].f;                                      //���� ��忡 f,g������ tmp���� ����
                        tmp_g = block[k][l].g;
                        i = k;                                                      //���õ� ���� ��带 ���� ��忡 ��ǥ�� ����
                        j = l;
                    }
                    else if (tmp_f == block[k][l].f && tmp_g < block[k][l].g) {     /*����� �ӽ� f���� �ݺ����� ����Ű�� ��� f���� ������
                                                                                      �ӽ� g���� block�� g���� ��, g���� �� ������ �۵���*/

                        tmp_g = block[k][l].g;                                      //���� f,g�� ��� ������ ����(��) �켱���� �̵�
                        i = k;
                        j = l;
                    }
                }
            }
        }
        block[i][j].past_node = 1;                                                  //���� ���� ���õ� ���� ���� ���� ����
        tmp_g = 0;                                                                  //tmp_g �ʱ�ȭ
        tmp_f = 10000000;                                                           //tmp_f �ʱ�ȭ
        if (i == ROW_SIZE - 2 && j == COL_SIZE - 2) {                               //����� ã�� ���
            break;
        }
    }

    //����ڿ��� �ذ� ������ �����ִ� �κ�
    k = 0;
    while (1) {                                                                     //i�� ��,j�� ���� 1,1 ���������� �������� �ݺ�
        r_len++;
        Coords = (Coord*)realloc(Coords, r_len * sizeof(Coord));                    //�ذ� ��Ʈ�� ���� ����ü�迭
        if (k == 0) {
            maze[i][j] = 2;                                                         //k=0�϶� i , j�� ���� ����
            printf("���� [%d] (%d,%d)\n", k, block[i][j].row, block[i][j].col);
        }
        Coords[k].row = block[i][j].mom_node_row;                                   //��Ʈ�� row�� �θ��忡 ��
        Coords[k].col = block[i][j].mom_node_col;                                   //��Ʈ�� col�� �θ��忡 ��
        i = Coords[k].row;                                                          //�θ��忡 �� ����
        j = Coords[k].col;                                                          //�θ��忡 �� ����
        k++;                                                                        //���� ��Ʈ��ǥ
        maze[i][j] = 2;                                                             //�θ���� ����� ���� ��带 ���� ��η� ����
        printf("���� [%d] (%d,%d)\n", k, block[i][j].row, block[i][j].col);

        if (i == 1 && j == 1) {                                                     //���� �������� ���ƿ���
            Coords[k].row = 1;
            Coords[k].col = 1;
            break;
        }
    }
    end = clock();

    _getch();
    system("cls");
    PrintMaze(printmaze, ROW_SIZE, COL_SIZE);                       //�ذ������ ���� �� �̷� ����
    for (l = r_len; l > 0; l--) {                                   //���� ��� �ð�ȭ
        Sleep(50);
        gotoxy(Coords[l].col * 2 + 1, Coords[l].row + 1);
        setColor(0, 14);
        printf("  ");
        gotoxy(Coords[l - 1].col * 2 + 1, Coords[l - 1].row + 1);
        setColor(12, 14);
        printf("��");
    }
    gotoxy(Coords[l].col * 2 + 1, Coords[l].row + 1);
    setColor(0, 14);
    printf("  ");
    gotoxy(COL_SIZE * 2 - 3, ROW_SIZE - 1);
    setColor(12, 14);
    printf("��");
    resetColor();

    for (i = 0; i < ROW_SIZE; i++) {                                //�����Ҵ� ����
        free(block[i]);
    }
    free(block);
    pst = (double)(end - start) / CLK_TCK;
    return pst;                                                     //�����ð� ��ȯ
}





//�켱��
//�տ� ���� �ִ��� ���ϴ� �Լ�
int RightHand_WallAhead(int** maze, int row_index, int col_index, int dir) {   //�켱�� ����(dir) �տ� ���� �ִ��� Ȯ��
    row_index = (dir == LEFT) ? --row_index :                                  //���� ������ LEFT�� --row, RIGHT�� ++row
        (dir == RIGHT) ? ++row_index : row_index;
    col_index = (dir == UP) ? --col_index :                                    //���� ������ UP�̸� --col, DOWN�̸� ++col
        (dir == DOWN) ? ++col_index : col_index;
    return maze[col_index][row_index];                                         //���� �����ڿ� ���� ��ġ�� ���Ͽ� �� ��ȯ  �� = 1, �� = 0
}

//�켱�� main
double RightHand(int** maze, int row_index, int col_index, int dir) {
    double pst;                                                                //�ð� ���� ����

    start = clock();
    while (row_index > 0 && row_index < COL_SIZE - 1 && col_index>0 && col_index < ROW_SIZE - 1 && (row_index != COL_SIZE - 2 || col_index != ROW_SIZE - 2)) {
        dir <<= 1;                                                             //����Ʈ ������ bit���� ���������� ��ĭ �����Դϴ�.
        dir = (dir > LEFT) ? UP : dir;                                         //���� ���������� ������. UP->RIGHT->DOWN->LEFT
        while (RightHand_WallAhead(maze, row_index, col_index, dir)) {         //dir ���� ���� ������ Ȯ���ؼ� ��(0)�̸� ��� ��(1)�̸� �ݺ��� ����
            dir >>= 1;                                                         //�������� bit ����Ʈ
            dir = (dir == 0) ? LEFT : dir;
        }
        //���̶�� ���� �ƴҶ����� ���� �������� ����. LEFT->DOWN->RIGHT->UP
        row_index = (dir == LEFT) ? --(row_index) : (dir == RIGHT) ? ++(row_index) : row_index;   //���� dir ������ row,col�� �̵��մϴ�.
        col_index = (dir == UP) ? --(col_index) : (dir == DOWN) ? ++(col_index) : col_index;
    }
    end = clock();

    pst = (double)(end - start) / CLK_TCK;
    return pst;
}

//�켱�� �ð�ȭ
void RightHand_Visualize(int** maze, int row_index, int col_index, int dir) {  //�켱�� �Լ��� �ð�ȭ �κ��� �߰��� �Լ�
    gotoxy(row_index + 2, col_index + 1);
    setColor(0, 14);
    printf("  ");

    //���� �켱�� main�� ����
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
        printf("��");                                                           //�ð�ȭ���� ���� ��ġ�� ��Ÿ��
    }
    resetColor();
}





//DFS Algorithm
//push
void DFS_push(Coord* mStack, int* top, int row, int col)
{
    if (*top < ROW_SIZE * COL_SIZE - 1)
    {
        mStack[++ * top].row = row;     //Stack ����ü�� row�� col���� ���Ͽ� push()�ϰ� top + 1
        mStack[*top].col = col;
    }
    else puts("Stack Overflow");        //������ ���� �� ���
}

//pop
Coord DFS_pop(Coord* mStack, int* top)
{
    if (*top < 0)                       //top�� -1 ��, �� ���
    {
        puts("Stack Underflow");
        mStack[ROW_SIZE * COL_SIZE].row = -1;
        mStack[ROW_SIZE * COL_SIZE].col = -1;
        return mStack[ROW_SIZE * COL_SIZE];
    }
    else return mStack[(*top)--];       //����ü ���� ��ȯ �� top - 1
}

//DFS main
double DFS(int** maze)
{
    double pst;                         //�ð� ���� ����
    int col, row;                       //���

    //���ð��� �κ�
    Coord mStack[ROW_SIZE * COL_SIZE];
    int top = 0;                        //top�� ù��°�� 0�� (1,1)�� ����
    mStack[0].col = 1;
    mStack[0].row = 1;
    Coord tmpStack;                     //Stack �񱳸� ���� tmp��
    tmpStack.row = -1;                  //�� ����
    tmpStack.col = -1;                  //�� ����

    maze[1][1] = 3;                     //�ʱ�ȭ(���� ��ġ)


    start = clock();

    while (1)
    {
        if (!DFS_CurrentPosition(maze, &row, &col))                             //���� ��ġ�� �̷� �ȿ� �ִ��� Ȯ��
        {
            if (row == ROW_SIZE - 2 && col == COL_SIZE - 2)                     // ����
            {
                break;
            }

            if (row < ROW_SIZE - 1 && maze[row + 1][col] == 0)                  // ��
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);   //BACK�̸� push
                maze[row][col] = 2;                                             //���� ��ġ�� ���
                maze[++row][col] = 3;                                           //���� ��ġ�� ���� (��)
                DFS_push(mStack, &top, row, col);                               //���� ��ġ push
            }
            else if (col < COL_SIZE - 1 && maze[row][col + 1] == 0)             //��
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[row][++col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (row > 0 && maze[row - 1][col] == 0)                        // ��
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[--row][col] = 3;
                DFS_push(mStack, &top, row, col);
            }
            else if (col > 0 && maze[row][col - 1] == 0)                        // ��
            {
                if (maze[row][col] == BACK) DFS_push(mStack, &top, row, col);
                maze[row][col] = 2;
                maze[row][--col] = 3;
                DFS_push(mStack, &top, row, col);
            }

            else
            {
                maze[row][col] = 2;                                             //���� ��ġ�� ���
                tmpStack = DFS_pop(mStack, &top);                               //stack�� pop()
                row = tmpStack.row;
                col = tmpStack.col;
                maze[row][col] = BACK;                                          //pop��Ų ��� ���� BACK(4)�� ����
            }

        }
    }

    end = clock();
    pst = (double)(end - start) / CLK_TCK;
    return pst;
}

//�̷ξȿ� �ִ��� ���ϴ� �Լ�
int DFS_CurrentPosition(int** maze, int* row, int* col)     //���� ��ġ
{
    int i, j;
    for (i = 0; i < ROW_SIZE; i++)
    {
        for (j = 0; j < COL_SIZE; j++)
        {
            if (maze[i][j] == 3 || maze[i][j] == BACK)      //������ġ�� BACK�� ������ �� ���� ���� ��ġ�� return
            {
                *row = i;
                *col = j;
                return 0;
            }
        }
    }
    return 1;                                               //BACK�̳� ������ġ(3)���� ������ return 1�� �ݺ� ����
}

//DFS �ð�ȭ
void DFS_Visualize(int** maze)                              //���� DFS main�� ����
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
                printf("  ");                               //�̷��� �ذ��� �ð�ȭ(gotoxy)
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
            printf("��");
            Sleep(30);
        }
        resetColor();
    }
}