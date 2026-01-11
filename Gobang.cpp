#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<string>
#include<fstream>
#include<iostream>
#include<MMSystem.h>
#pragma comment(lib,"winmm.lib")

int board[16][16]{};//初始化棋盘
int pos[300][2]{};

class Menu
{
public:
	void ChessBoard(MOUSEMSG m);
    void Display();
};

class Play
{
public:
	void Two(MOUSEMSG m,int a);
	void Computer(MOUSEMSG m,int n,int a);
	void buttonRingth(MOUSEMSG m,int pl);//判断是否点击右侧工具栏
	void displayWin(int n1, int n2);//显示哪一方赢了,n1为0表示双人.为1表示人机，n2为0表示黑、人为1表示白、机
	int Win(int i,int j);
    bool T33(int x, int y);//33
    bool F44(int x, int y);//44
    bool ll(int x, int y);//长连
    bool Judge(int x, int y);
    bool AI(int n);
    int calculate(int x, int y,int z);
    void loadboard(MOUSEMSG m);
    void storeboard(int pl);//1000表示人机对战，1111表示双人对战
};

void Menu::Display() {
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
    rectangle(480, 60, 600, 120);
    settextcolor(BLACK);
    settextstyle(30, 0, "楷体");
    setbkmode(TRANSPARENT);
    outtextxy(480, 75, "存储棋盘");

    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
    rectangle(480, 140, 600, 200);
    settextcolor(BLACK);
    settextstyle(30, 0, "楷体");
    setbkmode(TRANSPARENT);
    outtextxy(480,155, "读取棋盘");

    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
    rectangle(480, 230, 600, 270);
    settextcolor(BLACK);
    settextstyle(30, 0, "楷体");
    setbkmode(TRANSPARENT);
    outtextxy(480, 235, "退出游戏");
}

void Play::storeboard(int pl) {
    std::ofstream outputFile("board.txt");
    if (outputFile.is_open()) {
        int i = 1;
        for (; i < 256; ++i) {
            if (pos[i][0] == 0&& pos[i + 1][0] == 0 && pos[i][1] == 0 && pos[i + 1][1] == 0 ) break;
            outputFile << pos[i][0] << " " << pos[i][1] << " ";
        }
        outputFile << std::endl;
        outputFile << pl;
    }
    outputFile.close();
}

void Play::loadboard(MOUSEMSG m) {
    memset(board, 0, sizeof(board));
    memset(pos, 0, sizeof(pos));
    initgraph(665, 490, SHOWCONSOLE);
    setbkcolor(RGB(245, 245, 220));
    cleardevice();
    for (int i = 20; i <= 470; i += 30)
        {
            setlinecolor(BLACK);  //设置线条颜色为黑色
            line(20, i, 470, i);
            line(i, 20, i, 470);
        }
    Menu().Display();
    std::ifstream inputFile("board.txt");
    if (inputFile.is_open()){
        int i = 0;
        for (; i < 256; ++i) {
            inputFile >> pos[i][0];
            if (pos[i][0] > 300) {
                if (pos[i][0] == 1000) {
                    Play::Computer(m, i % 2+1,i+1);
                }
                if (pos[i][0] == 1111) {
                    Play::Two(m,i+1);
                }
                break;
            }
            inputFile >> pos[i][1];
            int x = pos[i][0], y = pos[i][1];
            board[x][y] = i % 2 + 1;
            if (i % 2 == 1){//白棋
                int row = 20 + 30 * x, col = 20 + 30 * y;
                setfillcolor(BLACK);
                solidcircle(row, col, 12);
                setfillcolor(WHITE);
                solidcircle(row, col, 11);
                settextcolor(BLACK);
                settextstyle(15, 0, "黑体");
                setbkmode(TRANSPARENT);
                std::string numStr = std::to_string(i+1);
                outtextxy(row - 5, col - 7, numStr.c_str());
            }
            else {//黑棋
                int row = 20 + 30 * x, col = 20 + 30 * y;
                setfillcolor(BLACK);
                solidcircle(row, col, 12);
                settextcolor(WHITE);
                settextstyle(15, 0, "黑体");
                setbkmode(TRANSPARENT);
                std::string numStr = std::to_string(i+1);
                outtextxy(row - 5, col - 7, numStr.c_str());
            }
        }
    }

}

void Play::buttonRingth(MOUSEMSG m, int pl) {//实现存盘和读盘功能
        if (m.uMsg == WM_LBUTTONDOWN) {
            if (m.x > 480 && m.x < 600 && m.y>140 && m.y < 200) {
                setlinecolor(GREEN);
                setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
                rectangle(480, 140, 600, 200);
                Play().loadboard(m);
            }
            if (m.x > 480 && m.x < 600 && m.y>60 && m.y < 120) {
                setlinecolor(BLUE);
                setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
                rectangle(480, 60, 600, 120);
                Play().storeboard(pl);
            }
            if (m.x > 480 && m.x < 600 && m.y>230 && m.y < 270) {
                exit(0);
            }
        }
}

void Play::displayWin(int n1, int n2) {
    memset(board, 0, sizeof(board));
    settextcolor(RED);
    settextstyle(40, 0, "黑体");
    setbkmode(TRANSPARENT);
    if(n2==1)outtextxy(520, 190, "黑棋胜");
    else outtextxy(520, 190, "白棋胜");
    MOUSEMSG m;//鼠标操作
    while (1)
    {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN && m.x >= 215 && m.x <= 270 && m.y >= 285 && m.y <= 320)
        {
            setlinecolor(YELLOW);
            //空心矩形框
            rectangle(215, 285, 270, 320);
            Sleep(300);
            break;
        }
        else if (m.uMsg == WM_LBUTTONDOWN)
            exit(0);
    }
}

int Play::Win(int x,int y) {
    int nums[4] = { 1,1,1,1 }, dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    for (int i = 0; i < 4; ++i) {
        int nowx = x, nowy = y;
        while (1) {
            nowx += dx[i];
            nowy += dy[i];
            if (nowx < 0 || nowx > 15 || nowy < 0 || nowy > 15 || board[nowx][nowy] != board[x][y]) { break; }
            else nums[i] += 1;
        }
        nowx = x, nowy = y;
        while (1) {
            nowx -= dx[i];
            nowy -= dy[i];
            if (nowx < 0 || nowx>15 || nowy < 0 || nowy>15 || board[nowx][nowy] != board[x][y]) { break; }
            else nums[i] += 1;
        }
    }
    //是否连成五子
    if (board[x][y] == 2) {
        for (int i = 0; i < 4; ++i) {
            if (nums[i] >= 5) return 2;
        }
    }
    if (board[x][y] == 1) {//黑棋还要额外判断禁手
        for (int i = 0; i < 4; ++i) {
            if (nums[i] > 5) return 2;//若黑长连，白胜
        }
        for (int i = 0; i < 4; ++i) {
            if (nums[i] == 5) return 1;//若黑五连，黑胜
        }
        if (F44(x,y)||T33(x,y)) {//若触犯33、44禁手
            return 2;
        }
    }
    return 0;//棋局继续
}

bool Play::ll(int x, int y){
    int nums[4] = { 1,1,1,1 }, dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    for (int i = 0; i < 4; ++i) {
        int nowx = x, nowy = y;
        while (1) {
            nowx += dx[i];
            nowy += dy[i];
            if (nowx < 0 || nowx > 15 || nowy < 0 || nowy > 15 || board[nowx][nowy] != 1) { break; }
            else nums[i] += 1;
        }
        nowx = x, nowy = y;
        while (1) {
            nowx -= dx[i];
            nowy -= dy[i];
            if (nowx < 0 || nowx>15 || nowy < 0 || nowy>15 || board[nowx][nowy] != 1) { break; }
            else nums[i] += 1;
        }
    }
    for (int i = 0; i < 4; ++i) {
        if (nums[i] > 5) return true;
    }
    return false;
}

bool Play::Judge(int x, int y) {
    return  F44(x, y) || ll(x, y);
}

bool Play::F44(int x, int y) {
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    int fourcount = 0;
    for (int b = 0; b < 4; ++b) {
        int before = 0, middle = 1, after = 0;
        bool flag = false;//flag标记是否有遇到空格
        //正方向
        for (int i = x - dx[b], j = y - dy[b];; i -= dx[b], j -= dy[b]) {
            if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == 2) {
                if (!flag) before = -1;
                break;
            }
            else if (board[i][j] == 1) {
                if (flag) before += 1;
                else middle += 1;
            }
            else {
                if (flag) break;
                flag = true;
            }
        }
        //反方向
        for (int i = x + dx[b], j = y + dy[b];; i += dx[b], j += dy[b]) {
            if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == 2) {
                if (!flag) after = -1;
                break;
            }
            else if (board[i][j] == 1) {
                if (flag) after += 1;
                else middle += 1;
            }
            else {
                if (flag) break;
                flag = true;
            }
        }
        //判断该方向是否能形成冲四or活四
        if (middle == 4) {//活四
            if (before == 0||after==0) ++fourcount;
        }
        else {//冲四
            if (before > 0 && before + middle == 4) ++fourcount;
            if (after > 0 && after + middle == 4) ++fourcount;
        }
    }
    return fourcount > 1;
}

bool Play::T33(int x, int y) {
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    int Threecount = 0;
    for (int n = 0; n < 4; ++n) {
        int b = 0, bb = 0, m = 1, a = 0, aa = 0, flag = 0;
        for(int i = x - dx[n], j = y - dy[n];; i -= dx[n], j -= dy[n]){
            if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == 2) {
                if (flag == 0) b = -1;
                else if (flag == 1) bb = -1;
                break;
            }
            else if (board[i][j] == 1) {
                if (flag == 1) ++b;
                else if (flag == 2) ++bb;
                else ++m;
            }
            else {
                if (flag == 2) break;
                else ++flag;
            }
        }
        flag = 0;
        int dm = m;
        for(int i = x + dx[n], j = y + dy[n];; i += dx[n], j += dy[n]) {
            if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == 2) {
                if (flag == 0) a = -1;
                else if (flag == 1) aa = -1;
                break;
            }
            else if (board[i][j] == 1) {
                if (flag == 1) ++a;
                else if (flag == 2) ++aa;
                else ++m;
            }
            else {
                if (flag == 2) break;
                else ++flag;
            }
        }
        if (b == -1 || a == -1 || m > 3) continue;
        if (m == 3) {
            if (b != 0 || a != 0) continue;
            if (bb == -1 && aa == -1) continue;
            if (bb == -1) {
                if (aa > 0) continue;
                if (!Judge(x+dx[n]*(4-dm),y+dy[n]*(4-dm))){
                    ++Threecount; continue;
                }
            }
            if (aa == -1) {
                if (bb > 0) continue;
                if (!Judge(x - dx[n] * (4 - dm), y - dy[n] * (4 - dm))) {
                    ++Threecount; continue;
                }
            }
            if (aa > 0 && bb > 0) continue;
            if(!(Judge(x + dx[n] * (4 - dm), y + dy[n] * (4 - dm))&&Judge(x-dx[n]*dm,y-dy[n]*dm))){
                ++Threecount; continue;
            }
        }
        if (m == 2) {
            if (b == 1 && a == 0) {
                if (bb == -1) continue;
                if (!Judge(x - dx[n] * dm, y - dy[n] * dm)) {
                    ++Threecount; continue;
                }
            }
            if (b == 0 && a == 1) {
                if(aa == -1) continue;
                if (!Judge(x + dx[n] * (3 - dm), y - dy[n] * (3 - dm))) {
                    ++Threecount; continue;
                }
            }
        }
        if (m == 1) {
            if (b == 2 && a == 0) {
                if (bb == -1) continue;
                if (!Judge(x - dx[n], y - dy[n])) {
                    ++Threecount; continue;
                }
            }
            if (a == 2 && b == 0) {
                if (aa == -1) continue;
                if (!Judge(x + dx[n], y + dy[n])) {
                    ++Threecount; continue;
                }
            }
        }
    }
    return Threecount > 1;
}

int Play::calculate(int x, int y,int z) {
    int ans[3]{};//black和white的分数
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    for (int m = 1; m <= 2; ++m) {//1为黑，2为白
        for (int b = 0; b < 4; ++b) {
            int before = 0, middle = 1, after = 0;
            bool flag = false;//flag标记是否有遇到空格
            //正方向
            for (int i = x - dx[b], j = y - dy[b];; i -= dx[b], j -= dy[b]) {
                if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == m) {
                    if (!flag) before = -1;
                    break;
                }
                else if (board[i][j] == 3 - m) {
                    if (flag) before += 1;
                    else middle += 1;
                }
                else {
                    if (flag) break;
                    flag = true;
                }
            }
            //反方向
            flag = false;
            for (int i = x + dx[b], j = y + dy[b];; i += dx[b], j += dy[b]) {
                if (i < 0 || i>15 || j < 0 || j>15 || board[i][j] == m) {
                    if (!flag) after = -1;
                    break;
                }
                else if (board[i][j] == 3 - m) {
                    if (flag) after += 1;
                    else middle += 1;
                }
                else {
                    if (flag) break;
                    flag = true;
                }
            }
            //计算每种棋形的分数
            //const judge_standard = {
            //    //连五
            //    FIVE: 300000,
            //    //活四
            //    FOUR : 10000,
            //    //活三
            //    THREE : 1000,
            //    //活二
            //    TWO : 100,
            //    //活一
            //    ONE : 10,
            //    //眠四
            //    BLOCK_FOUR : 1000,
            //    //眠三
            //    BLOCK_THREE : 100,
            //    //眠二
            //    BLOCK_TWO : 10,
            //    //眠一
            //    BLOCK_ONE : 1
            //};
            if (middle == 5) { ans[m] += 300000; }
            else if (middle == 4) {
                if (before == 0 && after == 0) { ans[m] += 20000; }
                else if ((before >= 0 && after == -1) || (before == -1 && after >= 0)) { ans[m] += 1000; }
            }
            else if (middle == 3) {
                if (before > 0 && before + middle == 4) ans[m] += 10000;//冲四
                if (after > 0 && after + middle == 4) ans[m] += 10000;
                if (before == 0 && after == 0) ans[m] += 1000;//活3
                if ((before >= 0 && after == -1) || (before == -1 && after >= 0)) ans[m] += 100;
            }
            else if (middle == 2) {
                if (before > 0 && before + middle == 3) ans[m] += 500;//冲三
                if (after > 0 && after + middle == 3) ans[m] += 500;
                if (before == 0 && after == 0) ans[m] += 100;
                if ((before >= 0 && after == -1) || (before == -1 && after >= 0)) ans[m] += 10;
            }
            else {
                if (before > 0 && before + middle == 4) ans[m] += 2000;//冲四
                if (after > 0 && after + middle == 4) ans[m] += 2000;
                if (before > 0 && before + middle == 3) ans[m] += 500;//冲三
                if (after > 0 && after + middle == 3) ans[m] += 500;
                if (before == 0 && after == 0) ans[m] += 10;
                if ((before >= 0 && after == -1) || (before == -1 && after >= 0)) ans[m] += 1;
            }
        }
    }
    if (z % 2 == 0) ans[1] = ans[1] * 1.2;
    else ans[2] = ans[2] * 1.2;
    return ans[1]+ans[2];
}

bool Play::AI(int n) {
    //n为奇数 表示AI执黑棋；n为偶数 表示AI执白棋
    bool flag;
    if (n % 2 == 1) flag = false;
    else flag = true;
    int m = -400000,temp = 0,tx = 0,ty = 0;
    for (int i = 0; i <= 15; ++i) {
        for (int j = 0; j <= 15; ++j) {
            if (board[i][j] == 0) {
                if (flag || !Judge(i, j)) {
                    temp = calculate(i, j, n);
                    if (temp > m) {
                        m = temp;
                        tx = i;
                        ty = j;
                    }
                }
            }
        }
    }
    if (flag) {//白棋
        int row = 20 + 30 * tx, col = 20 + 30 * ty;
        setfillcolor(BLACK);
        solidcircle(row, col, 12);
        setfillcolor(WHITE);
        solidcircle(row, col, 11);
        settextcolor(BLACK);
        settextstyle(15, 0, "黑体");
        setbkmode(TRANSPARENT);
        std::string numStr = std::to_string(n);
        outtextxy(row - 5, col - 7, numStr.c_str());
        board[tx][ty] = 2;
    }
    else{
        int row = 20 + 30 * tx, col = 20 + 30 * ty;
        setfillcolor(BLACK);
        solidcircle(row, col, 12);
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(15, 0, "黑体");
        setbkmode(TRANSPARENT);
        std::string numStr = std::to_string(n);
        outtextxy(row - 5, col - 7, numStr.c_str());
        board[tx][ty] = 1;
    }
    pos[n][0] = tx;
    pos[n][1] = ty;
    return Win(tx, ty)==0;
}

void Menu::ChessBoard(MOUSEMSG m)
{
    initgraph(665, 490, SHOWCONSOLE);

    setbkcolor(RGB(245, 245, 220));
    cleardevice();
    while (true)
    {
        m = GetMouseMsg();
        for (int i = 20; i <= 470; i += 30)
        {
            setlinecolor(BLACK);  //设置线条颜色为黑色
            line(20, i, 470, i);
            line(i, 20, i, 470);
        }
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
        rectangle(480, 280, 600, 340);
        settextcolor(BLACK);
        settextstyle(30, 0, "楷体");
        setbkmode(TRANSPARENT);
        outtextxy(480,295, "双人对战");
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
        rectangle(480, 360, 600, 420);
        settextcolor(BLACK);
        settextstyle(30, 0, "楷体");
        setbkmode(TRANSPARENT);
        outtextxy(480,375, "人机对战");
        setbkmode(TRANSPARENT);
        rectangle(480, 420, 540, 450);
        rectangle(540, 420, 600, 450);
        outtextxy(480, 420, "执黑");
        outtextxy(540, 420, "执白");
        if (m.uMsg == WM_LBUTTONDOWN && (m.x >= 480 && m.x <= 600 && m.y >= 280 && m.y <= 340
            || m.x >= 480 && m.x <= 600 && m.y >= 360 && m.y <= 450))
        {
            if (m.x >= 480 && m.x <= 600 && m.y >= 280 && m.y <= 340)
            {
                setlinecolor(RED);
                setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
                rectangle(480, 280, 600, 340);
            }
            else if (m.x >= 480 && m.x <= 540 && m.y >= 360 && m.y <= 450)
            {
                setlinecolor(RED);
                rectangle(480, 420, 540, 450);
            }
            else if (m.x >= 540 && m.x <= 600 && m.y >= 360 && m.y <= 450) {
                setlinecolor(RED);
                rectangle(540, 420, 600, 450);
            }
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                if (m.x >= 480 && m.x <= 600 && m.y >= 280 && m.y <= 340)
                {
                    Display();
                    Play().Two(m,1);
                }
                else if(m.x >= 480 && m.x <= 540 && m.y >= 360 && m.y <= 450)
                {
                    Display();
                    Play().Computer(m, 1,1);
                }
                else if(m.x >= 540 && m.x <= 600 && m.y >= 360 && m.y <= 450)
                    Display();
                    Play().Computer(m, 2,1);
                }
            }
        }
    }

void Play::Two(MOUSEMSG m,int a) {
    int gameing = 0, pl = 1, num = a;
    MOUSEMSG ms;
    while (num<=256) {
        ms = GetMouseMsg();
        int i, j;
        bool flag = 0;
        if (ms.uMsg == WM_LBUTTONDOWN) {
            buttonRingth(ms, 1111);
            for (int col = 20; col <= 470; col += 30)
            {
                if (ms.y <= col + 15 && ms.y >= col - 15)
                {
                    for (int row = 20; row <= 470; row += 30)
                    {
                        if (ms.x <= row + 15 && ms.x >= row - 15)
                        {
                            if (pl == 1 && board[row / 30 ][col / 30 ] == 0)
                            {
                                flag = 1;
                                setfillcolor(BLACK);
                                solidcircle(row, col, 12);
                                settextcolor(WHITE);
                                settextstyle(15, 0, "黑体");
                                setbkmode(TRANSPARENT);
                                std::string numStr = std::to_string(num);
                                outtextxy(row-5, col-7, numStr.c_str());
                                i = row / 30 ;
                                j = col / 30 ;
                                board[i][j] = 1;
                                pos[num][0] = i;
                                pos[num][1] = j;
                                pl = 0;
                                ++num;
                                break;
                            }
                            if (pl == 0 && board[row / 30 ][col / 30 ] == 0)
                            {
                                flag = 1;
                                setfillcolor(BLACK);
                                solidcircle(row, col, 12);
                                setfillcolor(WHITE);
                                solidcircle(row, col, 11);
                                settextcolor(BLACK);
                                settextstyle(15, 0, "黑体");
                                setbkmode(TRANSPARENT);
                                std::string numStr = std::to_string(num);
                                outtextxy(row-5, col-7, numStr.c_str());
                                i = row / 30 ;
                                j = col / 30 ;
                                board[i][j] = 2;
                                pos[num][0] = i;
                                pos[num][1] = j;
                                pl = 1;
                                ++num;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (flag) {
            gameing = Win(i, j);

            if (gameing != 0) {
                Sleep(100);
                break;
            }
        }
        
    }
    displayWin(0, gameing);
}

void Play::Computer(MOUSEMSG m,int n,int a) {
    int game = 0, pl = n, num = a;//pl为1表示玩家执黑棋，2表示执白棋
    if (pl == 2&&num==1) {//若AI执黑，在中间落子
        setfillcolor(BLACK);
        solidcircle(260, 260, 12);
        settextcolor(WHITE);
        settextstyle(15, 0, "黑体");
        setbkmode(TRANSPARENT);
        std::string numStr = std::to_string(num);
        outtextxy(255, 253, numStr.c_str());
        pos[1][0] = 8;
        pos[1][1] = 8;
        board[8][8] = 1;
        ++num;
    }
    while (num<=256) {
        MOUSEMSG ms = GetMouseMsg();
        int i, j;
        bool flag = 0;
        if (ms.uMsg == WM_LBUTTONDOWN) {
            buttonRingth(ms, 1000);
            for (int col = 20; col <= 470; col += 30)
            {
                if (ms.y <= col + 15 && ms.y >= col - 15)
                {
                    for (int row = 20; row <= 470; row += 30)
                    {
                        if (ms.x <= row + 15 && ms.x >= row - 15)
                        {
                            if (pl == 1 && board[row / 30][col / 30] == 0)
                            {
                                flag = 1;
                                setfillcolor(BLACK);
                                solidcircle(row, col, 12);
                                settextcolor(WHITE);
                                settextstyle(15, 0, "黑体");
                                setbkmode(TRANSPARENT);
                                std::string numStr = std::to_string(num);
                                outtextxy(row - 5, col - 7, numStr.c_str());
                                i = row / 30;
                                j = col / 30;
                                board[i][j] = 1;
                                pos[num][0] = i;
                                pos[num][1] = j;
                                ++num;
                                break;
                            }
                            if (pl == 2 && board[row / 30][col / 30] == 0)
                            {
                                flag = 1;
                                setfillcolor(BLACK);
                                solidcircle(row, col, 12);
                                setfillcolor(WHITE);
                                solidcircle(row, col, 11);
                                settextcolor(BLACK);
                                settextstyle(15, 0, "黑体");
                                setbkmode(TRANSPARENT);
                                std::string numStr = std::to_string(num);
                                outtextxy(row - 5, col - 7, numStr.c_str());
                                i = row / 30;
                                j = col / 30;
                                board[i][j] = 2;
                                pos[num][0] = i;
                                pos[num][1] = j;
                                ++num;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (flag) {
            game = Win(i, j);
            if (game != 0) {
                displayWin(1, game); break;
            }
            if (AI(num)) {
                ++num;
            }
            else {
                displayWin(1, 3 - pl); break;
            }
        }
    }
}

int main()
{
    Menu menu;
    initgraph(416, 624, SHOWCONSOLE);
    MOUSEMSG m;
    //mciSendString("open bgm.mp3 type mpegvideo", NULL, 0, 0);
    //mciSendString("play bgm.mp3 repeat", NULL, 0, 0);
    while (true) {
        m = GetMouseMsg();
        Menu().ChessBoard(m);
        break;
    }
    closegraph();
    return 0;
}
