
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#define H 5
#define W 5
//#define DEBUG

using namespace std;
int path[12]={0};
bool F = 0;
int other;
int me;

struct Node {
    int m;              //步數標記 0 ~ 5 步
    int x;
    int y;
    int level;          //重要度
    bool f;
    bool cango;
    struct Node* pUp;
    struct Node* pDown;
    struct Node* pLeft;
    struct Node* pRight;
    struct Node* pBack;
};
/*

void checkerboard(int A[2][H][W], int M[2][2], int O[2][2]) {
    int i, j;
    for (int c = 0; c < 2; c++) {
        for (i = 0; i < H; i++) {
            for (j = 0; j < W; j++) {
                if ((abs(O[c][0] - i) + abs(O[c][1] - j)) % 2 == 1 && (abs(O[c][0] - i) + abs(O[c][1] - j)) <= 5) {
                    A[1][i][j] = 1;
                }
            }
        }
    }
    
}
*/
void getPosition(int A[2][H][W], int M[2][2], int O[2][2]) {       //取得自己與對方位置座標 (棋盤的x, y)
    int i, j;
    int c = 0;
    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++) {
            if (A[0][i][j] == me) {
                M[c][0] = j;
                M[c][1] = i;
                c++;
            }
        }
    }
    c = 0;
    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++) {
            if (A[0][i][j] == other) {
                O[c][0] = j;
                O[c][1] = i;
                c++;
            }
        }
    }
}
int check(int A[2][H][W],int M[2][2], int O[2][2]) {        //判斷我方兩個棋子是否有被吃或吃掉對方的可能性，如果有，則優先移動此棋子(優先移動的棋子移到A[0][0]、A[0][1]位置)
    int x, y;
    int i, j;
    int sum;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            x = abs(M[i][0] - O[j][0]);
            y = abs(M[i][1] - O[j][1]);
            sum = x + y;
            if (sum % 2 == 1 && sum <= 5) {         //距離為奇數 ( 有被吃或吃掉對方的可能 )
                return i;
            }
        }
    }
    return 0;   
}
void fTo1(struct Node* p) {
    path[0] = p->y;
    path[1] = p->x;
    int c=2;
    while (p && c < 12) {
        p->f = 1;
        p->level = 2;
        if (p->pBack != 0) {
            p = p->pBack;    
        }
        path[c] = p->y;
        path[c + 1] = p->x;
        c += 2;
    }
}
bool judge(int x, int y, int A[2][H][W], struct Node* p) {      //x, y 為欲到達的點，判斷此點是否與tmp的x, y一樣，有一樣的錶此點被走過，不可在次走到此點
    struct Node* tmp;
    tmp = p;
    while (tmp) {
        if (tmp->x==x && tmp->y==y) {
            return 1;       //此點被走過
        }
        else if (tmp->pBack) {
            tmp = tmp->pBack;
        }
        else {
            return 0;       //此點沒被走過
        }
    }
}
void addNode(struct Node* p, int A[2][H][W], int X, int Y, int m) {       //增加p點後的分支，p位置為X,Y，tmp新節點為上下左右某位置(x,y為棋盤的x, y)，第m步
    if (m < 6) {
        if (m == 5) {
            if (A[0][Y - 1][X] == other && Y>0) {
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X;
                tmp.y = Y - 1;
                tmp.level = 2;
                tmp.f = 1;
                tmp.cango = 1;
                p->pUp = &tmp;
                p->pDown = 0;
                p->pLeft = 0;
                p->pRight = 0;
                F = 1;
                fTo1(&tmp);
            }
            else if (A[0][Y + 1][X] == other && Y<4) {
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X;
                tmp.y = Y + 1;
                tmp.level = 2;
                tmp.f = 1;
                tmp.cango = 1;
                p->pUp = 0;
                p->pDown = &tmp;
                p->pLeft = 0;
                p->pRight = 0;
                F = 1;
                fTo1(&tmp);
            }
            else if (A[0][Y][X - 1] == other && X>0) {
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X - 1;
                tmp.y = Y;
                tmp.level = 2;
                tmp.f = 1;
                tmp.cango = 1;
                p->pUp = 0;
                p->pDown = 0;
                p->pLeft = &tmp;
                p->pRight = 0;
                F = 1;
                fTo1(&tmp);
            }
            else if (A[0][Y][X + 1] == other && X<4) {
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X + 1;
                tmp.y = Y;
                tmp.level = 2;
                tmp.f = 1;
                tmp.cango = 1;
                p->pUp = 0;
                p->pDown = 0;
                p->pLeft = 0;
                p->pRight = &tmp;
                F = 1;
                fTo1(&tmp);
            }
        }
        if (A[0][Y - 1][X] == 0 && Y>0) {
            if (!judge(X, Y - 1, A, p)) {
                p->cango = 1;
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X;
                tmp.y = Y - 1;
                tmp.f = 0;
                tmp.level = 1;
                tmp.cango = 1;
                p->pUp = &tmp;
                addNode(&tmp, A, X, Y - 1, m + 1);
            }
            else p->pUp = 0;
        }
        else
            p->pUp = 0;
        if (A[0][Y + 1][X] == 0 && Y<4) {
            if (!judge(X, Y + 1, A, p)) {
                p->cango = 1;
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X;
                tmp.y = Y + 1;
                tmp.f = 0;
                tmp.level = 1;
                tmp.cango = 1;
                p->pDown = &tmp;
                addNode(&tmp, A, X, Y + 1, m + 1);
            }
            else p->pDown = 0;
        }
        else
            p->pDown = 0;
        if (A[0][Y][X - 1] == 0 && X>0) {
            if (!judge(X - 1, Y, A, p)) {
                p->cango = 1;
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X - 1;
                tmp.y = Y;
                tmp.f = 0;
                tmp.level = 1;
                tmp.cango = 1;
                p->pLeft = &tmp;
                addNode(&tmp, A, X - 1, Y, m + 1);
            }
            else p->pLeft = 0;
        }
        else
            p->pLeft = 0;
        if (A[0][Y][X + 1] == 0 && X<4) {
            if (!judge(X + 1, Y, A, p)) {
                p->cango = 1;
                struct Node tmp = { 0 };
                tmp.pBack = p;
                tmp.m = m;
                tmp.x = X + 1;
                tmp.y = Y;
                tmp.f = 0;
                tmp.level = 1;
                tmp.cango = 1;
                p->pRight = &tmp;
                addNode(&tmp, A, X + 1, Y, m + 1);
            }
            else p->pRight = 0;
        }
        else
            p->pRight = 0;
    }
}
/*
int max(struct Node* p) {
    int up = 0, down = 0, left = 0, right = 0;
    struct Node *tmp;
    if (p->pUp) {
        tmp = p->pUp;
        up = tmp->level;
    }
    if (p->pDown && p->pDown->level) {
        down = p->pDown->level;
    }
    if (p->pLeft && p->pLeft->level) {
        left = p->pLeft->level;
    }
    if (p->pRight && p->pRight->level) {
        right = p->pRight->level;
    }
    int tmp1, tmp2;
    tmp1 = up >= down ? 1 : 2;
    tmp2 = left >= right ? 3 : 4;
    return tmp1 >= tmp2 ? tmp1 : tmp2;
}

void walk(struct Node* p) {
    ofstream out;
    out.open("play.txt");
    int road;
    int counter = 0;
    road = max(p);
    struct Node *tmp;
    tmp = p;
    while (tmp) {
        road = max(tmp);      //road為重要性最大的路徑的編號
        switch (road)
        {
        case 1:
            if (tmp->pUp) {
                tmp = tmp->pUp;
                out << tmp->y << " " << tmp->x << " ";
            }
            break;
        case 2:
            if (tmp->pDown) {
                tmp = tmp->pDown;
                out << tmp->y << " " << tmp->x << " ";
            }
            break;
        case 3:
            if (tmp->pLeft) {
                tmp = tmp->pLeft;
                out << tmp->y << " " << tmp->x << " ";
            }
            break;
        case 4:
            if (tmp->pRight != 0) {
                tmp = tmp->pRight;
                out << tmp->y << " " << tmp->x << " ";
            }
            break;
        }
        counter++;
    }
    out.close();
}
*/
/*void walk2(struct Node* p) {
    ofstream out;
    out.open("play.txt");
    out << p->y << " " << p->x;
    struct Node* tmp;
    tmp = p;
    while(tmp)
    if (p->pUp != 0 && p->pUp->f==1) {
        tmp = tmp->pUp;
        out << tmp->y << " " << tmp->x << " ";
    }
    else if (p->pDown !=0 && p->pDown->f==1) {
        tmp = tmp->pDown;
        out << tmp->y << " " << tmp->x << " ";
    }
    else if (p->pLeft != 0 && p->pLeft->f==1) {
        tmp = tmp->pLeft;
        out << tmp->y << " " << tmp->x << " ";
    }
    else if (p->pRight !=0 && p->pRight->f==1) {
        tmp = tmp->pRight;
        out << tmp->y << " " << tmp->x << " ";
    }
}
*/
int main(int argc, char** argv)
{
    int i, j;
    int A[2][H][W];    //棋盤
    int k = 0;
    me = atoi(argv[2]);
    other = (me == 1 ? 2 : 1);
    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++) {
            A[0][i][j] = atoi(argv[k + 3]);
            k++;
        }
    }
    
    int M[2][2];        //我的棋子的位置 (棋盤的x,y)
    int O[2][2];        //對方棋子的位置 (棋盤的x,y)
    getPosition(A, M, O);
    int firstChess = check(A, M, O);            //第0或第1顆棋子
    int chess[2] = { M[firstChess][0],M[firstChess][1] };       //棋盤的x,y 
    struct Node C1 = {0};
    C1.x = M[firstChess][0];
    C1.y = M[firstChess][1];
    C1.level = 1;
    C1.cango = 0;
    //checkerboard(A, M, O);
    if (A[0][M[firstChess][0] - 1][M[firstChess][1]] == 0 || A[0][M[firstChess][0] + 1][M[firstChess][1]] == 0 || A[0][M[firstChess][0]][M[firstChess][1] - 1] == 0 || A[0][M[firstChess][0]][M[firstChess][1] + 1] == 0) { 
        addNode(&C1, A, M[firstChess][0], M[firstChess][1], 1);
    }
    else {
        firstChess = firstChess == 0 ? 1 : 0;
        addNode(&C1, A, M[firstChess][0], M[firstChess][1], 1);
    }
     //walk(&C1);
    ofstream out;
    out.open("play.txt");
    if (F == 1) {
        for (i = 11; i >= 0; i--) {
            out << path[i] << " ";
        }
    }
    else {
        if (A[0][chess[1] + 1][chess[0] + 1] == 0 && A[0][chess[1]][chess[0] + 1] == 0 && chess[1]<4 && chess[0]<4) {
            out << chess[0] << " " << chess[1] << " ";
            out << chess[0] + 1 << " " << chess[1] << " ";
            out << chess[0] + 1 << " " << chess[1] + 1 << " ";
        }
        else if (A[0][chess[1] - 1][chess[0] - 1] == 0 && A[0][chess[1]][chess[0] - 1] == 0 && chess[1] > 0 && chess[0] > 0) {
            out << chess[0] << " " << chess[1] << " ";
            out << chess[0] - 1 << " " << chess[1] << " ";
            out << chess[0] - 1 << " " << chess[1] - 1 << " ";
        }
        else if (chess[1] < 4 && chess[0] > 0) {
            if(A[0][chess[1] + 1][chess[0] - 1] == 0 && A[0][chess[1]][chess[0] - 1] == 0){
                out << chess[0] << " " << chess[1] << " ";
                out << chess[0] - 1 << " " << chess[1] << " ";
                out << chess[0] - 1 << " " << chess[1] + 1 << " ";
            }
        }
        else if (chess[1] > 0 && chess[0] < 4) {
            if (A[0][chess[1] - 1][chess[0] + 1] == 0 && A[0][chess[1]][chess[0] - 1] == 0) {
                out << chess[0] << " " << chess[1] << " ";
                out << chess[0] - 1 << " " << chess[1] << " ";
                out << chess[0] + 1 << " " << chess[1] - 1 << " ";
            }

        }
        else if (chess[0] < 3) {
            if (A[0][chess[1]][chess[0] + 1] == 0 && A[0][chess[1]][chess[0] + 2] == 0) {
                out << chess[0] << " " << chess[1] << " ";
                out << chess[0] + 1 << " " << chess[1] << " ";
                out << chess[0] + 2 << " " << chess[1] << " ";
            }  
        }
        else if (chess[0] > 1) {
            if (A[0][chess[1]][chess[0] - 1] == 0 && A[0][chess[1]][chess[0] - 2] == 0) {
                out << chess[0] << " " << chess[1] << " ";
                out << chess[0] - 1 << " " << chess[1] << " ";
                out << chess[0] - 2 << " " << chess[1] << " ";
            }
        }
    }
#ifdef DEBUG
    cout << me;
    cout << other;
    cout << endl;
    for (i = 0; i < 2; i++) {
        cout << chess[i] << " ";
    }
    cout << endl;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            cout << M[i][j];
        }
    }
    cout << endl;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            cout << O[i][j];
        }
    }
    cout << endl;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            cout << A[0][i][j];
        }
        cout << endl;
    }
#endif // DEBUG
    out.close();
    return 0;
}
