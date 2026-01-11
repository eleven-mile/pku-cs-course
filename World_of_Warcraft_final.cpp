#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <vector>
#include <string>
#include <typeinfo>
using namespace std;
class warrior;
class dragon;
class ninja;
class iceman;
class lion;
class wolf;

class weapon;
class sword;
class bomb;
class arrow;

class head;
class city;
// enum warriors {dragon,ninja,iceman,lion,wolf};
// enum weapons {sword,bomb,arrow};
int M,N,R,K,T;
int hp[5],atk[5];//{dragon,ninja,iceman,lion,wolf}
int mn;
int seq_red[5]{2,3,4,1,0},seq_blue[5]{3,0,1,2,4};
string war_mp[5]{"dragon","ninja","iceman","lion","wolf"};
string wea_mp[3]{"sword","bomb","arrow"};
class city{
    public:
    vector<warrior*> red_warriors;
    vector<warrior*> blue_warriors;
    vector<int> city_elements;
    head *red,*blue;
    vector<int>flag;//表示城市中叉的旗帜,1表示红方有旗帜，2表示蓝方有旗帜，0表示无旗帜
    vector<int>win_cnt;
    int num_red,num_blue;//表示红方/蓝方司令部中对方士兵的数量

    city(int n): red_warriors(n+2), blue_warriors(n+2), city_elements(n+2, 0), num_red(0), num_blue(0), flag(n+2, 0),win_cnt(n+2,0) {};
    void add_warrior(int idx, warrior* war);
    void add_head(head* r, head* b) {
        red = r;
        blue = b;
    }
    void lion_runaway(int hour, int minute);
    int move(int hour, int minute);
    void arrow_attack(int hour, int minute);
    void bomb_attack(int hour, int minute);
    void city_fight(int hour,int minute);
    void produce_elements(int hour,int minute);
    void take_elements(int hour,int minute);
    void announce_weapon(int hour,int minute);
    void print_get_elements(int idx,bool colour,int hour,int minute);
};
class weapon{
    public:
    int type;
    warrior* owner;

    weapon(int ty, warrior* own): type(ty), owner(own) {};
    virtual void update(){};
};
class warrior{
    public:
    int type, cur_hp, num, pos,cur_atk;
    head* gov;
    bool alive;

    warrior(int ty, int he, int at, int n,int p, head* g): type(ty), cur_hp(he), cur_atk(at), num(n), gov(g), pos(p),alive(true) {};
    bool is_alive() const { return alive && cur_hp > 0; }
    virtual void fight(warrior* enemy,bool f);//f表示是进攻还是反击
    // virtual void counterattack(warrior* enemy);
    virtual void move_update(){};
    virtual weapon* get_arrow(){return nullptr;};
    virtual weapon* get_bomb(){return nullptr;};
    virtual weapon* get_sword(){return nullptr;};
};
class sword : public weapon {
    public:
    int attack;
    sword(int ty, warrior* own,int atk_now): weapon(ty, own) ,attack(atk_now/5){};
    void update() override {
        attack = attack * 8 / 10;
    }
};
class bomb : public weapon{
    public:
    bomb(int ty,warrior* own):weapon(ty,own){};
};
class arrow :public weapon{
    public:
    int cnt;
    arrow(int ty,warrior* own):weapon(ty,own),cnt(3){};
    void update() override {
        cnt--;
    }
};
class head{
    public:
    int elements,idx,num;
    vector<warrior*> army[5];
    city* c;
    string colour;
    int *seq;

    head(int v,string s,city* ci):colour(s),elements(v),idx(0),num(1),c(ci){
        seq = (colour == "red") ? seq_red : seq_blue;
    }
    void produce(int hour,int minite);
    void announce_elements(int hour,int minite){
        cout << setw(3) << setfill('0') << hour << ":" 
        << setw(2) << setfill('0') << minite << " " 
        << elements << " elements in " << colour <<" headquarter" << endl;
    }
};

class dragon : public warrior {
    public:
    weapon* wea;
    double morale;
    dragon(int ty, int he, int at, int n, int p, head* g): warrior(ty, he, at, n, p, g) {
        morale = 1.0 * gov->elements / he;
        int wea_num = n % 3;
        switch(wea_num){
            case 0 : wea = new sword(0, this,at); break;
            case 1 : wea = new bomb(1, this); break;
            case 2 : wea = new arrow(2, this); break;
            default: wea = nullptr; break;
        }
        cout << "Its morale is " << fixed << setprecision(2) << morale << endl;
    }
    weapon* get_arrow() override {
        if(wea == nullptr) return {nullptr};
        return wea->type == 2 ? wea : nullptr;
    }
    weapon* get_bomb() override {
        if(wea == nullptr) return {nullptr};
        return wea->type == 1 ? wea : nullptr;
    }
    weapon* get_sword() override {
        if(wea == nullptr) return {nullptr};
        return wea->type == 0 ? wea : nullptr;
    }
};
class ninja : public warrior {
public:
    weapon *wea1, *wea2;
    ninja(int ty, int he, int at, int n, int p, head* g): warrior(ty, he, at, n, p, g) {
        switch(n % 3){
            case 0 : wea1 = new sword(0, this,at); break;
            case 1 : wea1 = new bomb(1, this); break;
            case 2 : wea1 = new arrow(2, this); break;
            default: wea1 = nullptr; break;
        }
        switch((n + 1) % 3){
            case 0 : wea2 = new sword(0, this,at); break;
            case 1 : wea2 = new bomb(1, this); break;
            case 2 : wea2 = new arrow(2, this); break;
            default: wea2 = nullptr; break;
        }
    }
    weapon* get_arrow() override {
        if(wea1 && wea1->type == 2) return wea1;
        if(wea2 && wea2->type == 2) return wea2;
        return nullptr;
    }
    weapon* get_bomb() override {
        if(wea1 && wea1->type == 1) return wea1;
        if(wea2 && wea2->type == 1) return wea2;
        return nullptr;
    }
    weapon* get_sword() override {
        if(wea1 && wea1->type == 0) return wea1;
        if(wea2 && wea2->type == 0) return wea2;
        return nullptr;
    }
};

class lion : public warrior {
public:
    weapon *wea;
    int loyalty;
    lion(int ty, int he, int at, int n, int p, head* g): warrior(ty, he, at, n, p, g) {
        loyalty = g->elements;
        cout << "Its loyalty is " << loyalty << endl;
        wea = nullptr; // 如果你后续要加武器，记得初始化
    }
    weapon* get_arrow() override {
        if(wea && wea->type == 2) return wea;
        return nullptr;
    }
    weapon* get_bomb() override {
        if(wea && wea->type == 1) return wea;
        return nullptr;
    }
    weapon* get_sword() override {
        if(wea && wea->type == 0) return wea;
        return nullptr;
    }
};

class wolf : public warrior {
public:
    weapon *a,*b,*s;
    wolf(int ty, int he, int at, int n, int p, head* g): warrior(ty, he, at, n, p, g) ,a(nullptr),b(nullptr),s(nullptr){}
    weapon* get_arrow() override {
        if(a && a->type == 2) return a;
        return nullptr;
    }
    weapon* get_bomb() override {
        if(b && b->type == 1) return b;
        return nullptr;
    }
    weapon* get_sword() override {
        if(s && s->type == 0) return s;
        return nullptr;
    }
};

class iceman : public warrior {
public:
    weapon *wea;
    int cnt;
    iceman(int ty, int he, int at, int n, int p, head* g): warrior(ty, he, at, n, p, g) ,cnt(0){
        switch(n % 3){
            case 0 : wea = new sword(0, this,at); break;
            case 1 : wea = new bomb(1, this); break;
            case 2 : wea = new arrow(2, this); break;
            default: wea = nullptr; break;
        }
    }
    void move_update() override {
        if(cnt == 0) cnt++;
        else {
            cnt = 0;
            cur_hp = max(1,cur_hp - 9);
            cur_atk += 20;
        }
    }
    weapon* get_arrow() override {
        if(wea && wea->type == 2) return wea;
        return nullptr;
    }
    weapon* get_bomb() override {
        if(wea && wea->type == 1) return wea;
        return nullptr;
    }
    weapon* get_sword() override {
        if(wea && wea->type == 0) return wea;
        return nullptr;
    }
};
void head::produce(int hour,int minite){
    if(elements < hp[seq[idx]]) return;
    // while(hp[seq[idx]] > elements) idx = (idx + 1) % 5;
    int new_type = seq[idx];
    cout << setw(3) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minite 
    << " " << colour << " " << war_mp[new_type] << " " <<num << " born" << endl;
    // cout << elements << endl;
    int city_idx = (colour == "red") ? 0 : N + 1;
    elements -= hp[new_type];
    switch(new_type){
        case 0:{
            army[0].push_back(new dragon(new_type,hp[new_type],atk[new_type],num,city_idx,this));
            break;
        }
        case 1:{
            army[1].push_back(new ninja(new_type,hp[new_type],atk[new_type],num,city_idx,this));
            break;
        }
        case 2:{
            army[2].push_back(new iceman(new_type,hp[new_type],atk[new_type],num,city_idx,this));
            break;
        }
        case 3:{
            army[3].push_back(new lion(new_type,hp[new_type],atk[new_type],num,city_idx,this));
            break;
        }
        case 4:{
            army[4].push_back(new wolf(new_type,hp[new_type],atk[new_type],num,city_idx,this));
            break;
        }
    }
    c->add_warrior(city_idx, army[new_type].back());
    
    idx = (idx + 1) % 5;
    num++;
}
void city::add_warrior(int idx, warrior* war) {
    if (war->gov->colour == "red") {
        red_warriors[idx] = war;
    } else {
        blue_warriors[idx] = war;
    }
}
void city::lion_runaway(int hour, int minite){
    for(int i=0;i<N+2;++i){
        //红方狮子
        if(i != N + 1 && red_warriors[i] != nullptr && red_warriors[i]->type == 3){
            lion *li = dynamic_cast<lion*>(red_warriors[i]);
            if(li && li->loyalty <= 0){
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << li->gov->colour << " lion " << li->num << " ran away" << endl;
                red_warriors[i] = nullptr;
            }
        }
        //蓝方狮子
        if(i != 0 && blue_warriors[i] != nullptr && blue_warriors[i]->type == 3){
            lion *li = dynamic_cast<lion*>(blue_warriors[i]);
            if(li && li->loyalty <= 0){
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << li->gov->colour << " lion " << li->num << " ran away" << endl;
                blue_warriors[i] = nullptr;
            }
        }
    }
}
int city::move(int hour, int minite){
    int res = 0;
    for(int i=0;i<=N+1;++i){
        //红方士兵走
        if(i > 0 && red_warriors[i-1] != nullptr){
            warrior* war = red_warriors[i-1];
            // war->pos = i;
            // red_warriors[i] = war;
            // red_warriors[i-1] = nullptr;
            if(war->type == 2){
                war->move_update();
            }
            if(i != N + 1){
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << war->gov->colour << " "<< war_mp[war->type] << " " 
                << war->num << " marched to city " << i << " with " 
                << war->cur_hp << " elements and force "<< war->cur_atk << endl;
            }
            else if(i == N + 1){//红士兵到蓝方司令部
                num_blue++;
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << war->gov->colour << " "<< war_mp[war->type] << " " 
                << war->num << " reached blue headquarter with " 
                << war->cur_hp << " elements and force "<< war->cur_atk << endl;
                if(num_blue == 2){
                    cout << setw(3) << setfill('0') << hour << ":" 
                    << setw(2) << setfill('0') << minite << " blue headquarter was taken" <<endl;
                    res = 1;
                }
            }
        }
        //蓝方士兵走
        if(i < N + 1 && blue_warriors[i+1] != nullptr){
            warrior* war = blue_warriors[i+1];
            // war->pos = i;
            // blue_warriors[i] = war;
            // blue_warriors[i+1] = nullptr;
            if(war->type == 2){
                war->move_update();
            }
            if(i != 0){
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << war->gov->colour << " "<< war_mp[war->type] << " " 
                << war->num << " marched to city " << i << " with " 
                << war->cur_hp << " elements and force "<< war->cur_atk << endl;
                if(i == 0) num_red++;
            }
            else if(i == 0){
                num_red++;
                cout << setw(3) << setfill('0') << hour << ":" 
                << setw(2) << setfill('0') << minite << " " 
                << war->gov->colour << " "<< war_mp[war->type] << " " 
                << war->num << " reached red headquarter with " 
                << war->cur_hp << " elements and force "<< war->cur_atk << endl;
                if(num_red == 2){
                    cout << setw(3) << setfill('0') << hour << ":" 
                    << setw(2) << setfill('0') << minite << " red headquarter was taken" <<endl;
                    res = 2;
                }
            }
        }
    }
    //红方士兵实际走
    for(int i=N+1;i>=1;--i){
        if(red_warriors[i-1] != nullptr){
            auto war = red_warriors[i-1];
            war->pos = i;
            red_warriors[i] = war;
            red_warriors[i-1] = nullptr;
        }
    }
    //蓝方士兵实际走
    for(int i=0;i<=N;++i){
        if(blue_warriors[i+1] != nullptr){
            auto war = blue_warriors[i+1];
            war->pos = i;
            blue_warriors[i] = war;
            blue_warriors[i+1] = nullptr;
        }
    }

    return res;
}
void city::arrow_attack(int hour,int minite){
    for(int i=0;i<=N+1;++i){
        // 红方士兵 对i+1处放箭
        if(i < N + 1 && red_warriors[i] != nullptr && blue_warriors[i+1] != nullptr){
            auto cur_weapon = red_warriors[i]->get_arrow();
            if(cur_weapon != nullptr){
                cout << setw(3) << setfill('0') << hour << ":" 
                     << setw(2) << setfill('0') << minite << " " 
                     << red_warriors[i]->gov->colour << " " 
                     << war_mp[red_warriors[i]->type] << " " 
                     << red_warriors[i]->num << " shot";
                cur_weapon->update();
                if(cur_weapon->type == 2) {
                    arrow* ar = dynamic_cast<arrow*>(cur_weapon);
                    if(ar && ar->cnt <= 0){
                        // 删除箭时同步置空武士的箭指针
                        if(red_warriors[i]->type == 0) { // dragon
                            dragon* d = dynamic_cast<dragon*>(red_warriors[i]);
                            if(d && d->wea == cur_weapon) d->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 2) { // iceman
                            iceman* ic = dynamic_cast<iceman*>(red_warriors[i]);
                            if(ic && ic->get_arrow() == cur_weapon) ic->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 1) { // ninja
                            ninja* n = dynamic_cast<ninja*>(red_warriors[i]);
                            if(n) {
                                if(n->wea1 == cur_weapon) n->wea1 = nullptr;
                                if(n->wea2 == cur_weapon) n->wea2 = nullptr;
                            }
                        }
                        if(red_warriors[i]->type == 4) { // wolf
                            wolf* w = dynamic_cast<wolf*>(red_warriors[i]);
                            if(w && w->a == cur_weapon) w->a = nullptr;
                        }
                        delete cur_weapon;
                        cur_weapon = nullptr;
                    }
                }
                blue_warriors[i+1]->cur_hp -= R;
                if(blue_warriors[i+1]->cur_hp <= 0){
                    blue_warriors[i+1]->alive = false;
                    cout << " and killed " 
                         << blue_warriors[i+1]->gov->colour << " "
                         << war_mp[blue_warriors[i+1]->type] << " "
                         << blue_warriors[i+1]->num;
                }
                cout << endl;
            }
        }
        // 蓝方士兵 对i-1处放箭
        if(i > 0 && blue_warriors[i] != nullptr && red_warriors[i-1] != nullptr){
            auto cur_weapon = blue_warriors[i]->get_arrow();
            if(cur_weapon != nullptr){
                cout << setw(3) << setfill('0') << hour << ":" 
                     << setw(2) << setfill('0') << minite << " " 
                     << blue_warriors[i]->gov->colour << " " 
                     << war_mp[blue_warriors[i]->type] << " " 
                     << blue_warriors[i]->num << " shot";
                cur_weapon->update();
                if(cur_weapon->type == 2) {
                    arrow* ar = dynamic_cast<arrow*>(cur_weapon);
                    if(ar && ar->cnt <= 0){
                        if(blue_warriors[i]->type == 0) { // dragon
                            dragon* d = dynamic_cast<dragon*>(blue_warriors[i]);
                            if(d && d->wea == cur_weapon) d->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 2) { // iceman
                            iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]);
                            if(ic && ic->get_arrow() == cur_weapon) ic->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 1) { // ninja
                            ninja* n = dynamic_cast<ninja*>(blue_warriors[i]);
                            if(n) {
                                if(n->wea1 == cur_weapon) n->wea1 = nullptr;
                                if(n->wea2 == cur_weapon) n->wea2 = nullptr;
                            }
                        }
                        if(blue_warriors[i]->type == 4) { // wolf
                            wolf* w = dynamic_cast<wolf*>(blue_warriors[i]);
                            if(w && w->a == cur_weapon) w->a = nullptr;
                        }
                        delete cur_weapon;
                        cur_weapon = nullptr;
                    }
                }
                red_warriors[i-1]->cur_hp -= R;
                if(red_warriors[i-1]->cur_hp <= 0){
                    red_warriors[i-1]->alive = false;
                    cout << " and killed " 
                         << red_warriors[i-1]->gov->colour << " "
                         << war_mp[red_warriors[i-1]->type] << " "
                         << red_warriors[i-1]->num;
                }
                cout << endl;
            }
        }
    }
}
void city::bomb_attack(int hour,int minite){
    for(int i=1;i<=N;++i){
        if(red_warriors[i] == nullptr || blue_warriors[i] == nullptr) continue;
        if(!red_warriors[i]->is_alive() || !blue_warriors[i]->is_alive()) continue;

        // 红方自爆判定
        bomb* red_bomb = dynamic_cast<bomb*>(red_warriors[i]->get_bomb());
        if(red_bomb) {
            // 1. 红方主动进攻，如果反击会死，红方自爆
            bool red_first = (flag[i] == 1 || (flag[i] == 0 && i % 2 == 1));
            if(red_first) {
                int red_atk = red_warriors[i]->cur_atk;
                sword* red_sword = dynamic_cast<sword*>(red_warriors[i]->get_sword());
                if(red_sword) red_atk += red_sword->attack;
                int blue_hp = blue_warriors[i]->cur_hp - red_atk;
                if(blue_hp > 0 && blue_warriors[i]->type != 1) { // blue可反击
                    int blue_atk = blue_warriors[i]->cur_atk / 2;
                    sword* blue_sword = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
                    if(blue_sword) blue_atk += blue_sword->attack;
                    int red_hp = red_warriors[i]->cur_hp - blue_atk;
                    if(red_hp <= 0) {
                        // 红方自爆
                        red_warriors[i]->alive = false;
                        blue_warriors[i]->alive = false;
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minite << " "
                             << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
                             << red_warriors[i]->num << " used a bomb and killed " << blue_warriors[i]->gov->colour << " "
                             << war_mp[blue_warriors[i]->type] << " " << blue_warriors[i]->num << endl;
                        // 置空红方bomb指针
                        if(red_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(red_warriors[i]); if(d && d->wea == red_bomb) d->wea = nullptr; }
                        if(red_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(red_warriors[i]); if(ic && ic->wea == red_bomb) ic->wea = nullptr; }
                        if(red_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(red_warriors[i]); if(n) { if(n->wea1 == red_bomb) n->wea1 = nullptr; if(n->wea2 == red_bomb) n->wea2 = nullptr; } }
                        if(red_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(red_warriors[i]); if(w && w->b == red_bomb) w->b = nullptr; }
                        delete red_bomb;
                        continue;
                    }
                }
            }
            // 2. 红方被动防守，如果被蓝方主动攻击会死，红方自爆
            if(!red_first) {
                int blue_atk = blue_warriors[i]->cur_atk;
                sword* blue_sword = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
                if(blue_sword) blue_atk += blue_sword->attack;
                int red_hp = red_warriors[i]->cur_hp - blue_atk;
                if(red_hp <= 0) {
                    red_warriors[i]->alive = false;
                    blue_warriors[i]->alive = false;
                    cout << setw(3) << setfill('0') << hour << ":"
                         << setw(2) << setfill('0') << minite << " "
                         << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
                         << red_warriors[i]->num << " used a bomb and killed " << blue_warriors[i]->gov->colour << " "
                         << war_mp[blue_warriors[i]->type] << " " << blue_warriors[i]->num << endl;
                    if(red_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(red_warriors[i]); if(d && d->wea == red_bomb) d->wea = nullptr; }
                    if(red_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(red_warriors[i]); if(ic && ic->wea == red_bomb) ic->wea = nullptr; }
                    if(red_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(red_warriors[i]); if(n) { if(n->wea1 == red_bomb) n->wea1 = nullptr; if(n->wea2 == red_bomb) n->wea2 = nullptr; } }
                    if(red_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(red_warriors[i]); if(w && w->b == red_bomb) w->b = nullptr; }
                    delete red_bomb;
                    continue;
                }
            }
        }

        // 蓝方自爆判定
        bomb* blue_bomb = dynamic_cast<bomb*>(blue_warriors[i]->get_bomb());
        if(blue_bomb) {
            // 1. 蓝方主动进攻，如果反击会死，蓝方自爆
            bool blue_first = (flag[i] == 2 || (flag[i] == 0 && i % 2 == 0));
            if(blue_first) {
                int blue_atk = blue_warriors[i]->cur_atk;
                sword* blue_sword = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
                if(blue_sword) blue_atk += blue_sword->attack;
                int red_hp = red_warriors[i]->cur_hp - blue_atk;
                if(red_hp > 0 && red_warriors[i]->type != 1) { // red可反击
                    int red_atk = red_warriors[i]->cur_atk / 2;
                    sword* red_sword = dynamic_cast<sword*>(red_warriors[i]->get_sword());
                    if(red_sword) red_atk += red_sword->attack;
                    int blue_hp = blue_warriors[i]->cur_hp - red_atk;
                    if(blue_hp <= 0) {
                        red_warriors[i]->alive = false;
                        blue_warriors[i]->alive = false;
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minite << " "
                             << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
                             << blue_warriors[i]->num << " used a bomb and killed " << red_warriors[i]->gov->colour << " "
                             << war_mp[red_warriors[i]->type] << " " << red_warriors[i]->num << endl;
                        if(blue_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(blue_warriors[i]); if(d && d->wea == blue_bomb) d->wea = nullptr; }
                        if(blue_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]); if(ic && ic->wea == blue_bomb) ic->wea = nullptr; }
                        if(blue_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(blue_warriors[i]); if(n) { if(n->wea1 == blue_bomb) n->wea1 = nullptr; if(n->wea2 == blue_bomb) n->wea2 = nullptr; } }
                        if(blue_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(blue_warriors[i]); if(w && w->b == blue_bomb) w->b = nullptr; }
                        delete blue_bomb;
                        continue;
                    }
                }
            }
            // 2. 蓝方被动防守，如果被红方主动攻击会死，蓝方自爆
            if(!blue_first) {
                int red_atk = red_warriors[i]->cur_atk;
                sword* red_sword = dynamic_cast<sword*>(red_warriors[i]->get_sword());
                if(red_sword) red_atk += red_sword->attack;
                int blue_hp = blue_warriors[i]->cur_hp - red_atk;
                if(blue_hp <= 0) {
                    red_warriors[i]->alive = false;
                    blue_warriors[i]->alive = false;
                    cout << setw(3) << setfill('0') << hour << ":"
                         << setw(2) << setfill('0') << minite << " "
                         << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
                         << blue_warriors[i]->num << " used a bomb and killed " << red_warriors[i]->gov->colour << " "
                         << war_mp[red_warriors[i]->type] << " " << red_warriors[i]->num << endl;
                    if(blue_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(blue_warriors[i]); if(d && d->wea == blue_bomb) d->wea = nullptr; }
                    if(blue_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]); if(ic && ic->wea == blue_bomb) ic->wea = nullptr; }
                    if(blue_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(blue_warriors[i]); if(n) { if(n->wea1 == blue_bomb) n->wea1 = nullptr; if(n->wea2 == blue_bomb) n->wea2 = nullptr; } }
                    if(blue_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(blue_warriors[i]); if(w && w->b == blue_bomb) w->b = nullptr; }
                    delete blue_bomb;
                    continue;
                }
            }
        }
    }
}
void city::produce_elements(int hour,int minute){
    for(int i=1;i<=N;++i){
        city_elements[i] += 10;
    }
}
void city::print_get_elements(int i,bool c,int hour,int minute){
    // cout << i << endl;
    if(c){//红
        cout << setw(3) << setfill('0') << hour << ":"
        << setw(2) << setfill('0') << minute << " "
        << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
        << red_warriors[i]->num << " earned " << city_elements[i] <<  " elements for his headquarter" << endl;
    }
    else{//蓝
        cout << setw(3) << setfill('0') << hour << ":"
        << setw(2) << setfill('0') << minute << " "
        << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
        << blue_warriors[i]->num << " earned " << city_elements[i] <<  " elements for his headquarter" << endl;
    }

}
void city::take_elements(int hour,int minute){
    for(int i=1;i<=N;++i){
        if(red_warriors[i] != nullptr && blue_warriors[i] == nullptr){
            print_get_elements(i,true,hour,minute);
            red->elements += city_elements[i];
            city_elements[i] = 0;
        }else if(red_warriors[i] == nullptr && blue_warriors[i] != nullptr){
            print_get_elements(i,false,hour,minute);
            blue->elements += city_elements[i];
            city_elements[i] = 0;
        }
    }
}
void warrior::fight(warrior* enemy,bool flag){
    if(!alive || !enemy->alive) return;
    auto sw = get_sword();
    sword* sef = dynamic_cast<sword*>(sw);
    int atk = (sef == nullptr) ? 0 : sef->attack;
    if(flag) atk += cur_atk;
    else atk += cur_atk / 2;
    int tmp_hp = enemy->cur_hp;
    enemy->cur_hp -= atk;
    if(sw != nullptr){
        sword* sef = dynamic_cast<sword*>(sw);
        sef->update();
        if(sef->attack <= 0){
            // 这里要同步置空
            if(type == 0) { // dragon
                dragon* d = dynamic_cast<dragon*>(this);
                if(d && d->wea == sw) d->wea = nullptr;
            }
            if(type == 2) { // iceman
                iceman* ic = dynamic_cast<iceman*>(this);
                if(ic && ic->wea == sw) ic->wea = nullptr;
            }
            if(type == 1) { // ninja
                ninja* n = dynamic_cast<ninja*>(this);
                if(n) {
                    if(n->wea1 == sw) n->wea1 = nullptr;
                    if(n->wea2 == sw) n->wea2 = nullptr;
                }
            }
            if(type == 4) { // wolf
                wolf* w = dynamic_cast<wolf*>(this);
                if(w && w->s == sw) w->s = nullptr;
            }
            delete sw;
            sw = nullptr;
        }
    }
    if(enemy->cur_hp <= 0) {
        enemy->alive = false;
        if(enemy->type == 3){//enemy 是 lion
            cur_hp += tmp_hp;
        }
    }
}
void city::city_fight(int hour,int minute){
    vector<bool> award_red(N+1,false),award_blue(N+1,false);
    for(int i=1;i<=N;++i){
        //特判一些不需要发生战斗的情况
        if(red_warriors[i] == nullptr && blue_warriors[i] == nullptr) continue;
        if(red_warriors[i] == nullptr){
            if(blue_warriors[i]->alive == false) {
                delete blue_warriors[i];
                blue_warriors[i] = nullptr;
            }
            continue;
        }
        if(blue_warriors[i] == nullptr){
            if(red_warriors[i]->alive == false){
                delete red_warriors[i];
                red_warriors[i] = nullptr;
            }
            continue;
        }
        if(red_warriors[i]->alive == false && blue_warriors[i]->alive == false){
            delete red_warriors[i],delete blue_warriors[i];
            red_warriors[i] = nullptr,blue_warriors[i] = nullptr;continue;
        }
        bool flg = ( flag[i] == 1 || (flag[i] == 0 && i % 2 == 1) );//flg = 1,红方先攻 flg = 0,蓝方先攻
        if(flg == true){
            bool attacked = false;
            if(red_warriors[i]->alive == true){
                if(blue_warriors[i]->is_alive()){
                    cout << setw(3) << setfill('0') << hour << ":"
                    << setw(2) << setfill('0') << minute << " "
                    << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
                    << red_warriors[i]->num << " attacked " << blue_warriors[i]->gov->colour << " "
                    << war_mp[blue_warriors[i]->type] << " " << blue_warriors[i]->num << " in city " << i 
                    << " with " << red_warriors[i]->cur_hp << " elements and force " << red_warriors[i]->cur_atk << endl;
                    red_warriors[i]->fight(blue_warriors[i],true);
                    attacked = true;
                    if(blue_warriors[i]->is_alive() == false){
                        cout << setw(3) << setfill('0') << hour << ":"
                        << setw(2) << setfill('0') << minute << " "
                        << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
                        << blue_warriors[i]->num << " was killed in city " <<  i << endl;
                    }
                }
                if(blue_warriors[i]->is_alive() && blue_warriors[i]->type != 1){//不是ninja
                    blue_warriors[i]->fight(red_warriors[i],false);
                    cout << setw(3) << setfill('0') << hour << ":"
                    << setw(2) << setfill('0') << minute << " "
                    << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
                    << blue_warriors[i]->num << " fought back against " << red_warriors[i]->gov->colour << " "
                    << war_mp[red_warriors[i]->type] << " " << red_warriors[i]->num << " in city " << i << endl;
                    if(red_warriors[i]->is_alive() == false){
                        cout << setw(3) << setfill('0') << hour << ":"
                        << setw(2) << setfill('0') << minute << " "
                        << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
                        << red_warriors[i]->num << " was killed in city " <<  i << endl;
                    }
                }
            }         
        }
        else{
            if(blue_warriors[i]->alive == true){
                bool attacked = false;
                if(red_warriors[i]->is_alive()){
                    cout << setw(3) << setfill('0') << hour << ":"
                    << setw(2) << setfill('0') << minute << " "
                    << blue_warriors[i]->gov->colour << " " << war_mp[blue_warriors[i]->type] << " "
                    << blue_warriors[i]->num << " attacked " << red_warriors[i]->gov->colour << " "
                    << war_mp[red_warriors[i]->type] << " " << red_warriors[i]->num << " in city " << i 
                    << " with " << blue_warriors[i]->cur_hp << " elements and force " << blue_warriors[i]->cur_atk << endl;
                    blue_warriors[i]->fight(red_warriors[i],true);
                    attacked = true;
                    if(red_warriors[i]->is_alive() == false){
                        cout << setw(3) << setfill('0') << hour << ":"
                        << setw(2) << setfill('0') << minute << " "
                        << red_warriors[i]->gov->colour << " " << war_mp[red_warriors[i]->type] << " "
                        << red_warriors[i]->num << " was killed in city " <<  i << endl;
                    }
                }
                if(red_warriors[i]->is_alive() && red_warriors[i]->type != 1){//不是ninja
                    red_warriors[i]->fight(blue_warriors[i],false);
                    cout << setw(3) << setfill('0') << hour << ":"
                    << setw(2) << setfill('0') << minute << " "
                    << red_warriors[i]->gov->colour << " " 
                    << war_mp[red_warriors[i]->type] 
                    <<" "<< red_warriors[i]->num 
                    <<" fought back against "<< blue_warriors[i]->gov->colour 
                    <<" "<< war_mp[blue_warriors[i]->type] 
                    <<" "<< blue_warriors[i]->num 
                    <<" in city "<< i<< endl;
                    if(blue_warriors[i]->is_alive() == false){
                        cout<<setw(3)<<setfill('0')<<hour<<":"
                        <<setw(2)<<setfill('0')<<minute<<" "
                        <<blue_warriors[i]->gov->colour<<" "<<war_mp[blue_warriors[i]->type]<<" "
                        <<blue_warriors[i]->num<<" was killed in city "<<i<<endl;
                    }
                }
            }
        }
        //战斗结果
        // 红方胜利
        if(red_warriors[i]->is_alive() && blue_warriors[i]->is_alive() == false){
            // 先判断dragon欢呼与morale更新
            if(flg && red_warriors[i]->type == 0){
                dragon* dr = dynamic_cast<dragon*>(red_warriors[i]);
                if(dr){
                    dr->morale += 0.2;
                    if(dr->morale > 0.8){
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minute << " "
                             << dr->gov->colour << " " << war_mp[dr->type] << " "
                             << dr->num << " yelled in city " << i << endl;
                    }
                }
            }
            print_get_elements(i,true,hour,minute);
            if(red_warriors[i]->type == 4){//wolf 抢武器
                wolf* w = dynamic_cast<wolf*>(red_warriors[i]);
                // 抢箭
                if(w && w->a == nullptr && blue_warriors[i] != nullptr){
                    arrow* ba = dynamic_cast<arrow*>(blue_warriors[i]->get_arrow());
                    if(ba){
                        w->a = ba;
                        if(blue_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(blue_warriors[i]);
                            if(d && d->wea == ba) d->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]);
                            if(ic && ic->wea == ba) ic->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(blue_warriors[i]);
                            if(n) {
                                if(n->wea1 == ba) n->wea1 = nullptr;
                                if(n->wea2 == ba) n->wea2 = nullptr;
                            }
                        }
                        if(blue_warriors[i]->type == 4) {
                            wolf* bw = dynamic_cast<wolf*>(blue_warriors[i]);
                            if(bw && bw->a == ba) bw->a = nullptr;
                        }
                    }
                }
                // 抢bomb
                if(w && w->b == nullptr && blue_warriors[i] != nullptr){
                    bomb* bb = dynamic_cast<bomb*>(blue_warriors[i]->get_bomb());
                    if(bb){
                        w->b = bb;
                        if(blue_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(blue_warriors[i]);
                            if(d && d->wea == bb) d->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]);
                            if(ic && ic->wea == bb) ic->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(blue_warriors[i]);
                            if(n) {
                                if(n->wea1 == bb) n->wea1 = nullptr;
                                if(n->wea2 == bb) n->wea2 = nullptr;
                            }
                        }
                        if(blue_warriors[i]->type == 4) {
                            wolf* bw = dynamic_cast<wolf*>(blue_warriors[i]);
                            if(bw && bw->b == bb) bw->b = nullptr;
                        }
                    }
                }
                // 抢sword
                if(w && w->s == nullptr && blue_warriors[i] != nullptr){
                    sword* bs = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
                    if(bs){
                        w->s = bs;
                        if(blue_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(blue_warriors[i]);
                            if(d && d->wea == bs) d->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]);
                            if(ic && ic->wea == bs) ic->wea = nullptr;
                        }
                        if(blue_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(blue_warriors[i]);
                            if(n) {
                                if(n->wea1 == bs) n->wea1 = nullptr;
                                if(n->wea2 == bs) n->wea2 = nullptr;
                            }
                        }
                        if(blue_warriors[i]->type == 4) {
                            wolf* bw = dynamic_cast<wolf*>(blue_warriors[i]);
                            if(bw && bw->s == bs) bw->s = nullptr;
                        }
                    }
                }
            }
            win_cnt[i] = max(0,win_cnt[i]);
            win_cnt[i]++;
            if(win_cnt[i] == 2 && flag[i] != 1){//插红旗
                flag[i] = 1;
                cout<<setw(3)<<setfill('0')<<hour<<":"
                <<setw(2)<<setfill('0')<<minute<<" "
                <<red_warriors[i]->gov->colour<<" flag raised in city "<< i <<endl;
            }
            award_red[i] = true;
            delete blue_warriors[i];
            blue_warriors[i] = nullptr;
        }
        // 蓝方胜利
        else if(blue_warriors[i]->is_alive() && red_warriors[i]->is_alive() == false){
            if(!flg && blue_warriors[i]->type == 0){
                dragon* dr = dynamic_cast<dragon*>(blue_warriors[i]);
                if(dr){
                    dr->morale += 0.2;
                    if(dr->morale > 0.8){
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minute << " "
                             << dr->gov->colour << " " << war_mp[dr->type] << " "
                             << dr->num << " yelled in city " << i << endl;
                    }
                }
            }
            print_get_elements(i,false,hour,minute);
            if(blue_warriors[i]->type == 4){//wolf 抢武器
                wolf* w = dynamic_cast<wolf*>(blue_warriors[i]);
                // 抢箭
                if(w && w->a == nullptr && red_warriors[i] != nullptr){
                    arrow* ra = dynamic_cast<arrow*>(red_warriors[i]->get_arrow());
                    if(ra){
                        w->a = ra;
                        if(red_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(red_warriors[i]);
                            if(d && d->wea == ra) d->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(red_warriors[i]);
                            if(ic && ic->wea == ra) ic->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(red_warriors[i]);
                            if(n) {
                                if(n->wea1 == ra) n->wea1 = nullptr;
                                if(n->wea2 == ra) n->wea2 = nullptr;
                            }
                        }
                        if(red_warriors[i]->type == 4) {
                            wolf* rw = dynamic_cast<wolf*>(red_warriors[i]);
                            if(rw && rw->a == ra) rw->a = nullptr;
                        }
                    }
                }
                // 抢bomb
                if(w && w->b == nullptr && red_warriors[i] != nullptr){
                    bomb* rb = dynamic_cast<bomb*>(red_warriors[i]->get_bomb());
                    if(rb){
                        w->b = rb;
                        if(red_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(red_warriors[i]);
                            if(d && d->wea == rb) d->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(red_warriors[i]);
                            if(ic && ic->wea == rb) ic->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(red_warriors[i]);
                            if(n) {
                                if(n->wea1 == rb) n->wea1 = nullptr;
                                if(n->wea2 == rb) n->wea2 = nullptr;
                            }
                        }
                        if(red_warriors[i]->type == 4) {
                            wolf* rw = dynamic_cast<wolf*>(red_warriors[i]);
                            if(rw && rw->b == rb) rw->b = nullptr;
                        }
                    }
                }
                // 抢sword
                if(w && w->s == nullptr && red_warriors[i] != nullptr){
                    sword* rs = dynamic_cast<sword*>(red_warriors[i]->get_sword());
                    if(rs){
                        w->s = rs;
                        if(red_warriors[i]->type == 0) {
                            dragon* d = dynamic_cast<dragon*>(red_warriors[i]);
                            if(d && d->wea == rs) d->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 2) {
                            iceman* ic = dynamic_cast<iceman*>(red_warriors[i]);
                            if(ic && ic->wea == rs) ic->wea = nullptr;
                        }
                        if(red_warriors[i]->type == 1) {
                            ninja* n = dynamic_cast<ninja*>(red_warriors[i]);
                            if(n) {
                                if(n->wea1 == rs) n->wea1 = nullptr;
                                if(n->wea2 == rs) n->wea2 = nullptr;
                            }
                        }
                        if(red_warriors[i]->type == 4) {
                            wolf* rw = dynamic_cast<wolf*>(red_warriors[i]);
                            if(rw && rw->s == rs) rw->s = nullptr;
                        }
                    }
                }
            }
            win_cnt[i] = min(0,win_cnt[i]);
            win_cnt[i]--;
            if(win_cnt[i] == -2 && flag[i] != 2){//插蓝旗
                flag[i] = 2;
                cout<<setw(3)<<setfill('0')<<hour<<":"
                <<setw(2)<<setfill('0')<<minute<<" "
                <<blue_warriors[i]->gov->colour<<" flag raised in city "<< i <<endl;
            }
            award_blue[i] = true;
            delete red_warriors[i];
            red_warriors[i] = nullptr;
        }
        // 平局
        else if(red_warriors[i]->is_alive() && blue_warriors[i]->is_alive()){
            // 同步更新morale
            if(flg && red_warriors[i]->type == 0){
                dragon* dr = dynamic_cast<dragon*>(red_warriors[i]);
                if(dr){
                    dr->morale -= 0.2;
                    if(dr->morale > 0.8){
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minute << " "
                             << dr->gov->colour << " " << war_mp[dr->type] << " "
                             << dr->num << " yelled in city " << i << endl;
                    }
                }
            }
            if(!flg && blue_warriors[i]->type == 0){
                dragon* dr = dynamic_cast<dragon*>(blue_warriors[i]);
                if(dr){
                    dr->morale -= 0.2;
                    if(dr->morale > 0.8){
                        cout << setw(3) << setfill('0') << hour << ":"
                             << setw(2) << setfill('0') << minute << " "
                             << dr->gov->colour << " " << war_mp[dr->type] << " "
                             << dr->num << " yelled in city " << i << endl;
                    }
                }
            }
            if(red_warriors[i]->type == 3){
                lion* li = dynamic_cast<lion*>(red_warriors[i]);
                li->loyalty -= K;
            }
            if(blue_warriors[i]->type == 3){
                lion* li = dynamic_cast<lion*>(blue_warriors[i]);
                li->loyalty -= K;
            }
            win_cnt[i] = 0;        
        }
    }
    //开始奖励
    //红方 从N到1
    for(int i=N;i>=1;--i){
        if(red->elements < 8)break;
        if(award_red[i]){
            red_warriors[i]->cur_hp += 8;
            red->elements -= 8;
        }
    }
    //蓝方 从1到N
    for(int i=1;i<=N;++i){
        if(blue->elements < 8)break;
        if(award_blue[i]){
            blue_warriors[i]->cur_hp += 8;
            blue->elements -= 8;
        }
    }
    //红方 获取城市的生命元
    for(int i=1;i<=N;++i){
        if(award_red[i] && city_elements[i] > 0){
            // print_get_elements(i,true,hour,minute);
            red->elements += city_elements[i];
            city_elements[i] = 0;
        }
    }
    //蓝方 获取城市的生命元
    for(int i=1;i<=N;++i){
        if(award_blue[i] && city_elements[i] > 0){
            // print_get_elements(i,false,hour,minute);
            blue->elements += city_elements[i];
            city_elements[i] = 0;
        }
    }
}void city::announce_weapon(int hour,int minute){
    for(int i=0;i<=N+1;++i){
        if(red_warriors[i] != nullptr){
            arrow* a = dynamic_cast<arrow*>(red_warriors[i]->get_arrow());
            bomb* b = dynamic_cast<bomb*>(red_warriors[i]->get_bomb());
            sword* s = dynamic_cast<sword*>(red_warriors[i]->get_sword());
            // 清理无效 sword
            if(s != nullptr && s->attack <= 0){
                // 置空指针
                if(red_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(red_warriors[i]); if(d && d->wea == s) d->wea = nullptr; }
                if(red_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(red_warriors[i]); if(ic && ic->wea == s) ic->wea = nullptr; }
                if(red_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(red_warriors[i]); if(n) { if(n->wea1 == s) n->wea1 = nullptr; if(n->wea2 == s) n->wea2 = nullptr; } }
                if(red_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(red_warriors[i]); if(w && w->s == s) w->s = nullptr; }
                delete s;
                s = nullptr;
            }
            cout << setw(3) << setfill('0') << hour << ":" 
            << setw(2) << setfill('0') << minute << " " 
            << red_warriors[i]->gov->colour << " " 
            << war_mp[red_warriors[i]->type] << " " 
            << red_warriors[i]->num << " has ";
            int cnt = 0;
            if(a != nullptr){
                cnt++;
                cout << "arrow(" << a->cnt << ")";
            }
            if(b != nullptr){
                if(cnt > 0) cout << ",";
                cout << "bomb";
                cnt++;
            }
            // 只输出有效 sword
            s = dynamic_cast<sword*>(red_warriors[i]->get_sword());
            if(s != nullptr && s->attack > 0){
                if(cnt > 0) cout << ",";
                cout << "sword(" << s->attack << ")";
                cnt++;
            }
            if(cnt == 0) cout << "no weapon";
            cout << endl;
        }
    }
    for(int i=0;i<=N+1;++i){
        if(blue_warriors[i] != nullptr){
            arrow* a = dynamic_cast<arrow*>(blue_warriors[i]->get_arrow());
            bomb* b = dynamic_cast<bomb*>(blue_warriors[i]->get_bomb());
            sword* s = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
            // 清理无效 sword
            if(s != nullptr && s->attack <= 0){
                if(blue_warriors[i]->type == 0) { dragon* d = dynamic_cast<dragon*>(blue_warriors[i]); if(d && d->wea == s) d->wea = nullptr; }
                if(blue_warriors[i]->type == 2) { iceman* ic = dynamic_cast<iceman*>(blue_warriors[i]); if(ic && ic->wea == s) ic->wea = nullptr; }
                if(blue_warriors[i]->type == 1) { ninja* n = dynamic_cast<ninja*>(blue_warriors[i]); if(n) { if(n->wea1 == s) n->wea1 = nullptr; if(n->wea2 == s) n->wea2 = nullptr; } }
                if(blue_warriors[i]->type == 4) { wolf* w = dynamic_cast<wolf*>(blue_warriors[i]); if(w && w->s == s) w->s = nullptr; }
                delete s;
                s = nullptr;
            }
            cout << setw(3) << setfill('0') << hour << ":" 
            << setw(2) << setfill('0') << minute << " " 
            << blue_warriors[i]->gov->colour << " " 
            << war_mp[blue_warriors[i]->type] << " " 
            << blue_warriors[i]->num << " has ";
            int cnt = 0;
            if(a != nullptr){
                cnt++;
                cout << "arrow(" << a->cnt << ")";
            }
            if(b != nullptr){
                if(cnt > 0) cout << ",";
                cout << "bomb";
                cnt++;
            }
            s = dynamic_cast<sword*>(blue_warriors[i]->get_sword());
            if(s != nullptr && s->attack > 0){
                if(cnt > 0) cout << ",";
                cout << "sword(" << s->attack << ")";
                cnt++;
            }
            if(cnt == 0) cout << "no weapon";
            cout << endl;
        }
    }
}
void solve(){
    cin >> M >> N >> R >> K >> T;
    for(int i=0;i<5;++i) cin >> hp[i];
    for(int i=0;i<5;++i) cin >> atk[i];
    mn = INT_MAX;
    for(int i=0;i<5;++i) mn = min(mn,hp[i]);
    city citys(N);
    head red(M,"red",&citys);
    head blue(M,"blue",&citys);
    citys.add_head(&red, &blue);
    for(int hour = 0; hour * 60 <= T;++hour){
        int minute = 0;
        if(hour * 60 + minute > T) break;
        red.produce(hour,minute);
        blue.produce(hour,minute);
        minute = 5;
        if(hour * 60 + minute > T) break;
        citys.lion_runaway(hour,minute);
        minute = 10;
        if(hour * 60 + minute > T) break;
        if(citys.move(hour,minute)!=0) break;
        minute = 20;
        if(hour * 60 + minute > T) break;
        citys.produce_elements(hour,minute);
        minute = 30;
        if(hour * 60 + minute > T) break;
        citys.take_elements(hour,minute);
        minute = 35;
        if(hour * 60 + minute > T) break;
        citys.arrow_attack(hour,minute);
        minute = 38;
        if(hour * 60 + minute > T) break;
        citys.bomb_attack(hour,minute);
        minute = 40;
        if(hour * 60 + minute > T) break;
        citys.city_fight(hour,minute);
        minute = 50;
        if(hour * 60 + minute > T) break;
        red.announce_elements(hour,minute);
        blue.announce_elements(hour,minute);
        minute = 55;
        if(hour * 60 + minute > T) break;
        citys.announce_weapon(hour,minute);
    }
}
int main(){
    // ifstream fin("Warcraft.in");
    // ofstream fout("war_output.out");   
    // streambuf* cinbuf = cin.rdbuf();
    // streambuf* coutbuf = cout.rdbuf();
    // cin.rdbuf(fin.rdbuf());
    // cout.rdbuf(fout.rdbuf());
    int t;
    cin >> t;
    for(int i=1;i<=t;++i){
        cout << "Case " << i <<":"<< endl;
        solve();
    }
}