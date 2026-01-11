#include <iostream>
#include <string>
#include <cstdlib>
#include "jsoncpp/json.h" // C++编译时默认包含此库 

using namespace std;

const int SIZE = 15;
int board[SIZE][SIZE]{};

bool ll(int x, int y) {
    int nums[4] = { 1,1,1,1 }, dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    for (int i = 0; i < 4; ++i) {
        int nowx = x, nowy = y;
        while (1) {
            nowx += dx[i];
            nowy += dy[i];
            if (nowx < 0 || nowx > 14 || nowy < 0 || nowy > 14 || board[nowx][nowy] != 1) { break; }
            else nums[i] += 1;
        }
        nowx = x, nowy = y;
        while (1) {
            nowx -= dx[i];
            nowy -= dy[i];
            if (nowx < 0 || nowx>14 || nowy < 0 || nowy>14 || board[nowx][nowy] != 1) { break; }
            else nums[i] += 1;
        }
    }
    for (int i = 0; i < 4; ++i) {
        if (nums[i] > 5) return true;
    }
    return false;
}

bool F44(int x, int y) {
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    int fourcount = 0;
    for (int b = 0; b < 4; ++b) {
        int before = 0, middle = 1, after = 0;
        bool flag = false;//flag标记是否有遇到空格
        //正方向
        for (int i = x - dx[b], j = y - dy[b];; i -= dx[b], j -= dy[b]) {
            if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == 2) {
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
            if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == 2) {
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
            if (before == 0 || after == 0) ++fourcount;
        }
        else {//冲四
            if (before > 0 && before + middle == 4) ++fourcount;
            if (after > 0 && after + middle == 4) ++fourcount;
        }
    }
    return fourcount > 1;
}

bool Judge(int x, int y) {
    return  F44(x, y) || ll(x, y);
}

bool T33(int x, int y) {
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    int Threecount = 0;
    for (int n = 0; n < 4; ++n) {
        int b = 0, bb = 0, m = 1, a = 0, aa = 0, flag = 0;
        for (int i = x - dx[n], j = y - dy[n];; i -= dx[n], j -= dy[n]) {
            if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == 2) {
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
        for (int i = x + dx[n], j = y + dy[n];; i += dx[n], j += dy[n]) {
            if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == 2) {
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
                if (!Judge(x + dx[n] * (4 - dm), y + dy[n] * (4 - dm))) {
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
            if (!(Judge(x + dx[n] * (4 - dm), y + dy[n] * (4 - dm)) && Judge(x - dx[n] * dm, y - dy[n] * dm))) {
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
                if (aa == -1) continue;
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

int calculate(int x, int y,int z) {
    int ans[3]{};//black和white的分数
    int dx[4] = { 1,0,1,1 }, dy[4] = { 0,1,1,-1 };//水平 竖直 y=x y=-x
    for (int m = 1; m <= 2; ++m) {//1为黑，2为白
        for (int b = 0; b < 4; ++b) {
            int before = 0, middle = 1, after = 0;
            bool flag = false;//flag标记是否有遇到空格
            //正方向
            for (int i = x - dx[b], j = y - dy[b];; i -= dx[b], j -= dy[b]) {
                if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == m) {
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
                if (i < 0 || i>14 || j < 0 || j>14 || board[i][j] == m) {
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
            if (middle >= 5) {
                ans[m] += 300000;
                if (m == z) ans[m] += 3000000;
            }
            else if (middle == 4) {
                if (before == 0 && after == 0) { ans[m] += 20000; }
                else if ((before >= 0 && after == -1) || (before == -1 && after >= 0)) { ans[m] += 1000; }
            }
            else if (middle == 3) {
                if (before > 0 && before + middle == 4) ans[m] += 10000;//冲四
                if (after > 0 && after + middle == 4) ans[m] += 10000;
                if (before == 0 && after == 0) ans[m] += 5000;//活3
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
    return ans[1] * 1.5 + ans[2];//黑棋重进攻，白棋重防守
}




int main()
{
	// 读入JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	int turn = input["requests"].size();
	Json::Value ret;
    int zero = 0;
	if (input["requests"][zero]["x"].asInt() == -1 && input["requests"][zero]["y"].asInt() == -1) {//执黑
		if (turn > 1) {
			for (int i = 0; i < turn; ++i) {
				board[input["requests"][i+1]["x"].asInt()][input["requests"][i+1]["y"].asInt()] = 2;
				board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
            int tx = 0, ty = 0, m = 0;
            for (int i = 0; i < 15; ++i) {
                for (int j = 0; j < 15; ++j) {
                    if (board[i][j] == 0 &&(!Judge(i, j) && !T33(i, j))) {
                        int t = calculate(i, j, 1);
                        if (t > m) {
                            m = t;
                            tx = i;
                            ty = j;
                        }
                    }
                }
            }
            Json::Value action;
            action["x"] = tx;
            action["y"] = ty;
            ret["response"] = action;
		}
		else {
			Json::Value action;
			action["x"] = 7;
			action["y"] = 7;
			ret["response"] = action;
		}
	}
	else {//执白
		for (int i = 0; i < turn; ++i) {
			board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 1;
			if(i>0)board[input["responses"][i-1]["x"].asInt()][input["responses"][i-1]["y"].asInt()] = 2;
		}
        int tx = 0, ty = 0, m = 0;
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if (board[i][j] == 0) {
                    int t = calculate(i, j, 2);
                    if (t > m) {
                        m = t;
                        tx = i;
                        ty = j;
                    }
                }
            }
        }
        Json::Value action;
        action["x"] = tx;
        action["y"] = ty;
        ret["response"] = action;
	}

	Json::FastWriter writer;
	std::cout << writer.write(ret) << endl;
	return 0;
}
