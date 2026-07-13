#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <math.h>

struct player_attribute {
    int max_move_point;
    int max_health;
    int health;
    int def;
    int move_point;
    int pull_num;
};

struct monster_attribute {
    int id;
    char name[30];
    int max_health;
    int health;
    int atk;
    int def;
    int move;
    int speed;
    bool mul;
    int ex_speed;
    char info[200];
};

struct skill {
    int id;
    char name[100];
    int cost;
    int atk;
    int heal;
    int def;
    bool mul;
    int exhaust;
    char info[200];
    int tag;
};

struct effect {
    int rage;
    int power;
    int regenerate;
    int fire; //-1兩傷 
    int poison; //除2等量傷 
    int bleed;  //全清等量傷 
    int spike;
    int deride; //嘲諷 
};

struct relic_attribute{
	int id;
	int count;
	char name[100];
	char info[200];
	bool have;
};

const struct skill skill_db[200] = {
    
    /*
    給自己的話 
	嘗試以流派的角度思考 分為dot(火焰法師) 快速出牌(刺客)
	反傷與護盾轉傷害(spike效果) 純攻擊(戰士) 消耗品流(女巫) 燒血流(考慮是否獨立於反傷流) 
	*/ 
	//id 名稱 費用 傷害 治療 防禦 是否全體 是否消耗 tag
    {0, "無", 0,0,0,0,0,0,"你怎麼看到的",0},
    {1, "打擊",1,3,0,0,0,0,"輕輕敲醒沉睡的心靈",0},
    {2, "防禦",1,0,0,3,0,0,"怕痛的我...",0},
    {3, "疲勞",99,0,0,0,0,2,"這裡不讓睡大覺",0},
    {4,"連擊",0,1,0,0,0,1,"消耗品，使用後抽一張牌，不計入消耗",0},
    {5,"蛛網",1,0,0,0,0,1,"無法...動彈...",0},
	{6,"重傷",0,0,0,0,0,1,"疼痛使你難以行動，使用後受到3點真實傷害",0},
	{7,"可疑的藥水",0,0,0,0,0,1,"如同打開潘朵拉的盒子...",0},
	{8,"垃圾",99,0,0,0,0,0,"為何要在背包放這個...",0},
	{9,"深淵詛咒",99,0,0,0,0,0,"黑暗的力量使你極度虛弱",0},
	{19,"解藥",0,0,0,0,0,2,"消耗品，移除自身中毒，並獲得1層再生",0},
	{20,"毒藥",0,1,0,0,0,2,"消耗品，給予目標4層中毒",0},
	{21,"好戲開場:2",1,0,0,0,0,1,"消耗品，獲得1張「好戲開場:1」",0},
	{22,"好戲開場:1",1,0,0,0,0,1,"消耗品，獲得1張「好戲開場」",0},
	{23,"好戲開場",1,7,0,0,1,1,"全體攻擊，消耗品，獲得1張「好戲開場:3」",0},
	{24,"狂歡不止",0,2,0,0,0,1,"消耗品，抽1張牌",0},
	{25,"劍氣",0,2,0,0,0,2,"消耗品，沒人知道這到底怎麼形成的",0},
	//25後是一般前期卡池 
	{27,"肉身打擊",1,2,0,0,0,0,"如果沒有防禦，傷害+2",0},
	{28,"庇佑",2,0,3,3,0,0,"最好的避風港",0},
	{29,"招架",0,0,0,1,0,0,"簡單的擋一下",0},
	
    {31,"迴旋斬",2,4,0,0,1,0,"全體攻擊，轉圈圈~",0},
    {32,"狂化",2,0,0,0,0,0,"給予2層狂怒，使攻擊傷害提高2",0},
    {33,"刺擊",0,2,0,0,0,0,"就像被蚊子叮，不過痛一點",0},
    {34,"補給",1,0,0,0,0,0,"抽2張牌",0},
    {35,"暖身",1,0,0,0,0,0,"下回合額外獲得2點行動值",0},
    {36,"怒火",0,0,0,0,0,0,"點燃自己，並獲得1點狂怒",0}, 
    {37,"銅牆",2,0,0,7,0,0,"更硬了",0},
    {38,"雜耍",1,2,1,1,0,0,"亂搞一通，就說有沒有效吧",0},
    {39,"加速藥水",0,0,0,0,0,1,"消耗品，立刻獲得3點行動值",0},
    {40,"治愈術",2,0,5,0,0,0,"藥王慈懷，建木生發。",0},
    {41,"傷害藥水",0,5,0,0,0,1,"消耗品，傷害主要來自於碎玻璃",0},
    {42,"猩紅爪",1,2,2,0,0,0,"爪子上紅色液體的其實是紅酒",0},
    
    {47,"戰術規劃",0,0,0,0,0,0,"移除所有手牌，並立刻抽等量的牌",0},
    {48,"手裡劍",1,1,0,0,0,0,"額外彈射5次",0},
    {49,"治療藥水",1,0,1,0,0,1,"消耗品，獲得1層再生",0},
    
    //50後是一般後期卡池 
    {51,"劍術．百花繚亂",3,6,0,0,1,0,"全體攻擊，其實我也不知道在揮哪裡XD",0},
    {52,"全心防禦",0,0,0,1,0,0,"消耗所有行動，每點獲得5點防禦",0},
    {53,"全力一擊",2,15,0,0,0,0,"攻擊使你疲憊，獲得4張疲勞",0},
    {54,"重裝",3,0,0,11,0,0,"you should not pass!!!",0},
    {55,"天使之護",3,0,6,4,0,0,"能讓我摸摸翅膀嗎?",0},
    {56,"聖光",1,0,0,0,0,0,"移除所有效果",0},
    {57,"燃燒瓶",1,1,0,0,1,1,"消耗品，全體攻擊，給予所有目標2層燃燒",0},
    {58,"防守反擊",2,2,0,5,0,0,"集防禦與攻擊於一身",0},
    
    //100後是職業專屬
    //戰士 
    {101,"冷靜",1,0,0,0,0,0,"將所有狂怒轉化成再生",0},
    {102,"磨刀",2,0,0,0,0,1,"消耗品，使一張牌攻擊力+1，僅此局戰鬥生效",0},
    {103,"嗜血",2,4,0,0,0,0,"回復造成傷害量50%的生命值",0},
    {104,"雙刀流",1,2,0,0,0,0,"額外造成一次50%傷害的攻擊",0},
    {105,"弱點打擊",2,4,0,0,0,0,"造成傷害後，斬殺生命值低於此次攻擊100%的敵人",0},
    {106,"怒氣爆發",1,3,0,0,0,0,"消耗所有怒氣提升傷害，每點提升3點傷害",0},
    {107,"三連斬",3,3,0,0,0,0,"連續攻擊三次",0},
    {108,"十字斬",3,4,0,0,1,0,"全體攻擊，對選中目標造成雙倍傷害",0},
    {109,"折返",4,4,0,0,0,0,"對生命值最低的單位造成一次150%傷害的攻擊，如果殺死該單位，再執行一次",0},
    {110,"蝶舞",5,4,0,0,1,0,"全體攻擊，彈射4次，每次造成100%傷害，彈射必定命中",0},
    //火法 
    {111,"炎槍",1,2,0,0,0,0,"給予目標2層燃燒",0},
    {112,"爆燃",2,3,0,0,1,0,"全體攻擊，引爆主目標的燃燒",0},
    {113,"蒸氣護盾",1,0,0,0,0,0,"將所有燃燒移除，並獲得雙倍量護盾",0},
    {114,"火球",1,1,0,0,0,0,"點燃所有敵人",0},
    {115,"自燃",0,0,1,0,0,0,"給予自身2層燃燒",0},
    {116,"融斷",1,3,0,0,0,0,"自身每有1層燃燒使傷害+1，並移除1層燃燒",0},
    {117,"浴火重生",2,0,0,0,0,0,"移除所有燃燒，每移除一層獲得1層再生",0},
    {118,"聚爆",3,4,0,0,1,0,"全體攻擊，引爆所有燃燒",0},
    {119,"焠火",2,0,0,0,0,1,"消耗品，使1張牌的詞條替換成「高溫的」，僅此局戰鬥生效",0},
    {120,"流星雨",5,0,0,0,1,0,"彈射8次，每次造成3點傷害並給予2層燃燒，彈射必定命中",0},
    //武士 
    {121,"全彈發射",2,2,0,0,1,0,"全體攻擊，額外彈射2次，每打出4張牌多彈射1次",0}, 
    {122,"拔刀",1,0,0,0,0,0,"獲得2張連擊",0},
    {123,"血刃",2,3,0,1,1,0,"全體攻擊，每打出5張牌額外對主目標造成1點傷害，並回覆1點生命",0},
    {124,"亂華",1,2,0,0,0,0,"使用手牌所有的連擊，並獲得等量的連擊",0},
    {125,"蓄力重擊",1,2,0,0,0,0,"每打出4張牌額外造成1點傷害，上限6點",0},
    {126,"破空",2,3,0,0,0,0,"手牌添加兩張劍氣",0},
    {127,"劍舞",2,5,0,0,0,0,"獲得等同出牌數平方根量的連擊",0},
    {128,"起手式",0,2,0,0,0,0,"抽1張牌，每次打出此牌有25%概率使費用+1，上限1點",0},
    {129,"突刺",3,7,0,0,0,0,"若出牌數達到15/30/50，造成150%/200%/450%傷害",0},
    {130,"奧義．一閃",5,13,0,0,0,0,"每打出10張牌額外攻擊1次，最多額外攻擊4次",0},
    //刺客 
    {131,"連環斬擊",2,3,0,0,1,0,"全體攻擊，每次打出此牌傷害都會提升1",0},
    {132,"重複保護",1,0,0,3,0,0,"每次打出此牌使防禦提升1",0},
    {133,"閃擊",1,3,0,0,0,0,"抽1張牌",0},
    {134,"模仿",1,0,0,0,0,0,"給予一張指定手牌的複製體",0},
    {135,"策略進攻",1,3,0,0,0,0,"每次打出此牌傷害都會提升1",0},
    {136,"再現",1,0,0,0,0,0,"將上張棄置的牌從棄牌堆返回手中",0},
    {137,"好戲開場:3",1,0,0,0,0,1,"消耗品，獲得1張「好戲開場:2」",0},
    {138,"專一",2,0,0,0,0,1,"消耗品，使1張牌的詞條替換成「複現的」，僅此局戰鬥生效",0},
    {139,"暗箭",2,3,0,0,0,0,"每次攻擊提升1點傷害，並給予等同基本傷害150%的中毒層數",0},
    {140,"影殺陣",4,6,0,0,1,0,"全體攻擊，每次打出此牌傷害都會提升2",0},
    //女巫 
    {141,"藥水雨",2,1,0,0,1,0,"全體攻擊，額外彈射6次，每消耗2張牌多彈射1次",0},
    {142,"女巫的佳釀",1,2,0,0,0,0,"每消耗3張牌傷害+1，上限6點",0},
    {143,"硬化燒瓶",1,3,0,0,0,0,"打出此牌視為消耗物品",0},
    {144,"緊急注射",1,0,0,0,0,0,"於手牌添加一張毒藥或解藥",0},
    {145,"重新釀造",1,0,0,0,0,0,"選擇一張可疑的藥水，立即移除該牌，並於棄牌堆中添加3倍數值的可疑的藥水",0},
    {146,"藥水袋",1,0,0,0,0,0,"獲得2張[可疑的藥水]",0}, //atk(20%/50%30%)(0,1,2) def(30%/60%/10%)(0,1,2) heal(70%/30%)(0,1)
    {147,"猛烈的雞尾酒",1,0,0,0,0,0,"給予目標中毒，數量等同卡牌消耗量，上限8層",0},
    {148,"烈酒",3,2,0,0,1,0,"全體攻擊，給予自身2層中毒，額外彈射5次，每消耗3張牌多彈射1次，造成1點傷害並給予1層中毒",0},
    {149,"腐蝕",2,3,0,0,0,0,"引爆目標中毒，傷害倍率隨卡牌消耗量提升，最多300%",0},
    {150,"藥水狂歡",5,2,0,0,1,0,"彈射7次，每次給予1層中毒並有40%的概率獲得1張狂歡不止，每消耗2張牌額外彈射1次，必定命中目標",0},
    //聖騎士 
    {151,"岩彈",2,2,0,0,1,0,"全體攻擊，彈射1次，消耗防禦提升彈射次數，上限5次",0},
    {152,"犧牲打擊",1,6,0,0,0,0,"對自己造成2點傷害",0},
    {153,"血祭",0,0,0,0,0,0,"對自己造成2點傷害，獲得1層狂怒，此傷害視為怪物造成傷害",0},
    {154,"盾擊",1,2,0,0,0,0,"消耗防禦提升傷害，上限3點",0},
    {155,"衝撞",2,5,0,0,0,0,"消耗所有防禦提升傷害",0},
    {156,"復仇",1,3,0,0,0,0,"若生命值低於25%，造成200%傷害",0},
    {157,"猛攻",2,1,0,0,1,0,"全體攻擊，消耗當前生命25%生命值，造成消耗量50%的傷害",0},
    {158,"癲狂",2,0,0,0,1,0,"對自己造成3次1點傷害，此傷害視為怪物造成傷害，並對所有單位造成2~6點傷害",0},
    {159,"岩石之心",1,0,0,0,0,0,"將66%的防禦力轉化成50%的治療",0},
    {160,"星裂",4,0,0,0,1,0,"全體攻擊，當前每損失1點生命值提升1點傷害，傷害會平分給所有單位，並使玩家獲得等同損失生命值33%的防禦",0},
};

const struct monster_attribute monster_db[100] = {
    // ID 名稱 最大血量 血量 傷害 護盾 行動偏好 速度 是否群體 額外速度 描述 
    //1攻擊 2防禦 3治療 4召喚 5特殊行動 case呼叫 
    {0,"無",0,0,0,0,0,0,0,0,"這裡什麼都沒有"},          
    {1,"哥布林",9,9,2,0,1,1,0,0,"森林中最常見的怪物"},
    {2,"吹箭哥布林",6,6,4,0,1,2,0,0,"很可惜，箭矢不帶毒"},
    {3,"哥布林鬥士",18,18,1,0,1,1,0,0,"與其叫鬥士，不如叫坦克"},
    {4,"哥布林巨人",25,25,1,0,4,3,0,0,"會持續招喚哥布林。你問哥布林哪來的？這我不好說"},
    {5,"精銳哥布林",14,14,2,0,1,1,0,0,"精銳中的精銳，哥布林中的哥布林"},
    {6,"火筒哥布林",8,8,12,0,1,4,0,1,"只待致命一擊"},
    {7,"哥布林首領",40,40,8,0,4,1,1,0,"他其實很懶，不會主動攻擊"},
    {8,"哥布林僕從",3,3,3,0,1,1,0,0,"天生的打工人"},
    {9,"食人魔",40,40,9,0,1,3,0,0,"獻出我們的心臟吧！抱歉串台了"},
    {10,"殭屍",28,23,3,0,1,2,0,0,"攻擊後會回血，叫吸血鬼是不是更合適？"},
    {11,"蜘蛛",20,20,1,5,1,2,0,1,"使玩家難以行動，如果沒留足夠的行動點，會更難行動"},
    {12,"野狼",16,16,2,0,1,1,0,0,"如果將玩家防禦打完，就會使玩家重傷"},
    {13,"史萊姆",13,13,1,0,1,1,0,0,"人畜無害？當他開始分裂時就不是這回事了"},
    {14,"焦屍",27,27,5,2,1,3,0,1,"高溫的殭屍，攻擊會點燃玩家，並且不受火焰傷害"},
    {15,"毒蜘蛛",18,18,1,5,1,2,0,1,"他繼承了蜘蛛的特性，並且攻擊有概率使玩家中毒"},
    {16,"狂狼",19,19,1,0,1,1,0,0,"如果防禦被他打完將產生嚴重的後果..."},
    {17,"荒野領主",100,100,0,15,5,1,0,0,"精明的術師，不只會招喚荒野上的怪物，也會攻擊、防禦和火球術"},
    {18,"砲兵",27,27,8,0,1,4,0,1,"城牆轟然倒塌..."},
    {19,"亡靈戰士",30,30,3,0,1,1,0,0,"忠誠的士兵於死後依然死守此地"},
    {20,"不死盾衛",50,50,4,10,2,2,1,1,"十分令人討厭，讓你忍不住想打他"},
    {21,"黑暗術師",35,35,7,0,2,2,0,0,"死前不斷的救助他人，死後也不忘本心"},
    {22,"近衛隊隊長",90,90,6,20,5,2,1,0,"數一數二的人才，擅長指揮眾人"},
    {23,"深暗巨龍",400,400,2,0,5,1,0,0,"被世人恐懼的存在，曾憑一己之力夷平一座城邦"},
};



int locate; 
int choice; 
int aim; 
int al_pull; 
int be_pull; 
int turn_count;
int special_move;
bool over; 
bool die;
bool already_choice_skill;
struct player_attribute player;
struct monster_attribute monster[3];
struct skill backpack[100];
struct skill skill_list[21];
struct skill skill_deck[100]; 
struct effect player_effect;
struct effect monster_effect[3];
struct skill discard_pile[100];
int al_use; //卡牌使用量
int al_discard; //已主動消耗量 
int al_move_use; //已使用行動點 
int discard_num = 0;
int deck_num = 0;       
int skill_num;
int final_atk;
int move_i;
bool can_act[3];
int map[13] = {1,2,1,1,3,2,4,1,3,1,3,2,5};
int monster_list[39][3] ={ 
    {0,1,0},
    {0,0,0},
    {2,0,2},
    {1,3,2},
    {0,0,0},
    {0,0,0},
    {1,4,1},//第一區的菁英 
    {1,5,2},
    {0,0,0},
    {6,5,6},
    {0,0,0},
    {0,0,0},
    {5,7,6},//第一區的王 
    {10,10,10},
    {0,0,0},
    {12,10,11},
    {13,11,10},
    {0,0,0},
    {0,0,0},
    {11,9,12},//第二區的菁英 
    {13,13,13},
    {0,0,0},
    {14,15,10},
    {0,0,0},
    {0,0,0},
    {16,17,14},//第二區的王 
    {19,20,19},
    {0,0,0},
    {19,20,18},
    {20,19,21},
	{0,0,0},
	{0,0,0},
	{19,22,19},//第三區的菁英 
	{19,19,19},
    {0,0,0},
    {20,18,20},
    {0,0,0},
    {0,0,0},
    {0,23,0},//第三區的王 
};

int monster_speed[39][3]={
	{0,0,0},
    {0,0,0},
    {0,0,1},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},//第一區的菁英 
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,1},//第一區的王 
    {0,1,0},
    {0,0,0},
    {0,0,0},
    {0,1,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},//第二區的菁英 
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,1,0},
    {0,0,0},
    {0,0,0},//第二區的王
	{0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},//第三區的菁英 
    {0,0,0},
    {1,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},//第三區的王
};

const int event_list[3][5]={
	{1,2,3,4,6},
	{1,2,5,6},
	{1,2,5,6}
};
int event_num[3]=
{5,4,4};

void add_skill(int target_id) { //向背包加牌 
    int find_i;
    for (find_i = 0; find_i < 200; find_i++) {
        if (skill_db[find_i].id == target_id) {
            backpack[skill_num] = skill_db[find_i];
            skill_num++;
            return; 
        }
    }
    printf("【系統錯誤】找不到 ID 為 %d 的技能！\n", target_id);
}

void add_deck_skill(int target_id) { //向牌庫加牌
    int find_i;
    for (find_i = 0; find_i < 200; find_i++) {
        if (skill_db[find_i].id == target_id) {
            skill_deck[deck_num] = skill_db[find_i];
            deck_num++;
            return; 
        }
    }
    printf("【系統錯誤】找不到 ID 為 %d 的技能！\n", target_id);
}

void add_discard_skill(int target_id) { //向棄牌堆加牌 
    int find_i;
    for (find_i = 0; find_i < 200; find_i++) {
        if (skill_db[find_i].id == target_id) {
            discard_pile[discard_num] = skill_db[find_i];
            discard_num++;
            return; 
        }
    }
    printf("【系統錯誤】找不到 ID 為 %d 的技能！\n", target_id);
}

void add_hand_skill(int target_id){//向手牌加牌 
	int find_i;
	int hand_num = get_hand_count();
    for (find_i = 0; find_i < 200; find_i++) {
        if (skill_db[find_i].id == target_id) {
            if(hand_num<20) skill_list[hand_num] = skill_db[find_i];
            return; 
        }
    }
    printf("【系統錯誤】找不到 ID 為 %d 的技能！\n", target_id);
}

void reset_deck() {
    int i;
    // 將廢牌堆的卡牌全部搬進牌庫
    for (i = 0; i < discard_num; i++) {
        skill_deck[deck_num] = discard_pile[i];
        deck_num++;
    }
    // 洗完牌後，清空廢牌堆
    discard_num = 0; 
}

void discard_list() {  //將手牌移至棄牌堆 
    int i;
    for(i = 0; i < 20 && skill_list[i].id != 0; i++) {
        if (skill_list[i].exhaust == 2) {
            // 狀態牌/虛無牌：直接蒸發，不進廢牌堆
            printf("%s 從你的手中消逝了（已被消耗）。\n", skill_list[i].name);
            Sleep(300);
        } else {
            // 一般卡片：進入廢牌堆
            if (skill_list[i].tag < 0) {
            skill_list[i].tag = -skill_list[i].tag;
        }
            discard_pile[discard_num] = skill_list[i];
            discard_num++;
        }
        // 清空該格手牌
        skill_list[i].id = 0;
    }
}

void pull_skill(int amount){ //抽牌 
    int i;
    int current_hand_count = 0;
    while (current_hand_count < 20 && skill_list[current_hand_count].id != 0) {
        current_hand_count++;
    }
    for (i = 0; i < amount; i++) {
        if (current_hand_count >= 20) {
            printf("手牌已滿！無法再抽牌。\n");
            break;
        }
        if (deck_num == 0) {
            reset_deck();
        }
        be_pull = rand() % deck_num;
        skill_list[current_hand_count] = skill_deck[be_pull];
        skill_deck[be_pull] = skill_deck[deck_num - 1];
        deck_num--;
        current_hand_count++;
    }
}

typedef enum{
	when_start_fight,//開始戰鬥  
	when_start_round, //開始玩家回合 
	when_attack, //攻擊時(與怒氣並行) 
	when_be_attack, //被怪物攻擊且破防時 
	when_round_over, //玩家回合結束時(優先於dot) 
	when_used_skill, //於用完技能時(可用於打牌數判定) 
	when_card_discard, //主動消耗卡牌時 
	when_monster_round_over, //怪物回合結束時(優先於dot) 
}
play_timing;

void healing(int num){
	player.health+=num;
	if(num>0){
		printf("你獲得了%d點治療\n",num);	
		Sleep(700);
	}
	if(player.health>player.max_health){
		player.health=player.max_health;
		printf("生命值已達上限\n");
		Sleep(500);
	}
}

struct relic_attribute relic[100] = {
	{0,-1,"無","手持空氣做啥呢?",0},
	{1,5,"戰士紋章","消耗狂怒時，回復2點生命，上限5次，進入戰鬥重製",0},
	{2,10,"火焰法師紋章","攻擊時，若主目標沒有燃燒，給予主目標1層燃燒，上限10次，進入戰鬥重製",0},
	{3,3,"武士紋章","戰鬥開始時，獲得2張連擊，使用連擊後，牌庫添加2張連擊，上限3次，進入戰鬥重製",0},
	{4,1,"刺客紋章","首次行動點為0時，進行洗牌，然後立即棄置所有卡牌，獲得3點能量，並抽取等同(抽牌數量-2)張牌",0}, 
	{5,7,"女巫紋章","使用可疑的藥水時，獲得一張數值固定的可疑的藥水，上限7次，進入戰鬥重製",0},
	{6,0,"聖騎士紋章","每次受到怪物傷害，使下一回合開始時，獲得2點防禦",0}, 
	{11,-1,"金蘋果","戰鬥開始時，回復2點生命",0},
	{12,-1,"鳳凰羽毛","自身燃燒時，每回合結束回復4點生命(不免疫燃燒傷害)，戰鬥開始時，給予自己2層燃燒",0},
	{13,-1,"餘燼皇冠","戰鬥開始時，給予所有敵方目標2層燃燒",0},
	{14,3,"試管架","每次消耗卡牌時，消耗1點充能抽1張牌，回合開始時獲得3點充能，上限10點",0},
	{15,-1,"藥水腰帶","戰鬥開始時，獲得3瓶可疑的藥水，回合開始時，再獲得1瓶",0},//atk(10%/30%/30%/30%)(0,1,2,3) def(20%/40%/30%/10%)(0,1,2,3)
	{16,0,"全像之鏡","每打出7張牌，複製當前使用的牌",0},
	{17,3,"磨刀石","每打出9張牌，提升當前使用的牌的攻擊力，最多3次",0},
	{18,0,"隨身口袋","每打出3張牌，抽1張牌",0},
	{19,-1,"輕盈皮靴","戰鬥開始時，獲得4張連擊，回合開始時，再獲得1張",0},
	{20,-1,"憤怒指環","戰鬥開始時，獲得3層狂怒",0},
	{21,2,"力量藥水","戰鬥開始時，前2次攻擊傷害提升",0},
	{22,-1,"鋼鐵堅盾","回合開始時，獲得2層防禦",0},
	{23,-1,"不穩定核心","受到怪物傷害時，對所有敵人造成2點傷害",0},
	{24,0,"永恆之盾","防禦力能保留66%到下個回合",0},
	{25,-1,"借據","前三回合行動值-1，之後行動值+1",0},
	{26,-1,"孫子兵法","第一回合有3倍行動值",0},
	{27,4,"夜明珠","前四回合怪物無法給予負面效果",0},
	{28,-1,"無暇寶鑽","玩家回合結束時，手中每有1張牌給予1點防禦",0},
	{29,0,"能量電池","所有剩餘行動點將保留至下回合，第一回合行動點+2",0},
	{30,-1,"黑暗寶典","進入戰鬥時，所有生物減少20%當前生命值",0},
	{31,-1,"深淵寶典","能斬殺生命值低於10%的敵人",0},
	{32,-1,"惡魔寶典","出牌時有5%概率使基本攻擊力提升至400%",0},
};

void relic_effect(int timing){
	if(timing==when_start_fight){ //開始戰鬥時 
		if(relic[1].have) relic[1].count=5;
		if(relic[2].have) relic[2].count=10;
		if(relic[3].have){
			relic[3].count=3;
			add_deck_skill(4);
			add_deck_skill(4);
		} 
		if(relic[4].have) relic[4].count=1;
		if(relic[5].have) relic[5].count=7;
		if(relic[6].have) relic[6].count=0;
		if(relic[11].have) healing(2);
		if(relic[12].have) player_effect.fire+=2;
		if(relic[13].have){
			monster_effect[0].fire+=2;
			monster_effect[1].fire+=2;
			monster_effect[2].fire+=2;
		}
		if(relic[14].have) relic[14].count=0;
		if(relic[15].have){
			int i;
			int random;
			for(i=0;i<2;i++){
				add_deck_skill(7);
				random=rand() %10;
				if(random>=7) skill_deck[deck_num-1].atk+=3; 
				else if(random>=4) skill_deck[deck_num-1].atk+=2;
				else if(random>=1) skill_deck[deck_num-1].atk+=1;
				random=rand() %10;
				if(random==9) skill_deck[deck_num-1].def+=3; 
				else if(random>=6) skill_deck[deck_num-1].def+=2;
				else if(random>=2) skill_deck[deck_num-1].def+=1;
			}
		}
		if(relic[16].have) relic[16].count=0;
		if(relic[17].have) relic[17].count=3;
		if(relic[18].have) relic[18].count=0;
		if(relic[19].have){
			add_deck_skill(4);
			add_deck_skill(4);
			add_deck_skill(4);
		}
		if(relic[20].have) player_effect.rage+=3;
		if(relic[21].have) relic[21].count=2;
		if(relic[24].have) relic[24].count=0;
		if(relic[27].have) relic[27].count=4;
		if(relic[29].have) relic[29].count=2;
		if(relic[30].have){
			monster[0].health=monster[0].health*4/5;
			monster[1].health=monster[1].health*4/5;
			monster[2].health=monster[2].health*4/5;
			printf("黑暗寶典發動！降低了敵方生命值！\n", relic[24].count);
        	Sleep(800);
		}
	}
	if(timing==when_start_round){  //開始玩家回合  
		if(relic[6].have){
			player.def+=relic[6].count*2;
			printf("聖騎士紋章發動！獲得%d點防禦！\n", relic[6].count*2);
			relic[6].count=0;
		}
		if(relic[14].have){
			relic[14].count+=3;
			if(relic[14].count > 10) relic[14].count = 10;
		} 
		if(relic[15].have){
			int i;
			int random;
			for(i=0;i<1;i++){
				add_deck_skill(7);
				random=rand() %10;
				if(random>=7) skill_deck[deck_num-1].atk=3; 
				else if(random>=4) skill_deck[deck_num-1].atk=2;
				else if(random>=1) skill_deck[deck_num-1].atk=1;
				random=rand() %10;
				if(random==9) skill_deck[deck_num-1].def=3; 
				else if(random>=6) skill_deck[deck_num-1].def=2;
				else if(random>=2) skill_deck[deck_num-1].def=1;
			}
		}
		if(relic[19].have){
			add_deck_skill(4);
		}
		if(relic[22].have) player.def+=2;
		if(relic[24].have){
			player.def+=relic[24].count;
			printf("永恆之盾發動！繼承了%d點防禦！\n", relic[24].count);
        	Sleep(800);
			relic[24].count=0;
		} 
		if(relic[25].have){
			if(turn_count>3){
				player.move_point++;
				printf("借據發動！行動值+1！\n");
        		Sleep(800);
			}
			else{
				player.move_point--;
				printf("借據發動！行動值-1！\n");
        		Sleep(800);
			}
		}
		if(relic[26].have){
			if(turn_count==1){
				player.move_point+=player.max_move_point*2;
				printf("孫子兵法發動！行動值提升！\n");
        		Sleep(800);
			}
		}
		if(relic[29].have){
			player.move_point+=relic[29].count;
			printf("能量電池發動！額外保留了%d點行動值！\n",relic[29].count);
			relic[29].count=0;
        	Sleep(800);
		}
	}
	if(timing==when_attack){  //攻擊時(與怒氣並行) 
		if(relic[1].have){
			if(skill_list[choice].atk > 0 && relic[1].count > 0 && player_effect.rage>0){
			printf("戰士紋章發動！(剩餘充能: %d)\n", relic[1].count);	
			healing(2);
			relic[1].count-=1;
			} 
		}
		if(relic[2].have){
			if(monster_effect[aim-1].fire==0 && relic[2].count>0){
				monster_effect[aim-1].fire+=1;
				relic[2].count--;
				printf("火焰法師紋章發動！點燃了%s%d！(剩餘充能: %d)\n", monster[aim-1].name,aim,relic[2].count);
			}
		}
		if(relic[3].have){
			if(skill_list[choice].id==4 && relic[3].count>0){
				add_deck_skill(4);
				add_deck_skill(4);
				relic[3].count--;
				printf("武士紋章發動！獲得2張連擊！(剩餘充能: %d)\n", relic[3].count);
			}
		}
		if(relic[5].have){
			if(skill_list[choice].id==7 && relic[5].count>0){
				add_deck_skill(7);
				skill_deck[deck_num-1].atk=1;
				skill_deck[deck_num-1].def=1;
				skill_deck[deck_num-1].heal=1;
				relic[5].count--;
				printf("女巫紋章發動！獲得1瓶可疑的藥水！(剩餘充能: %d)\n", relic[5].count);
			}
		}
		if(relic[16].have){
			if(relic[16].count>=7){
				skill_deck[deck_num] = skill_list[choice];
				skill_deck[deck_num].exhaust = 1;
				deck_num++;
				printf("全像之鏡發動！複製了%s",skill_list[choice].name);
				Sleep(500);
				relic[16].count-=7;
			}
		}
		if(relic[17].have){
			if(skill_list[choice].atk > 0 && relic[17].count > 0 && al_use>=36-9*relic[17].count){
    			if(skill_list[choice].mul==0){
    				final_atk += 2; 
    				skill_list[choice].atk+=2; 
				}
				else{
					final_atk += 1; 
					skill_list[choice].atk+=1; 
				}
       			relic[17].count--; 
        		printf("磨刀石發動！消耗 1 點充能，此卡傷害提升！(剩餘充能: %d)\n", relic[17].count);
        		Sleep(800);
    		}
		} 
		if(relic[21].have){
			if(skill_list[choice].atk > 0 && relic[21].count > 0){
    			if(skill_list[choice].mul==0){
    				final_atk += 2; 
				}
				else{
					final_atk += 1; 
				}
       			relic[21].count--; 
        		printf("力量藥水發動！消耗 1 點充能，本次傷害提升！(剩餘充能: %d)\n", relic[21].count);
        		Sleep(800);
    		}
		} 
		if(relic[32].have){
			if(rand()%20==0&&skill_list[choice].atk > 0){
				final_atk*=5;
				printf("惡魔寶典發動！傷害提升至400%！\n");
        		Sleep(800);
			}
		}
	}
	if(timing==when_be_attack){  //被怪物攻擊且受到傷害時 
		if(relic[6].have) relic[6].count++;
		if(relic[23].have){
			int i;
			for(i=0;i<3;i++){
				monster[i].def-=2;
    			if(monster[i].def<0){
    				if(monster[i].health>0){
    					printf("不穩定核心發動！對%s%d造成了%d點額外傷害\n",monster[i].name,i+1,-monster[i].def);
    					Sleep(400);
					}
					monster[i].health += monster[i].def;
					monster[i].def = 0;
				}
			}
		} 
	}
	if(timing==when_round_over){  //玩家計算dot時(優先於dot) 
		if(relic[12].have){
			if(player_effect.fire>0){
				printf("鳳凰羽毛發動！\n");
				healing(4);
			}	
		}
		if(relic[28].have){
			if(get_hand_count()>0){
				printf("無暇寶鑽發動！獲得%d點防禦\n",get_hand_count());
				player.def+=get_hand_count();
			}	
		}
	}
	if(timing==when_used_skill){  //於用完技能時(可用於抽牌數判定)
		if(relic[4].have){
			if(relic[4].count>0 && player.move_point==0){
				reset_deck();
				discard_list();
				pull_skill(player.pull_num-2);
				player.move_point+=3;
				relic[4].count--;
				printf("刺客紋章發動！重新獲得卡牌並獲得3點充能！(剩餘充能: %d)\n", relic[4].count);
			}
		}
		if(relic[16].have)relic[16].count++;
		if(relic[18].have){
			relic[18].count++;
			if(relic[18].count>=3){
				pull_skill(1);
				printf("隨身口袋發動！再抽1張牌\n");
				Sleep(500);
				relic[18].count-=3;
			} 
		}
		if(relic[31].have){
			int i;
			for(i=0;i<3;i++){
				if(monster[i].health<monster[i].max_health/10){
					monster[i].health=0;
					printf("深淵寶典發動！斬殺了%s%d！\n",monster[i].name,i+1);
					Sleep(500);
				} 
			}
		}
	}
	if(timing==when_card_discard){  //主動消耗卡牌時 
		if(relic[14].have){
			if(relic[14].count>0){
				pull_skill(1);
				relic[14].count--;
			}
		}
	}
	if(timing==when_monster_round_over){//怪物回合結束時 
		if(relic[24].have) relic[24].count=player.def*2/3;
		if(relic[27].have){
			if(relic[27].count>0){
				player_effect.fire=0;
				player_effect.bleed=0;
				player_effect.deride=0;
				player_effect.poison=0;
				printf("夜明珠發動！免疫負面效果\n");
				Sleep(500);
				relic[27].count--;
			}
		} 
		if(relic[29].have){
			if(player.move_point>1){
				relic[29].count=player.move_point;
				player.move_point=1;
			}
		}
	}
}

// 透過卡牌 ID 在 skill_db 中尋找正確的卡牌並印出資訊
void print_skill_info(int target_id) {
    int j;
    // 假設 skill_db 的總大小是 100
    for (j = 0; j < 200; j++) { 
        if (skill_db[j].id == target_id) {
            // 找到了！印出該陣列位置(j)的內容
            printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d) \n    描述: %s\n", 
                skill_db[j].id, skill_db[j].name, skill_db[j].cost, 
                skill_db[j].atk, skill_db[j].def, skill_db[j].heal, skill_db[j].info);
            return; // 印完就跳出函式
        }
    }
    // 如果迴圈跑完都沒找到，印出錯誤提示防呆
    printf("[%d] 找不到對應的卡牌資料！\n", target_id); 
}

void pre_list(){
    int pre_i;
    for (pre_i=choice;pre_i<20;pre_i++) {
        skill_list[pre_i]=skill_list[pre_i+1];
    }
    skill_list[20].id=0;
}

int get_hand_count() {
    int count = 0;
    while (count < 20 && skill_list[count].id != 0) {
        count++;
    }
    return count;
}

int choice_skill_list() {
    int i;
    int list_num = get_hand_count();
    printf("請選擇一張卡牌\n");
    for (i = 0; i < list_num; i++) {
        printf("[%d] %s\n", i, skill_list[i].name); 
    }
    
    int input;
    while (1) {
        scanf("%d", &input); 
        if (input < list_num && input >= 0) { 
            printf("你選擇了 %s\n", skill_list[input].name); 
            return input;
        } 
		else {
            printf("輸入錯誤，請重試！\n"); 
        }
    } 
}

void starting(){
    player.def=0;
    player.health=40;
    player.max_health=40;
    player.max_move_point=3;
    player.pull_num=6;
    over=0;
    die=0;
    locate=0;
    skill_num=0;
    memset(skill_list, 0, sizeof(skill_list));
    add_skill(1);
    add_skill(1); 
    add_skill(1); 
    add_skill(2);
    add_skill(2); 
    add_skill(2);
}

void monster_special_effect(){
	switch(monster[move_i].id){
		case(10):
			monster[move_i].health+=3;
			if(monster[move_i].health>monster[move_i].max_health){
				monster[move_i].health=monster[move_i].max_health;
			}
			break;
		case(11):
			add_deck_skill(5);
			if(player.move_point==0){
				add_deck_skill(5);
				add_deck_skill(5);
				add_discard_skill(5);
			}
			else{
				player.move_point-=1;
			}
			printf("你被蛛網困住了\n");
			Sleep(500);
			break;
		case(12):
			if(player.def==0){
				add_deck_skill(6);
				printf("你受重傷了\n");
				Sleep(500);
			}
			break;
		case(13):{
				int summon_k;
				for (summon_k = 0; summon_k < 3; summon_k++) {
					if (monster[summon_k].health <= 0) {
						monster[summon_k] = monster_db[13];
						printf("%s%d 分裂了！\n", monster[move_i].name,move_i+1);
						monster[summon_k].health=monster[move_i].health;
						memset(&monster_effect[summon_k], 0, sizeof(monster_effect[summon_k]));
						Sleep(500);
						break; 
					}
				}
			}
			break;
		case(14):
			monster[move_i].health+=4;
			monster_effect[move_i].fire=0;
			if(monster[move_i].health>monster[move_i].max_health){
				monster[move_i].health=monster[move_i].max_health;
			}
			player_effect.fire+=1;
			printf("你被焦屍灼傷了\n");
			Sleep(500);
			break;
		case(15):
			add_deck_skill(5);
			if(player.move_point==0){
				add_deck_skill(5);
				add_deck_skill(5);
				add_discard_skill(5);
			}
			else{
				player.move_point-=1;
			}
			printf("你被蛛網困住了\n");
			Sleep(500);
			if(rand() %3 ==0){
				player_effect.poison+=3;
				printf("你中毒了\n");
			Sleep(500);
			}
			break;
		case(16):
			if(player.def==0){
				add_deck_skill(6);
				add_discard_skill(6);
				player_effect.bleed+=3;
				printf("你受重傷了\n");
				Sleep(500);
			}
			break;
		case(17):{
				int summon_k;
				switch(turn_count%4+1){
					
					case(1): //召喚 
						for (summon_k = 0; summon_k < 3; summon_k++) {
								if (monster[summon_k].health <= 0) {
									monster[summon_k] = monster_db[rand()%6+10];
									memset(&monster_effect[summon_k], 0, sizeof(monster_effect[summon_k]));
									printf("%s 召喚了 %s！\n", monster[move_i].name, monster[summon_k].name);
									break; 
								}
							}
						break;
					case(2): //點燃 
						player_effect.fire+=2;
						printf("你被荒野領主的火球擊中了\n");
						break;
					case(3): //防禦 
						monster[0].def+=3;
						monster[1].def+=6;
						monster[2].def+=3;
						printf("荒野領主保護了他與他的夥伴們\n");
						break;
					case(4): //攻擊 
						player.def-=4;
                        	if(player.def<0){
								player.health+=player.def;
								printf("你受到了 %d 點傷害\n",0-player.def);
								relic_effect(when_be_attack);
								player.def=0;
							}
							else{
							printf("你格擋了 4 點傷害\n");
							}
						break;
				}
			}
			break;	
		case(20):
			player_effect.deride=move_i;
			break;
		case(22):
			if(monster[0].health*2>monster[0].max_health&&monster[2].health*2>monster[2].max_health){
				int i;
				for( i = 0; i < 3; i++){
					if(monster[i].id != 0 && monster[i].health > 0){
						monster[i].def += monster[move_i].atk;
					}
				}
				printf("%s%d給予了所有隊友%d層防禦\n", monster[move_i].name, move_i+1, monster[move_i].atk);
			}
			else if(monster[0].health>0||monster[2].health>0){
				int target = -1;
				int min_health = 9999;
				int i ;
				for(i = 0; i < 3; i++){
					if(monster[i].id != 0 && monster[i].health > 0 && monster[i].health < monster[i].max_health){
						if(monster[i].health < min_health){
							min_health = monster[i].health;
							target = i;
						}
					}
				}
				if(target != -1){
					monster[target].health += monster[move_i].atk;
					if(monster[target].health > monster[target].max_health) monster[target].health = monster[target].max_health;
					printf("%s%d治療了%s%d\n", monster[move_i].name, move_i+1, monster[target].name, target+1);
				}
			}
			else{
				player.def-=6;
                if(player.def<0){
					player.health+=player.def;
					printf("你受到了 %d 點傷害\n",0-player.def);
					player.def=0;
					relic_effect(when_be_attack);
				}
				else{
					printf("你格擋了 %d 點傷害\n",monster[move_i].atk);
				}
			}
			break;
		case(23):{
			    if(monster[move_i].health<=monster[move_i].max_health/2&&monster[move_i].atk<10){
			    	printf("%s%d用翅膀保護起自己 能量從他體內湧出\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].def=15;
					monster[move_i].atk=10; 
				}
				else if(monster[move_i].atk==10){
					printf("%s%d體內的能量逐漸提升\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].def=10;
					monster[move_i].atk=11; 
				}
				else if(monster[move_i].atk==11){
					printf("%s%d累積的能量已接近臨界點\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].def=5;
					monster[move_i].atk=12; 
				}
				else if(monster[move_i].atk==12){
					printf("%s%d已蓄力完畢 準備釋放深淵崩壞 將造成大量傷害\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].def=0;
					monster[move_i].atk=13; 
				}
				else if(monster[move_i].atk==13){
					printf("%s%d釋放了深淵崩壞\n", monster[move_i].name, move_i+1);
					Sleep(800);
					player.def-=40;
                    if(player.def<0){
						player.health+=player.def;
						printf("你受到了 %d 點傷害\n",0-player.def);
						relic_effect(when_be_attack);
						player.def=0;
					}
					else{
						printf("你格擋了40點傷害\n");
					}
					add_deck_skill(9);
					add_deck_skill(9);
					monster[move_i].atk=14;
				}
				else if(monster[move_i].atk==14){
					printf("%s%d體力耗盡 無法行動\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].atk=15;
				}
				else if(monster[move_i].atk==15){
					printf("%s%d體力回復了\n", monster[move_i].name, move_i+1);
					Sleep(800);
					printf("%s%d進入了狂暴狀態\n", monster[move_i].name, move_i+1);
					Sleep(800);
					monster[move_i].atk=99;
				}
				else if(player_effect.fire==0&&monster[move_i].atk!=1){
					printf("%s%d噴射了高溫的龍息\n", monster[move_i].name, move_i+1);
					Sleep(800); 
					player.def-=3;
                    if(player.def<0){
						player.health+=player.def;
						printf("你受到了 %d 點傷害\n",0-player.def);
						relic_effect(when_be_attack);
						player.def=0;
					}
					else{
						printf("你格擋了 3 點傷害\n");
					}
					Sleep(800); 
					printf("你獲得了5層燃燒\n");
					Sleep(800); 
					player_effect.fire+=5;
					if(monster[move_i].atk!=99) monster[move_i].atk=1;
				}
				else if(monster[move_i].atk!=2&&monster[move_i].atk!=99&&rand()%5==0){
					printf("%s%d使用了黑暗咒法\n", monster[move_i].name, move_i+1);
					Sleep(800); 
					printf("你被詛咒了\n");
					Sleep(800); 
					printf("獲得了1張深淵詛咒\n");
					Sleep(800);
					add_deck_skill(9);
					monster[move_i].atk=2; 
				}
				else if(monster[move_i].atk==99&&rand()%5==0){
					printf("%s%d用龍爪向你攻擊\n", monster[move_i].name, move_i+1);
					Sleep(800); 
					player.def-=3;
                    if(player.def<0){
						player.health+=player.def;
						printf("你受到了 %d 點傷害\n",0-player.def);
						relic_effect(when_be_attack);
						player.def=0;
						Sleep(800);
						printf("你受到了嚴重的撕裂傷\n");
						player_effect.bleed+=7;
					}
					else{
						printf("你格擋了 3 點傷害\n");
					}
					Sleep(800); 
				}
				else if(rand()%4<2){
					printf("%s%d的尾巴甩向了你\n", monster[move_i].name, move_i+1);
					Sleep(800); 
					player.def-=7;
                    if(player.def<0){
						player.health+=player.def;
						printf("你受到了 %d 點傷害\n",0-player.def);
						relic_effect(when_be_attack);
						player.def=0;
					}
					else{
						printf("你格擋了 7 點傷害\n");
					}
					Sleep(800); 
					if(monster[move_i].atk!=99) monster[move_i].atk=0;
				}
				else if(monster[move_i].atk==99){
					printf("%s%d憤怒的向你咆嘯\n", monster[move_i].name, move_i+1);
					player.def-=3;
                    if(player.def<0){
						player.health+=player.def;
						printf("你受到了 %d 點傷害\n",0-player.def);
						relic_effect(when_be_attack);
						player.def=0;
					}
					else{
						printf("你格擋了 3 點傷害\n");
					}
					Sleep(800); 
					Sleep(800); 
				}
				else{
					printf("%s%d感到了疲憊\n", monster[move_i].name, move_i+1);
					Sleep(800);
				}
			} 
			break;
	}
}

void monster_move(){
    can_act[0] = false;
    can_act[1] = false;
    can_act[2] = false;
    for(move_i = 0; move_i < 3; move_i++){
        if(monster[move_i].health > 0){
            can_act[move_i] = true;
        }
    }
    for(move_i=0;move_i<3;move_i++){
        if(monster[move_i].health>0&&can_act[move_i]==true){
            if ((turn_count+monster[move_i].ex_speed) % monster[move_i].speed == 0) {
                printf("%s 的回合\n",monster[move_i].name);
                Sleep(800);
                switch(monster[move_i].move){
                    case(1):
                        player.def-=monster[move_i].atk;
                        if(player.def<0){
							player.health+=player.def;
							printf("你受到了 %d 點傷害\n",0-player.def);
							player.def=0;
							relic_effect(when_be_attack);
						}
						else{
							printf("你格擋了 %d 點傷害\n",monster[move_i].atk);
						}
						break;
					case(2): // 防禦 (給盾)
						if(monster[move_i].mul == 0){ // 單體給盾
							int target;
							int try_count = 0;
							// 確保選到活著的怪物 (加入防呆機制避免死迴圈)
							do {
								target = rand() % 3;
								try_count++;
								if(try_count > 10) break; 
							} while(monster[target].id == 0 || monster[target].health <= 0);
							
							if(monster[target].health > 0) {
								monster[target].def += monster[move_i].atk;
								printf("%s%d保護了%s%d\n", monster[move_i].name, move_i+1, monster[target].name, target+1);
							}
						}
						else{ // 全體給盾
							int i;
							for( i = 0; i < 3; i++){
								if(monster[i].id != 0 && monster[i].health > 0){
									monster[i].def += monster[move_i].atk;
								}
							}
							printf("%s%d給予了所有隊友%d層防禦\n", monster[move_i].name, move_i+1, monster[move_i].atk);
						}
						break;

					case(3):{ // 治療
						// 先判斷：是不是所有「活著」的怪物都已經滿血了？
						bool need_heal = false;
						int i;
						for(i = 0; i < 3; i++){
							if(monster[i].id != 0 && monster[i].health > 0 && monster[i].health < monster[i].max_health){
								need_heal = true; // 只要有一隻活著且沒滿血，就需要治療
								break;
							}
						}
						if(!need_heal) {
							printf("%s%d想嘗試治療隊友，但沒人需要治療\n", monster[move_i].name, move_i+1);
						}
						else if(monster[move_i].mul == 0){ // 單體治療 (尋找活著且血量最少的)
							int target = -1;
							int min_health = 9999;
							int i ;
							for(i = 0; i < 3; i++){
								// 必須活著、且不是滿血，才有資格被挑選
								if(monster[i].id != 0 && monster[i].health > 0 && monster[i].health < monster[i].max_health){
									if(monster[i].health < min_health){
										min_health = monster[i].health;
										target = i;
									}
								}
							}
							if(target != -1){
								monster[target].health += monster[move_i].atk;
								if(monster[target].health > monster[target].max_health) monster[target].health = monster[target].max_health;
								printf("%s%d治療了%s%d\n", monster[move_i].name, move_i+1, monster[target].name, target+1);
							}
						}
						else{ // 全體治療
							int i;
							for(i = 0; i < 3; i++){
								// 只治療活著且沒滿血的
								if(monster[i].id != 0 && monster[i].health > 0 && monster[i].health < monster[i].max_health){
									monster[i].health += monster[move_i].atk;
									if(monster[i].health > monster[i].max_health) monster[i].health = monster[i].max_health;
									printf("%s%d治療了%s%d\n", monster[move_i].name, move_i+1, monster[i].name, i+1);
								}
							}
						}
						}
						break;
					case(4): { 
						int summon_id = monster[move_i].atk; 
						int summon_k;
						int summoned_count = 0; 
						if (monster[move_i].mul == 0) {
							for (summon_k = 0; summon_k < 3; summon_k++) {
								if (monster[summon_k].health <= 0) {
									monster[summon_k] = monster_db[summon_id];
									memset(&monster_effect[summon_k], 0, sizeof(monster_effect[summon_k]));
									printf("%s 召喚了 %s！\n", monster[move_i].name, monster[summon_k].name);
									summoned_count++;
									break; 
								}
							}
						} 
						else {
							for (summon_k = 0; summon_k < 3; summon_k++) {
								if (monster[summon_k].health <= 0) {
									monster[summon_k] = monster_db[summon_id];
									memset(&monster_effect[summon_k], 0, sizeof(monster_effect[summon_k]));
									summoned_count++;
								}
							}
							if (summoned_count > 0) {
								printf("%s進行了軍團召喚，生成了%d隻 %s！\n", 
									   monster[move_i].name, summoned_count, monster_db[summon_id].name);
							}
						}
						if (summoned_count == 0) {
							printf("%s嘗試召喚，但場上已經沒有空位了！\n", monster[move_i].name);
						}
						break;
					}
				}
				monster_special_effect();
				Sleep(800);
			}
			else {
				printf("%s 正在蓄力中...\n", monster[move_i].name);
				Sleep(800);
			}
		}
	}
}

void special_effect(){
	switch(skill_list[choice].tag){
		case(1):
			if(rand() %10<6)pull_skill(1);
			break;
		case(2):{
			int i=rand() %2;
			if(i==0){
				monster[aim-1].def-=(final_atk*2/3);
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("爆擊! 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(400);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
			}
		} 	
		break;
		case(3):
			add_discard_skill(4);
			add_discard_skill(4);
			break;
		case(4):{ 
            	add_discard_skill(skill_list[choice].id);
            	char temp_name[100];
        		snprintf(temp_name, sizeof(temp_name), "被複製的%s",discard_pile[discard_num-1].name);
        		strcpy(discard_pile[discard_num-1].name, temp_name);
        		char temp_info[200];
        		snprintf(temp_info, sizeof(temp_info), "%s，此牌使用後會被消耗", discard_pile[discard_num-1].info);
        		strcpy(discard_pile[discard_num-1].info, temp_info);
            	discard_pile[discard_num-1].exhaust = 1;
            } 
            break;
		case(5):
      		skill_list[choice].tag = -5;  // 變成負數，下一次就不會再觸發此 case
        	special_move = 1;             // 扣留在手牌，不進入棄牌流向
        	break; 
        case(6):
        	monster_effect[aim-1].fire+=2;
        	break;
        case(7):
        	player_effect.rage+=1;
        	break;
	}
	switch(skill_list[choice].id){
		case(4):
			pull_skill(1);
			break;
		case(6):
			player.health-=3;
			pull_skill(1);
			break;
		case(19):
			player_effect.poison=0;
			player_effect.regenerate+=1;
			break;
		case(20):
			monster_effect[aim-1].poison+=4;
			break;
		case(21):
			add_discard_skill(22);
			break;
		case(22):
			add_discard_skill(23);
			break;
		case(23):
			add_discard_skill(137);
			break;
		case(24):
			pull_skill(1);
			break;
		case(27):
			if(player.def==0){
				monster[aim-1].def-=2;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("對%s%d造成了%d點額外傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(400);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}	
			}
			break;
		case(32):
			player_effect.rage+=2;
			break;
		case(34):
			pull_skill(2);
			break;
		case(35):
			player_effect.power+=2;
			break;
		case(36):
            player_effect.fire+=1;
            player_effect.rage+=1;
            break;
		
		case(39):
			player.move_point+=3;
			break;
		case(47):{
        	int current_hand = get_hand_count(); // 1. 先把當前的手牌數量存起來！
        	discard_list();                      // 2. 把所有手牌棄置（此時手牌數歸零）
        	pull_skill(current_hand);            // 3. 依照剛剛記下來的數量抽牌
        	special_move = 1;                    // 4. 告訴系統這張卡已經自行處理棄牌了，不要重複結算
    		}
    		break;
    	case(48):{
				int i;
				int random;
				for(i=0;i<5;i++){
					random=rand() %3;
					monster[random].def-=1;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(400);
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
				}
			}
			break;
		case(49):
			player_effect.regenerate+=1;
			break;
		case(52):
			player.def+=player.move_point*5;
			player.move_point=0;
			break;
		case(53):
            add_discard_skill(3);
            add_discard_skill(3);
            add_discard_skill(3);
            add_discard_skill(3);
            break;
        case(56):
        	memset(&player_effect, 0, sizeof(player_effect));
        	break;
		case(57):
			monster_effect[0].fire+=2;
			monster_effect[1].fire+=2;
			monster_effect[2].fire+=2;
			break;
		
		//職業專屬分隔線 
		case(101):
			player_effect.regenerate+=player_effect.rage;
			player_effect.rage=0;
			break;
		case(102):
			skill_list[choice_skill_list()].atk+=1;
			break;
		case(103):
			healing(final_atk/2);
			break;
		case(104):{
				monster[aim-1].def-=final_atk/2;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    						printf("二連擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    						Sleep(800);
						}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
			}
			break;
		case(105):
			if(monster[aim-1].health<final_atk){
				printf("爆擊！ %s%d被斬殺了\n",monster[aim-1].name,aim);
				monster[aim-1].health=0;
			}
			break;
		case(106):
			monster[aim-1].def-=player_effect.rage*3;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("憤怒一擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim-1,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;	
			}
			else{
				printf("%s%d格擋了%d點傷害\n",monster[aim-1].name,aim-1,player_effect.rage*3);
    			Sleep(500);
			}
			player_effect.rage=0;
			break;
		case(107):
        	monster[aim-1].def-=final_atk;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("二連擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			monster[aim-1].def-=final_atk;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("三連擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			break;
		case(108):
			monster[aim-1].def-=final_atk;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("爆擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			break;
		case(109):{
				int target,i;
				for(i=0;i<1;){
					target=3;
					if(monster[0].health<=monster[1].health&&monster[0].health<=monster[2].health&&monster[0].health>0) target=0;
					if(monster[1].health<=monster[0].health&&monster[1].health<=monster[2].health&&monster[1].health>0) target=1;
					if(monster[2].health<=monster[0].health&&monster[2].health<=monster[1].health&&monster[2].health>0) target=2;
					if(target==3)break;
					monster[target].def-=final_atk*3/2;
    				if(monster[target].def<0){
    					if(monster[target].health>0){
    							printf("折返！ 對%s%d造成了%d點傷害\n",monster[target].name,target,-monster[target].def);
    							Sleep(500);
							}
						monster[target].health += monster[target].def;
						monster[target].def = 0;	
					}
					else{
						printf("%s%d格擋了%d點傷害\n",monster[target].name,target,final_atk*3/2);
    					Sleep(500);
					}
					if(monster[target].health>0)i++;
				}
			}
			break;
		case(110):{
			int i=0;
			int round=0;
			while(round<4&&(monster[0].health>0||monster[1].health>0||monster[2].health>0)){
				if(monster[i%3].health>0){
					monster[i%3].def-=final_atk;
    				if(monster[i%3].def<0){
    					if(monster[i%3].health>0){
    							printf("亂蝶！ 對%s%d造成了%d點傷害\n",monster[i%3].name,i%3+1,-monster[i%3].def);
    							Sleep(500);
							}
						monster[i%3].health += monster[i%3].def;
						monster[i%3].def = 0;
						
					}
					else{
						printf("%s%d格擋了%d點傷害\n",monster[i%3].name,i%3+1,final_atk);
    					Sleep(500);
					}
					round++;
				}
				i++;
			}
			}
			break;
		case(111):
        	monster_effect[aim-1].fire+=2;
        	break;
        case(112):
        	monster[aim-1].health-=monster_effect[aim-1].fire*2;
        	printf("爆燃！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,monster_effect[aim-1].fire*2);
			monster_effect[aim-1].fire=0;
			Sleep(400); 
        	break;
        case(113):
        	player.def+=player_effect.fire*2;
        	player_effect.fire=0;
        	break;
        case(114):
			monster_effect[0].fire+=1;
			monster_effect[1].fire+=1;
			monster_effect[2].fire+=1;
			break;
		case(115):
			player_effect.fire+=2;
			break;
		case(116):
			monster[aim-1].def-=player_effect.fire;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("融斷！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim-1,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;	
			}
			else{
				printf("%s%d格擋了%d點傷害\n",monster[aim-1].name,aim-1,player_effect.fire);
    			Sleep(500);
			}
			if(player_effect.fire>0) player_effect.fire--;
			break;
		case(117):
			player_effect.regenerate+=player_effect.fire;
			player_effect.fire=0;
			break;
		case(118):{ 
				int i;
				for(i=1;i<3;i++){
					monster[i].health-=monster_effect[i].fire*2;
        			printf("聚爆！ 對%s%d造成了%d點傷害\n",monster[i].name,i+1,monster_effect[i].fire*2);
					monster_effect[i].fire=0;
					Sleep(400); 
				}
			}
			break;
		case(119):{
				int choice=choice_skill_list();
				int i=0;
				while(skill_db[i].id!=skill_list[choice].id){
					i++;
				}
				strcpy(skill_list[choice].name,skill_db[i].name);
				strcpy(skill_list[choice].info,skill_db[i].info);
				char temp_name[100];
        		snprintf(temp_name, sizeof(temp_name), "高溫的%s",skill_list[choice].name);
        		strcpy(skill_list[choice].name, temp_name);
        		char temp_info[200];
        		snprintf(temp_info, sizeof(temp_info), "%s，此牌會點燃主目標", skill_list[choice].info);
        		strcpy(skill_list[choice].info, temp_info);
        		skill_list[choice].tag=6; 
			}
			break;
		case(120):{
			int i=0;
			int round=0;
			while(round<9&&(monster[0].health>0||monster[1].health>0||monster[2].health>0)){
				if(monster[i%3].health>0){
					monster[i%3].def-=3;
    				if(monster[i%3].def<0){
    					if(monster[i%3].health>0){
    							printf("對%s%d造成了%d點傷害\n",monster[i%3].name,i%3+1,-monster[i%3].def);
    							Sleep(500);
							}
						monster[i%3].health += monster[i%3].def;
						monster[i%3].def = 0;
						
					}
					else{
						printf("%s%d格擋了3點傷害\n",monster[i%3].name,i%3+1);
    					Sleep(500);
					}
					monster_effect[i%3].fire+=2;
					round++;
				}
				i++;
			}
			}
			break;
		case(121):{
				int i;
				int random;
				for(i=0;i<2+(al_use)/4;i++){
					random=rand() %3;
					monster[random].def-=2;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(1000/(2+(al_use)/4));
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
				}
			} 
			break;
		case(122):
			add_deck_skill(4);
			add_deck_skill(4);
			break;
		case(123):
			monster[aim-1].def-=al_use/5;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("重擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			else{
				printf("%s%d格擋了%d點傷害\n",monster[aim-1].name,aim-1,al_use/5);
    			Sleep(500);
			}
			healing(al_use/4);
			break;
		case(124): {
    		int num = 0;
    		int i;
    		int pre_choice = choice;
    		int need = get_hand_count();
    		for (i = need - 1; i >= 0; i--) {
        		if (skill_list[i].id == 4) {
            		choice = i;
           			num++;
            		add_discard_skill(4); 
            		if (i < pre_choice) {
                		pre_choice--; 
            		}
            		pre_list();
        		}
    		}
    		choice = pre_choice;
    		if (num > 0) {
        		monster[aim-1].def -= num;
        		if (monster[aim-1].def < 0) {
            		if (monster[aim-1].health > 0) {
                		printf("追加攻擊！消耗了 %d 張連擊，對%s%d造成了%d點額外傷害！\n", num, monster[aim-1].name, aim, -monster[aim-1].def);
                		Sleep(500);
            		}
            		monster[aim-1].health += monster[aim-1].def;
            		monster[aim-1].def = 0;
       			} 
				else {
            		printf("%s%d格擋了%d點額外傷害\n", monster[aim-1].name, aim, num);
            		Sleep(500);
        		}
    		}	
			}
			break;
		case(125):
			if(al_use<24) monster[aim-1].def-=al_use/4;
			else monster[aim-1].def-=6;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("重擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			break;
		case(126):
			add_hand_skill(25);
			add_hand_skill(25);
			break;
		case(127):{
				int i;
				for(i=0;i<(int)sqrt(al_use);i++){
					add_deck_skill(4);
				}
			}
			break;
		case(128):
			pull_skill(1);
			if(skill_list[choice].cost==0){
				if(rand() %4==0){
					skill_list[choice].cost++;
				}
			}
			break;
		case(129):
			if(al_use>=50){
				monster[aim-1].def-=final_atk*7/2;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("爆擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點額外傷害\n", monster[aim-1].name, aim, final_atk*7/2);
            		Sleep(500);
				}
			}
			else if(al_use>=30){
				monster[aim-1].def-=final_atk;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("爆擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點額外傷害\n", monster[aim-1].name, aim, final_atk);
            		Sleep(500);
				}
			}
			else if(al_use>=15){
				monster[aim-1].def-=final_atk/2;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("爆擊！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點額外傷害\n", monster[aim-1].name, aim, final_atk/2);
            		Sleep(500);
				}
			}
		case(130):
			if(al_use>40){
				monster[aim-1].def-=final_atk*4;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("一閃！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
			}
			else{
				monster[aim-1].def-=final_atk*(al_use/10);
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("一閃！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
			}
			break;
		case(131):
			skill_list[choice].atk+=1;
			break;
		case(132):
			skill_list[choice].def+=1;
			break;
		case(133):
			pull_skill(1);
			break;
		case(134):{
				int num=get_hand_count(),trans=choice_skill_list();
				if(num<20){
					skill_list[num]=skill_list[trans];
					skill_list[trans].exhaust=2;
					printf("你模仿了%s\n",skill_list[num].name);
					Sleep(800);
				}
				else{
					printf("手牌過多 模仿失敗\n");
					Sleep(800);
				}
			}
			break;
		case(135):
			skill_list[choice].atk+=1;
			break;
		case(136):{
				int num=get_hand_count();
				if(num<20&&discard_num>0){
					skill_list[num]=discard_pile[discard_num-1];
					discard_num--;
					printf("你再現了%s\n",skill_list[num].name);
					Sleep(800);
				}
				else{
					printf("手牌過多 再現失敗\n");
					Sleep(800);
				}
			}
			break;
		case(137):
			add_discard_skill(21);
			break;
		case(138):{
				int choice=choice_skill_list();
				int i=0;
				while(skill_db[i].id!=skill_list[choice].id){
					i++;
				}
				strcpy(skill_list[choice].name,skill_db[i].name);
				strcpy(skill_list[choice].info,skill_db[i].info);
				char temp_name[100];
        		snprintf(temp_name, sizeof(temp_name), "複現的%s",skill_list[choice].name);
        		strcpy(skill_list[choice].name, temp_name);
        		char temp_info[200];
        		snprintf(temp_info, sizeof(temp_info), "%s，此牌能打出兩次", skill_list[choice].info);
        		strcpy(skill_list[choice].info, temp_info);
        		skill_list[choice].tag=6; 
			}
			break;
		case(139):
			monster_effect[aim-1].poison+=skill_list[choice].atk*3/2;
			skill_list[choice].atk+=1;
			break;
		case(140):
			skill_list[choice].atk+=2;
			break;
		case(141):{
				int i;
				int random;
				for(i=0;i<6+(al_discard)/2;i++){
					random=rand() %3;
					monster[random].def-=1;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(2000/(6+(al_discard)/2));
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
				}
			} 
			break;
		case(142):
			if(al_discard<18)monster[aim-1].def-=al_discard/3;
			else monster[aim-1].def-=6;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    					printf("對%s%d造成了%d點額外傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(500);
					}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}
			break;
		case(143):
			discard_num++;
			relic_effect(when_card_discard);
			break;
		case(144):
			if(rand()%2==0){
				add_hand_skill(20);
			}
			else{
				add_hand_skill(19);
			}
			break;
		case(145):{
				int i=choice_skill_list(),pre_choice=choice;
				if(skill_list[i].id==7){
					add_discard_skill(7);
					discard_pile[discard_num-1].atk+=skill_list[i].atk*3;
					discard_pile[discard_num-1].def+=skill_list[i].def*3;
					discard_pile[discard_num-1].heal+=skill_list[i].heal*3;
					choice=i;
					if(i<pre_choice)pre_choice--;
					pre_list();
					choice=pre_choice;
				}
			}
			break;
		case(146):{
				int i;
				int random;
				for(i=0;i<2;i++){
					add_deck_skill(7);
					random=rand() %10;
					if(random>=7) skill_deck[deck_num-1].atk+=2; 
					else if(random>=2) skill_deck[deck_num-1].atk+=1;
					random=rand() %10;
					if(random==9) skill_deck[deck_num-1].def+=2; 
					else if(random>=3) skill_deck[deck_num-1].def+=1;
					random=rand() %10;
					if(random>=7) skill_deck[deck_num-1].heal+=1; 
				}
				
			}
			break;
		case(147):
			if(al_discard>8){
				monster_effect[aim-1].poison+=8;
				printf("%s%d獲得了8層中毒\n",monster[aim-1].name,aim);
			}
			else{
				monster_effect[aim-1].poison+=al_discard;
				printf("%s%d獲得了%d層中毒\n",monster[aim-1].name,aim,discard_num);
			}
			break;
		case(148):{
				int i;
				int random;
				player_effect.poison+=2;
				for(i=0;i<5+(al_discard)/3;i++){
					random=rand() %3;
					monster[random].def-=1;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(2000/(5+(al_discard)/3));
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
					monster_effect[random].poison+=1;
				}
			} 
			break;
		case(149):
			if(al_discard<30){
				monster[aim-1].def-=monster_effect[aim-1].poison*(al_discard+15)/15;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("腐蝕！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點傷害\n", monster[aim-1].name, aim,monster_effect[aim-1].poison*(al_discard+15)/15);
            		Sleep(500);
				}
			}
			else{
				monster[aim-1].def-=monster_effect[aim-1].poison*3;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("腐蝕！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點傷害\n", monster[aim-1].name, aim,monster_effect[aim-1].poison*3);
            		Sleep(500);
				}
			}
		case(150):{
			int i=0;
			int round=0;
			while(round<(7+al_discard/2)&&(monster[0].health>0||monster[1].health>0||monster[2].health>0)){
				if(monster[i%3].health>0){
					if(rand()%10<4) add_deck_skill(24);
					monster_effect[i%3].poison+=1;
					printf("%s%d獲得了1層中毒\n",monster[i%3].name,i%3+1);
					round++;
					Sleep(4000/(7+al_discard/2));
				}
				i++;
			}
			}
			break;
		case(151):
			if(player.def>5){
				player.def-=5;
				int i;
				int random;
				for(i=0;i<6;i++){
					random=rand() %3;
					monster[random].def-=1;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(250);
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
				}	
			}
			else{
				int i;
				int random;
				for(i=0;i<player.def+1;i++){
					random=rand() %3;
					monster[random].def-=1;
    				if(monster[random].def<0){
    					if(monster[random].health>0){
    						printf("對%s%d造成了%d點傷害\n",monster[random].name,random+1,-monster[random].def);
    						Sleep(250);
						}
						monster[random].health += monster[random].def;
						monster[random].def = 0;
					}
				}	
				player.def=0;
			}
			break;
		case(152):
			player.health-=2;
			printf("你受到了2點傷害\n");
			Sleep(500);
			break;
		case(153):
			player_effect.rage+=1;
			player.health-=2;
			printf("你受到了2點傷害\n");
			Sleep(500);
			relic_effect(when_be_attack);
			break;
		case(154):
			if(player.def>3){
				player.def-=3;
				monster[aim-1].def-=3;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("對%s%d造成了%d點額外傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(400);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}	
			}
			else{
				monster[aim-1].def-=player.def;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("對%s%d造成了%d點額外傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(400);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}	
				player.def=0;
			}
			break;
		case(155):
			monster[aim-1].def-=player.def;
    		if(monster[aim-1].def<0){
    			if(monster[aim-1].health>0){
    				printf("對%s%d造成了%d點額外傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    				Sleep(400);
				}
				monster[aim-1].health += monster[aim-1].def;
				monster[aim-1].def = 0;
			}	
			player.def=0;
			break;
		case(156):
			if(player.health<player.max_health/4){
				monster[aim-1].def-=final_atk;
    			if(monster[aim-1].def<0){
    				if(monster[aim-1].health>0){
    					printf("復仇！ 對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    					Sleep(800);
					}
					monster[aim-1].health += monster[aim-1].def;
					monster[aim-1].def = 0;
				}
				else{
					printf("%s%d格擋了%d點傷害\n", monster[aim-1].name, aim, final_atk);
            		Sleep(500);
				}
			}
			break;
		case(157):{
				int i;
				for(i=0;i<3;i++){
					monster[i].def-=player.health/8;
    				if(monster[i].def<0){
    					if(monster[i].health>0){
    						printf("猛攻！ 對%s%d造成了%d點傷害\n",monster[i].name,i+1,-monster[i].def);
    						Sleep(500);
						}
						monster[i].health += monster[i].def;
						monster[i].def = 0;
					}
					else{
						printf("%s%d格擋了%d點傷害\n", monster[i].name,i+1, player.health/8);
            			Sleep(500);
					}
				}
				player.health/=8;
			}
			break;
		case(158):{
				int i;
				int damage;
				for(i=0;i<3;i++){
					damage=rand()%5+2;
					monster[i].def-=damage;
    				if(monster[i].def<0){
    					if(monster[i].health>0){
    						printf("癲狂！ 對%s%d造成了%d點傷害\n",monster[i].name,i+1,-monster[i].def);
    						Sleep(500);
						}
						monster[i].health += monster[i].def;
						monster[i].def = 0;
					}
					else{
						printf("%s%d格擋了%d點傷害\n", monster[i].name,i+1,damage);
            			Sleep(500);
					}
					player.health-=1;
					printf("你受到了1點傷害\n");
					Sleep(500);
					relic_effect(when_be_attack);
				}
			}
			break;
		case(159):
			healing(player.def/3);
			player.def/=3;
			break;
		case(160):{
				int i,num=0,damage;
				if(monster[0].health>0)num++;
				if(monster[1].health>0)num++;
				if(monster[2].health>0)num++;
				damage=(player.max_health-player.health)/num;
				for(i=0;i<3;i++){
					damage=rand()%5+2;
					monster[i].def-=damage;
    				if(monster[i].def<0){
    					if(monster[i].health>0){
    						printf("星裂！ 對%s%d造成了%d點傷害\n",monster[i].name,i+1,-monster[i].def);
    						Sleep(500);
						}
						monster[i].health += monster[i].def;
						monster[i].def = 0;
					}
					else{
						printf("%s%d格擋了%d點傷害\n", monster[i].name,i+1,damage);
            			Sleep(500);
					}
				}
			}
			break;
	}
}

void use_skill() {
	if(player_effect.deride != 0 && monster[player_effect.deride-1].health > 0){
		aim = player_effect.deride;
	}
    final_atk = skill_list[choice].atk;
    printf("順利使用 %s\n", skill_list[choice].name);
    al_use++;
    al_move_use+=skill_list[choice].cost;
    Sleep(400);
    relic_effect(when_attack);
    if(skill_list[choice].atk > 0 && player_effect.rage > 0){
    	if(skill_list[choice].mul==0){
    		final_atk += 2; 
		}
		else{
			final_atk += 1; 
		}
        player_effect.rage--; 
        printf("狂怒發動！消耗 1 點狂怒，本次傷害提升！(剩餘狂怒: %d)\n", player_effect.rage);
        Sleep(800);
    }
    Sleep(800);
    special_move = 0;
    
    // --- 扣除費用與獲得增益 ---
    player.move_point -= skill_list[choice].cost;
    player.def += skill_list[choice].def;
    healing(skill_list[choice].heal); 
    // --- 造成傷害 ---
    if(skill_list[choice].mul == 1){
    	int damage_i;
    	for(damage_i=0;damage_i<3;damage_i++){
    		monster[damage_i].def-=final_atk;
    		if(monster[damage_i].def<0){
    			if(monster[damage_i].health>0){
    				printf("對%s%d造成了%d點傷害\n",monster[damage_i].name,damage_i+1,-monster[damage_i].def);
    				Sleep(300);
				}
        		monster[damage_i].health += monster[damage_i].def;
				monster[damage_i].def = 0; 
  			}
		}
    } 
	else {
		monster[aim-1].def-=final_atk;
    	if(monster[aim-1].def<0){
    		if(monster[aim-1].health>0){
    				printf("對%s%d造成了%d點傷害\n",monster[aim-1].name,aim,-monster[aim-1].def);
    				Sleep(500);
				}
			monster[aim-1].health += monster[aim-1].def;
			monster[aim-1].def = 0;
		}
    }
    // --- 結算卡牌去留 ---
    special_effect(); 

    if (special_move != 1) { 
        // 【關鍵修復】如果這張牌剛剛被打出了第二下（標籤為 -5），在離開手牌前將其復活！
        if (skill_list[choice].tag < 0) {
            skill_list[choice].tag = -skill_list[choice].tag;
        }

        // 判定消失或棄牌
        if (skill_list[choice].exhaust >= 1) { // 包含 1(消耗) 和 2(虛無)
            // 如果這是一張「複現的虛無卡」，第二下打完在這裡就會徹底蒸發！
            printf("%s 已被消耗，化為灰燼！\n", skill_list[choice].name);
            Sleep(500);
            if(skill_list[choice].id!=4){
			  al_discard++;
			  relic_effect(when_card_discard);
            } 
        } 
		else {
            // 普通卡：還原成 tag=5 的狀態進入廢牌堆
            discard_pile[discard_num] = skill_list[choice];
            discard_num++;
        }
        pre_list(); 
    }
    Sleep(400);
}

void load_monster(){
    int load_i;
    int monster_id;
    for(load_i = 0; load_i <= 2; load_i++){
        monster_id = monster_list[locate%39][load_i];
        monster[load_i] = monster_db[monster_id]; 
        monster[load_i].ex_speed +=  monster_speed[locate%39][load_i];
    }
}

void card_reward() {
    int pool[100];
    int pool_count = 0;
    int i, j;
    for (i = 0; i < 100; i++) {
    	if (locate<=12){
    		if (skill_db[i].id >= 26 && skill_db[i].id <=50) {
            	pool[pool_count] = i;
            	pool_count++;
        	}
		}
		else{
			if (skill_db[i].id >= 26 && skill_db[i].id <=100) {
       		    pool[pool_count] = i;
       		    pool_count++;
        	}
		}
    }
    // 1. 先根據關卡進度 (locate)，決定要開放多少張職業卡
	int max_offset;
	if (locate <= 12) {
	    max_offset = 5;  // 例如 ID 101~106 (差值為5)
	} 
	else {
	    max_offset = 9;  // 例如 ID 101~110 (差值為9)
	}
	for (i = 1; i <= 6; i++) {
    	if (relic[i].have) {
        	int min_id = 91 + 10 * i; 
        	int max_id = min_id + max_offset; 
        	for (j = 1; j <= 200; j++) { 
            	if (skill_db[j].id >= min_id && skill_db[j].id <= max_id) {
                	pool[pool_count] = j;
                	pool_count++;
            	}
        	}
    	}
	}
    struct skill reward_options[3];
    int opt_count = 0;   
    while (opt_count < 3 && opt_count < pool_count) {
        int rand_idx = rand() % pool_count;
        bool duplicate = false;
        for (j = 0; j < opt_count; j++) {
            if (reward_options[j].id == skill_db[pool[rand_idx]].id) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            reward_options[opt_count] = skill_db[pool[rand_idx]];
            opt_count++;
        }
    }
    printf("\n==========================================\n");
    printf("請選擇一項戰利品加入永久背包：\n");
    printf("------------------------------------------\n");
    for (i = 0; i < 3; i++) {
        printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d)\n", i + 1, reward_options[i].name, reward_options[i].cost, reward_options[i].atk, reward_options[i].def, reward_options[i].heal);
        printf("    描述: %s\n\n", reward_options[i].info);
    }
    printf("[0] 放棄不拿\n");
    printf("==========================================\n");
    int reward_choice;
    while (1) {
        scanf("%d", &reward_choice);
        if (reward_choice == 0) {
            printf("你拒絕了戰利品，卡組沒有發生變化。\n");
            Sleep(1000);
            break;
        }
        else if (reward_choice >= 1 && reward_choice <= 3) {
            backpack[skill_num] = reward_options[reward_choice - 1];
            skill_num++;
            printf("順利將 %s 加入你的永久背包！\n", reward_options[reward_choice - 1].name);
            Sleep(1000);
            break;
        }
        else {
            printf("無效的輸入，請重新選擇。\n");
        }
    }
}

void ex_card_reward() {
    int pool[100];
    int pool_count = 0;
    int i, j;
    // 1. 建立卡池
    for (i = 0; i < 200; i++) {
    	if (locate<=12){
    		if (skill_db[i].id >= 25 && skill_db[i].id <=50) {
            	pool[pool_count] = i;
            	pool_count++;
        	}
		}
		else{
			if (skill_db[i].id >= 25 && skill_db[i].id <=100) {
       		    pool[pool_count] = i;
       		    pool_count++;
        	}
		}
        
    }
    int max_offset;
	if (locate <= 12) {
	    max_offset = 5;  // 例如 ID 101~106 (差值為5)
	} 
	else {
	    max_offset = 9;  // 例如 ID 101~110 (差值為9)
	}
	for (i = 1; i <= 6; i++) {
    	if (relic[i].have) {
        	int min_id = 91 + 10 * i; 
        	int max_id = min_id + max_offset; 
        	for (j = 1; j <= 200; j++) { 
            	if (skill_db[j].id >= min_id && skill_db[j].id <= max_id) {
                	pool[pool_count] = j;
                	pool_count++;
            	}
        	}
    	}
	}
    struct skill reward_options[3];
    int opt_count = 0;   
    // 2. 隨機抽取不重複卡片
    while (opt_count < 3 && opt_count < pool_count) {
        int rand_idx = rand() % pool_count;
        bool duplicate = false;
        for (j = 0; j < opt_count; j++) {
            if (reward_options[j].id == skill_db[pool[rand_idx]].id) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            reward_options[opt_count] = skill_db[pool[rand_idx]];
            opt_count++;
        }
    }
    // 3. 詞綴強化系統
    int card_upgrade;
    char prefix[100];
    char suffix[200]; // 描述可能比較長，給大一點的空間
    // 注意這裡迴圈條件改用 opt_count
    for (i = 0; i < opt_count; i++) {
        card_upgrade = rand() % 7 + 1;
        reward_options[i].tag = card_upgrade;
        // 初始化字串
        strcpy(prefix, "");
        strcpy(suffix, "");
        switch(card_upgrade) {
            case 1:
                strcpy(prefix, "餽贈的");
                strcpy(suffix, "，打出後有70%概率抽1張牌");
                break;
            case 2:
                strcpy(prefix, "精準的");
                strcpy(suffix, "，有50%概率對主目標造成66%額外傷害");
                break;
            case 3:
                strcpy(prefix, "連續的");
                strcpy(suffix, "，打出後獲得2張[連擊]");
                break;
            case 4:
                strcpy(prefix, "複製的");
                strcpy(suffix, "，打出後獲得未強化的消耗品相同牌");
                break;
            case 5:
                strcpy(prefix, "複現的");
                strcpy(suffix, "，此牌能打出兩次");
                break;
            case 6:
                strcpy(prefix, "高溫的");
                strcpy(suffix, "，此牌會點燃主目標");
                break;
            case 7:
                strcpy(prefix, "狂暴的");
                strcpy(suffix, "，此牌行動後獲得1點狂怒");
                break;
        }
        // 將前綴裝上卡片名字 (例如："掠奪的" + "火球術" -> "掠奪的火球術")
        char temp_name[100];
        snprintf(temp_name, sizeof(temp_name), "%s%s", prefix, reward_options[i].name);
        strcpy(reward_options[i].name, temp_name);
        // 將後綴裝上卡片描述 (例如："造成5點傷害" + "，打出後抽一張牌")
        char temp_info[200]; // 先前幫你加大過 info 的大小，這裡發揮作用了！
        snprintf(temp_info, sizeof(temp_info), "%s%s", reward_options[i].info, suffix);
        strcpy(reward_options[i].info, temp_info);
    }
    
    // 4. 顯示與選擇
    printf("\n==========================================\n");
    printf("請選擇一項帶有「稀有詞綴」的戰利品加入永久背包：\n");
    printf("------------------------------------------\n");
    
    // 注意這裡迴圈條件改用 opt_count
    for (i = 0; i < opt_count; i++) {
        printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d)\n", 
               i + 1, reward_options[i].name, reward_options[i].cost, 
               reward_options[i].atk, reward_options[i].def, reward_options[i].heal);
        printf("    描述: %s\n\n", reward_options[i].info);
    }
    printf("[0] 放棄不拿\n");
    printf("==========================================\n");
    
    int reward_choice;
    while (1) {
        scanf("%d", &reward_choice);
        if (reward_choice == 0) {
            printf("你拒絕了戰利品，卡組沒有發生變化。\n");
            Sleep(1000);
            break;
        }
        // 注意這裡判斷上限改用 opt_count
        else if (reward_choice >= 1 && reward_choice <= opt_count) {
            backpack[skill_num] = reward_options[reward_choice - 1];
            skill_num++;
            printf("順利將 %s 加入你的永久背包！\n", reward_options[reward_choice - 1].name);
            Sleep(1000);
            break;
        }
        else {
            printf("無效的輸入，請重新選擇。\n");
        }
    }
}

// 根據你的資料庫，目前最大遺物 ID 是 12
#define MAX_RELIC_ID 49 

void relic_reward() {
    int available_relics[100]; // 存放「還沒獲得」的遺物 ID
    int available_count = 0;
    int i;

    // 1. 掃描陣列，找出玩家「尚未擁有 (have == 0 / false)」的遺物
    for (i = 1; i <= MAX_RELIC_ID; i++) {
        // 確認 ID 存在，而且玩家還沒有這個遺物
        if (relic[i].id != 0 && relic[i].have == false) {
            available_relics[available_count] = i; 
            available_count++;
        }
    }

    // 防呆：如果玩家運氣超好，已經拿完所有 12 個遺物了
    if (available_count == 0) {
        printf("你已獲得所有遺物 無法再獲取更多了\n");
        Sleep(1000);
        return;
    }

    // 2. 隨機挑選 3 個不重複的選項 (若不滿 3 個就顯示剩下的全部)
    int offer_count = (available_count < 3) ? available_count : 3;
    int offered_ids[3];

    // 使用洗牌機制抽出不重複的遺物 ID
    for (i = 0; i < offer_count; i++) {
        int rand_idx = i + rand() % (available_count - i);
        offered_ids[i] = available_relics[rand_idx];
        
        // 將抽過的換到前面，避免重複抽出
        available_relics[rand_idx] = available_relics[i]; 
    }

    // 3. 顯示選單並讓玩家輸入
    int choice = -1;
    while (1) {
        // 可選：如果你有寫清空畫面，可以加在這裡
        printf("\n==========================================\n");
        printf("請選擇一個遺物：\n\n");

        for (i = 0; i < offer_count; i++) {
            int r_id = offered_ids[i];
            // 直接從你一體成型的 relic 陣列讀取名字與描述
            printf("[%d] %s \n    描述: %s\n\n", 
                   i + 1, relic[r_id].name, relic[r_id].info); 
        }
        
        printf("[0] 跳過，我什麼都不想要\n");
        printf("==========================================\n");
        printf("請輸入選擇 (0-%d): ", offer_count);

        scanf("%d", &choice);

        // 4. 結算獎勵
        if (choice == 0) {
            printf("\n你決定把這些遺物留給下一個有緣人。\n");
            Sleep(1000);
            break;
        } 
        else if (choice >= 1 && choice <= offer_count) {
            int selected_id = offered_ids[choice - 1];
            
            // ?? 最關鍵的一行：直接把 have 設為 true (1)！
            // 因為你的 count 已經在 struct 初始化寫好了，所以不需額外給值！
            relic[selected_id].have = true; 
            
            printf("\n你拿起了遺物 [%s]，感覺身體產生了變化！\n", relic[selected_id].name);
            Sleep(1000);
            break; // 拿完就跳出迴圈
        } 
        else {
            printf("\n無效的選擇，請重新輸入！\n");
            Sleep(500);
        }
    }
}

int attack_time(){
    int i; 
    memset(&player_effect, 0, sizeof(player_effect));
    memset(&monster_effect, 0, sizeof(monster_effect));
    memset(skill_list, 0, sizeof(skill_list));
    // 【修改重點】開局直接將永久背包複製到當前牌庫，並確保廢牌堆是空的
    deck_num = 0;
    discard_num = 0;
    al_use = 0;
	al_discard = 0; 
	al_move_use = 0;
    for (i = 0; i < skill_num; i++) {
        skill_deck[deck_num] = backpack[i];
        deck_num++;
    }
    load_monster();
    turn_count = 1;
    player.move_point = player.max_move_point;
    player.def = 0;
    pull_skill(player.pull_num);  
    relic_effect(when_start_fight);
    relic_effect(when_start_round);
    while((monster[0].health > 0 || monster[1].health > 0 || monster[2].health > 0) && player.health > 0){
        int main_choice;
        int i, temp_input;
        
        printf("\n==============================================================\n\n");
        printf("【你的回合 - 第 %d 回合】\n\n", turn_count);
        printf("玩家      \n生命:[%3d/%3d]  防禦:%d  行動點:%d\n", 
                player.health, player.max_health, player.def, player.move_point);
        printf("正面效果:\n");
        if(player_effect.rage>0) printf("怒氣(%d)  ",player_effect.rage);
		if(player_effect.power>0) printf("充能(%d)  ",player_effect.power);
		if(player_effect.regenerate>0) printf("再生(%d)  ",player_effect.regenerate);
		printf("\n負面效果:\n");
		if(player_effect.fire>0) printf("燃燒(%d)  ",player_effect.fire);
		if(player_effect.poison>0) printf("中毒(%d)  ",player_effect.poison);
		if(player_effect.bleed>0) printf("流血(%d)  ",player_effect.bleed);
		printf("\n");
        printf("--------------------------------------------------------------\n\n");
        for(i = 0; i < 3; i++) {
            if(monster[i].max_health > 0 && monster[i].health > 0){
                if ((turn_count+monster[i].ex_speed) % monster[i].speed == 0) {
                    printf("目標 %d:   %-10s 生命:[%3d/%3d] 防禦:[%d] (即將行動!)\n\n", i + 1, monster[i].name, monster[i].health, monster[i].max_health, monster[i].def);
                } else {
                    printf("目標 %d:   %-10s 生命:[%3d/%3d] 防禦:[%d] (準備中 剩餘%d回合)\n\n", i + 1, monster[i].name, monster[i].health, monster[i].max_health,monster[i].def,monster[i].speed-(turn_count+monster[i].ex_speed)%monster[i].speed);
                }
            } else {
                printf("目標 %d:   ---\n\n", i + 1); 
            }
        }
        printf("==============================================================\n");
        printf("請選擇行動目的：\n");
        printf("[1] 檢視遺物背包\n");
        printf("[2] 檢視卡牌背包\n");
        printf("[3] 檢視當前牌庫 (%d 張)\n", deck_num);
        printf("[4] 檢視當前手牌\n");
        printf("[5] 檢視敵人詳細資訊\n");
        printf("[6] 使用技能\n");
        printf("[7] 結束回合\n");
        printf("==============================================================\n");
        printf("請輸入選擇: ");
        scanf("%d", &main_choice);
        
        // --- 目的 1：檢視永久背包 ---
        if(main_choice == 1) {
        	printf("\n--- 遺物背包內容 ---\n");
            for(i = 0; i < 99; i++) {
                if(relic[i].have){
                	if(relic[i].count!=-1) printf("  %s\n  %s( 充能:%d )\n",relic[i].name,relic[i].info,relic[i].count);
                	else printf("  %s\n  %s\n",relic[i].name,relic[i].info); 
				}
            }
            printf("------------------------------------------\n");
            printf("輸入任意數字返回主選單: ");
            scanf("%d", &temp_input);
		}
        else if (main_choice == 2) {
            printf("\n--- 卡牌背包內容 (%d 張) ---\n", skill_num);
            for(i = 0; i < skill_num; i++) {
                printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d) \n    描述: %s\n", 
                       i+1, backpack[i].name, backpack[i].cost, backpack[i].atk, backpack[i].def, backpack[i].heal, backpack[i].info);
            }
            printf("------------------------------------------\n");
            printf("輸入任意數字返回主選單: ");
            scanf("%d", &temp_input);
        }
        // --- 目的 2：檢視當前牌庫 ---
        else if (main_choice == 3) {
            printf("\n--- 當前戰鬥牌庫剩餘 (%d 張) ---\n", deck_num);
            for(i = 0; i < deck_num; i++) {
                printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d) \n    描述: %s\n", 
                       i+1, skill_deck[i].name, skill_deck[i].cost, skill_deck[i].atk, skill_deck[i].def,skill_deck[i].heal,skill_deck[i].info);
            }
            printf("------------------------------------------\n");
            printf("輸入任意數字返回主選單: ");
            scanf("%d", &temp_input);
        }
        // --- 目的 3：檢視當前手牌 ---
        else if (main_choice == 4) {
            printf("\n--- 當前手牌詳細資訊 ---\n");
            for(i = 0; skill_list[i].id != 0; i++) {
                printf("[%d] %s (費用:%d, 傷害:%d, 防禦:%d, 治療:%d) \n    描述: %s\n", 
                       i, skill_list[i].name, skill_list[i].cost, skill_list[i].atk, skill_list[i].def, skill_list[i].heal, skill_list[i].info);
            }
            printf("------------------------------------------\n");
            printf("輸入任意數字返回主選單: ");
            scanf("%d", &temp_input);
        }
        // --- 目的 4：檢視敵人詳細狀態 ---
        else if (main_choice == 5) {
            printf("\n--- 敵方戰場詳細資訊 ---\n");
            for(i = 0; i < 3; i++) {
                if(monster[i].max_health > 0 && monster[i].health > 0) {
                    printf("目標 %d: %s\n", i + 1, monster[i].name);
                    printf("  當前狀態: 生命值:[%d/%d] | 防禦力:[%d] | 基礎數值: %d | 行動偏好ID: %d | 速度: %d\n", 
                           monster[i].health, monster[i].max_health, monster[i].def,monster[i].atk, monster[i].move, monster[i].speed);
                    printf("  負面效果: ");
                    if(monster_effect[i].fire>0) printf("燃燒(%d)  ",monster_effect[i].fire);
					if(monster_effect[i].poison>0) printf("中毒(%d)  ",monster_effect[i].poison);
					if(monster_effect[i].bleed>0) printf("流血(%d)  ",monster_effect[i].bleed);
                    printf("\n");
                    printf("  怪物描述: %s\n\n",monster[i].info);
                } 
				else {
                    printf("目標 %d: --- (已陣亡)\n\n", i + 1);
                }
            }
            printf("------------------------------------------\n");
            printf("輸入任意數字返回主選單: ");
            scanf("%d", &temp_input);
        }
        // --- 目的 5：使用技能 ---
        else if (main_choice == 6) {
            while (1) {
                int hand_count = 0;
                printf("\n--- 請選擇要施放的技能 ---\n");
                printf("[0] 返回主選單\n");
                for(hand_count = 0; skill_list[hand_count].id != 0; hand_count++){
                    printf("[%d] %s (費用: %d)\n", hand_count+1, skill_list[hand_count].name, skill_list[hand_count].cost);
                }
                printf("------------------------------------------\n");
                printf("請輸入選擇: ");
                scanf("%d", &choice);
                choice-=1;
                
                if (choice == -1) {
                    break; // 離開技能使用介面，回到戰鬥主選單
                }
                
                if (choice < -1 || choice >= hand_count || skill_list[choice].id == 0) {
                    printf("未檢測到對應手牌，請重新輸入\n");
                    Sleep(800);
                    continue;
                }
                
                if (skill_list[choice].cost > player.move_point) {
                    printf("費用不足！此技能需要 %d 點行動點 (當前剩餘: %d)\n", skill_list[choice].cost, player.move_point);
                    Sleep(800);
                    continue;
                }
                
                // 選牌完畢，直接亮出卡片細節，進入目標選擇
                /*printf("\n==========================================\n");
                printf("技能名稱: %s\n花費行動點: %d\n基礎傷害: %d\n預期治療: %d\n獲得防禦: %d\n", 
                       skill_list[choice].name, skill_list[choice].cost, skill_list[choice].atk, skill_list[choice].heal, skill_list[choice].def);
                printf("卡牌描述: %s\n", skill_list[choice].info);
                printf("------------------------------------------\n");*/
                printf("請選擇施放目標：\n");
                printf("[1] 攻擊/對1號目標施放\n");
                printf("[2] 攻擊/對2號目標施放\n");
                printf("[3] 攻擊/對3號目標施放\n");
                printf("[0] 返回上一層 (重新選牌)\n");
                printf("請輸入選擇: ");
                scanf("%d", &aim);
                
                if (aim == 0) {
                    printf("已取消施放，返回手牌選單。\n");
                    Sleep(800);
                    continue; 
                }
                
                if (aim >= 1 && aim <= 3) {
                    // 若是單體技能，判定目標是否還活著
                    if (skill_list[choice].mul == 0 && monster[aim-1].health <= 0) {
                        printf("無法選定該目標（目標不存在或已陣亡），請重新選擇\n");
                        Sleep(1000);
                        continue;
                    }
                    // 執行技能，執行完後自動退回主選單更新戰局數據
                    use_skill();
                    relic_effect(when_used_skill);
                    break; 
                } else {
                    printf("無效的目標代碼！\n");
                    Sleep(800);
                }
            }
        }
        // --- 目的 6：結束回合 ---
        else if (main_choice == 7) {
            over = 1;
        }
        else {
            printf("無效的指令，請重新輸入\n");
            Sleep(800);
        }
        // 回合結束結算
        if(over==1){
        	//處理玩家dot 
        	relic_effect(when_round_over);
        	if(player_effect.regenerate>0){
				healing(3);
				player_effect.regenerate-=1;
				Sleep(800);
			}
			if(player_effect.fire>0){
        		player.health-=2;
        		printf("你受到了2點燃燒傷害\n");
        		player_effect.fire-=1;
        		Sleep(800);
			}
			if(player_effect.bleed>0){
        		player.health-=player_effect.bleed;
        		printf("你受到了%d點流血傷害\n",player_effect.bleed);
        		player_effect.bleed=0;
        		Sleep(800);
			}
			if(player_effect.poison>0){
        		player.health-=(player_effect.poison+1)/2;
        		printf("你受到了%d點中毒傷害\n",(player_effect.poison+1)/2);
        		player_effect.poison-=(player_effect.poison+1)/2;
        		Sleep(800);
			}
			player_effect.deride=0;
        	if(player.health<=0)continue;
        	monster[0].def=0;
        	monster[1].def=0;
        	monster[2].def=0;
            monster_move();
            relic_effect(when_monster_round_over); 
            //處理怪物dot 
			int i;
            for(i=0;i<3;i++){
            	if(can_act[i]==false){
            		monster_effect[i].fire=0;
            		monster_effect[i].bleed=0;
            		monster_effect[i].poison=0;
				}
        		if(monster_effect[i].fire>0){
        			monster[i].health-=2;
        			monster_effect[i].fire-=1;
        			printf("%s%d受到了2點燃燒傷害\n",monster[i].name,i+1);
        			Sleep(800);
				}
				if(monster_effect[i].bleed>0){
        			monster[i].health-=monster_effect[i].bleed;
        			printf("%s%d受到了%d點流血傷害\n",monster[i].name,i+1,monster_effect[i].bleed);
        			Sleep(800);
        			monster_effect[i].bleed=0;
				}
				if(monster_effect[i].poison>0){
        			monster[i].health-=(monster_effect[i].poison+1)/2;
        			printf("%s%d受到了%d點中毒傷害\n",monster[i].name,i+1,(monster_effect[i].poison+1)/2);
        			Sleep(800);
        			monster_effect[i].poison-=(monster_effect[i].poison+1)/2;
				}
				if(monster[i].health<=0){
            		monster_effect[i].fire=0;
            		monster_effect[i].bleed=0;
            		monster_effect[i].poison=0;
				}
			}
            if(monster[0].health <= 0 && monster[1].health <= 0 && monster[2].health <= 0)continue;
            if(player.move_point>0) player.move_point = 1;
            player.move_point+=player.max_move_point; 
            player.move_point+=player_effect.power;
            player_effect.power=0;
            player.def=0;
            discard_list();
            pull_skill(player.pull_num);
            over = 0;
            turn_count++;
            relic_effect(when_start_round);
        }
    }
    
    printf("戰鬥結束\n");
    Sleep(1000);
    if(player.health<=0){
        printf("戰鬥失敗\n");
        printf("你倒在了第 %d 區\n", locate);
        die=1;
        over=1;
    }
    return 0;
}

// 移除卡牌的獨立函式
// 回傳值說明： 1 = 成功移除 | 0 = 玩家跳過/取消 | -1 = 卡組太少無法移除
int remove_card() {
    int i, j, remove_idx;
    // 檢查卡組數量
    if (skill_num <= 7) {
        printf("你的卡組太精簡了，不能再移除卡片！\n");
        Sleep(1000);
        return -1; 
    }
    printf("\n--- 請選擇要移除的卡片 ---\n");
    for (i = 0; i < skill_num; i++) {
        printf("[%d] %s\n", i + 1, backpack[i].name);
    }
    printf("[0] 取消/跳過 (不移除任何卡片)\n");
    while(1) {
        printf("請選擇 (0-%d): ", skill_num);
        scanf("%d", &remove_idx);
        if (remove_idx == 0) {
            return 0; // 玩家選擇跳過
        }
        if (remove_idx >= 1 && remove_idx <= skill_num) {
            int idx = remove_idx - 1;
            printf("已將 %s 從背包中永久移除！\n", backpack[idx].name);
            // 將後面的卡片往前移，覆蓋掉要刪除的卡片
            for (j = idx; j < skill_num - 1; j++) {
                backpack[j] = backpack[j + 1];
            }
            skill_num--; // 總牌數減 1
            Sleep(1500);
            return 1; // 成功移除
        } 
		else {
            printf("無效的選擇。\n");
        }
    }
}

void upgrade() {
    int i, j;
    int upgrade_type = rand() % 3; 
    while (1) {
        printf("\n==========================================\n");
        printf("請選擇一項行動：\n");
        printf("------------------------------------------\n");
        Sleep(1000);
        printf("[1] 移除一張牌 (精簡卡組)");
        if (skill_num <= 7) {
            printf("(目前卡片數 %d <= 7，無法使用)\n", skill_num);
        } 
		else {
            printf("\n");
        }
        Sleep(1000);
        printf("[2] 獲得一張新卡片\n");
        Sleep(1000);
        printf("[3] 進行卡牌強化 -> 目前提供：[");
        switch(upgrade_type) {
            case 0: printf("力量強化：隨機抽取卡片，選擇一張傷害 +1~2"); break;
            case 1: printf("防禦強化：隨機抽取卡片，選擇一張防禦 +2"); break;
            case 2: printf("能量強化：隨機抽取卡片，選擇一張費用 -1"); break;
        }
        printf("]\n");
        Sleep(1000);
        printf("[4] 休息：回復 20 點生命值\n");
        printf("==========================================\n");
        int local_choice;
        scanf("%d", &local_choice);
        // ==========================================
        // 【修改重點】目的 1：呼叫獨立的移除卡牌函式
        // ==========================================
        if (local_choice == 1) {
            int result = remove_card();
            if (result == -1) {
                // 卡組太少，直接退回營地主選單重選
                continue; 
            } 
            else if (result == 0) {
                // 玩家按下 0 取消
                printf("你選擇了跳過，空手離開了據點。\n");
                Sleep(1000);
                return; 
            } 
            else if (result == 1) {
                // 成功移除卡牌，結束營地事件
                return; 
            }
        }
        // ==========================================
        
        else if (local_choice == 2) {
            printf("你決定在營地翻找有用的東西...\n");
            Sleep(500);
            card_reward(); 
            return; 
        }
        else if (local_choice == 3) {
            int pool[100];
            int pool_count = 0;
            for (i = 0; i < skill_num; i++) {
                bool eligible = true;
                if (upgrade_type == 2 && backpack[i].cost == 0) eligible = false;
                if (upgrade_type == 3 && backpack[i].tag != 0) eligible = false; // 這裡你原本有寫 case 3，但上面選單只顯示 0-2，這是個小隱患要注意哦！
                
                if (eligible) {
                    pool[pool_count] = i; 
                    pool_count++;
                }
            }
            if (pool_count == 0) {
                printf("你的背包裡沒有符合此強化條件的卡牌！請選擇其他方案。\n");
                Sleep(1500);
                continue; 
            }
            int options[3];
            int opt_count = 0;
            while (opt_count < 3 && opt_count < pool_count) {
                int rand_idx = rand() % pool_count;
                int selected_backpack_idx = pool[rand_idx];
                bool duplicate = false;
                for (j = 0; j < opt_count; j++) {
                    if (options[j] == selected_backpack_idx) {
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate) {
                    options[opt_count] = selected_backpack_idx;
                    opt_count++;
                }
            }
            printf("\n---請選擇一張強化 ---\n");
            for (i = 0; i < opt_count; i++) {
                int idx = options[i];
                printf("[%d] %s (費用:%d, 傷害:%d, 治療:%d,防禦:%d)\n", 
                i + 1, backpack[idx].name, backpack[idx].cost ,backpack[idx].atk,backpack[idx].heal, backpack[idx].def);
                printf("%s\n\n",backpack[idx].info);
            }
            printf("[0] 跳過此據點 (不強化任何卡片，直接前往下一間)\n");
            int upgrade_choice;
            while (1) {
                printf("請選擇 (0-%d): ", opt_count);
                scanf("%d", &upgrade_choice);
                if (upgrade_choice == 0) {
                    printf("你選擇了跳過強化，浪費了這次大好機會...\n");
                    Sleep(1000);
                    return;
                }
                else if (upgrade_choice >= 1 && upgrade_choice <= opt_count) {
                    int target_idx = options[upgrade_choice - 1];
                    strcat(backpack[target_idx].name, "+");
                    switch(upgrade_type) {
                        case 0:
                            if(backpack[target_idx].mul == 0) { // 原本是 mul=0 (賦值)，幫你順手修正為 mul==0 (判斷)
                                backpack[target_idx].atk += 2;
                                printf("%s 強化完成！傷害提升 2 點。\n", backpack[target_idx].name);
                            }
                            else {
                                backpack[target_idx].atk += 1;
                                printf("%s 強化完成！傷害提升 1 點。\n", backpack[target_idx].name);
                            }
                            break;
                        case 1:
                            backpack[target_idx].def += 2;
                            printf("%s 強化完成！防禦提升 2 點。\n", backpack[target_idx].name);
                            break;
                        case 2:
                            backpack[target_idx].cost -= 1;
                            printf("%s 強化完成！費用降低 1 點。\n", backpack[target_idx].name);
                            break;
                    }
                    Sleep(1500);
                    return; 
                } else {
                    printf("無效的選擇。\n");
                }
            }
        }
        else if (local_choice == 4) {
            player.health += 20;
            if (player.health > player.max_health) player.health = player.max_health;
            printf("你在營地好好休息了一會，回復了 20 點生命值。(目前生命: %d/%d)\n", player.health, player.max_health);
            Sleep(1500);
            return; 
        }
        else {
            printf("無效的輸入，請重新選擇。\n");
        }
    }
}

void event(){
	switch(event_list[(locate/13%3)][rand() % event_num[(locate/13)%3]]){
		case(1):{ //純獎勵寶箱 
			printf("你在前方找到了一個豪華的寶箱\n");
			Sleep(1000);
			printf("好奇心驅使著你打開她\n");
			Sleep(1000);
			ex_card_reward();
			break;
		}
		case(2):{ //遭遇食人魔
            printf("你撞見了一個食人魔\n");
            Sleep(1000);
            printf("你想逃 但沒他跑得快\n");
            Sleep(1000);
            printf("所以你打算...\n");
            Sleep(500);
            while(1){
                printf("==========================================\n");
                printf("[1] 丟一塊肉給他 (損失一張牌)\n");
                Sleep(500);
                printf("[2] 與他決鬥 (與一隻食人魔戰鬥)\n");
                printf("==========================================\n");
                int event_choice;
                scanf("%d", &event_choice);
                switch(event_choice){
                    case(1):{
                        int result = remove_card();
                        switch(result){
                            case(1):
                                printf("食人魔順利被你丟出的肉吸引了注意\n");
                                Sleep(1000);
                                printf("你二話不說 拔腿就跑\n");
                                Sleep(1000);
                                return;
                            case(-1):
                                printf("你掏了掏背包\n");
                                Sleep(1000);
                                printf("發現沒有足夠的糧食給他\n");
                                Sleep(1000);
                                printf("你只好使用其他方法\n");
                                Sleep(1000);
                                break;
                            case(0):
                                printf("你仍在猶豫是否將寶貴的肉品給他...\n");
                                Sleep(1000);
                                break;
                        }
                        break;
                    }
                    case(2):{
                    	printf("你決定正面與他對峙\n");
						Sleep(1000);
						monster_list[locate%39][0] = 0;
                        monster_list[locate%39][1] = 9;
                        monster_list[locate%39][2] = 0;
                        monster_speed[locate%39][0] = 0;
                        monster_speed[locate%39][1] = 0;
                        monster_speed[locate%39][2] = 0;
                        attack_time();
                        if(player.health > 0){
                            ex_card_reward();
                        }
                        return;
                    }
                    default:
                        printf("輸入錯誤 請重新再試\n");
                        Sleep(1000);
                }
            }
        }
		case(3):{//哥布林大軍 
			printf("你在遠方看到了一群哥布林\n");
			Sleep(1000);
			printf("他們裝備精良 各個實力超群\n");
			Sleep(1000);
			printf("面對他們 你決定...\n");
			while(1){
				int event_choice;
				printf("==========================================\n");
                printf("[1] 與他們決鬥 (與三隻精銳哥布林戰鬥)\n");
                Sleep(500);
                printf("[2] 趕緊離開 (無事發生)\n");
                printf("==========================================\n");
				scanf("%d",&event_choice);
				switch(event_choice){
					case(1):
						printf("你認為放任他們在這裡是個危害\n");
						Sleep(1000);
						printf("所以你打算將他們清理乾淨\n");
						Sleep(1000);
						monster_list[locate%39][0] = 5;
						monster_list[locate%39][1] = 5;
						monster_list[locate%39][2] = 5;
						monster_speed[locate%39][0] = 0;
						monster_speed[locate%39][1] = 0;
						monster_speed[locate%39][2] = 0;
                        attack_time();
                        if(player.health > 0){
                            ex_card_reward();
                        }
                        return;
					case(2):
						printf("面對難打的敵人\n");
						Sleep(1000);
						printf("你覺得趕緊離開更好\n");
						Sleep(1000);
						return;
					default:
						printf("輸入錯誤 請重新再試\n");
                        Sleep(1000);
				} 
			}
		}
		case(4):{//火槍的試煉
			printf("你發現了一隊火筒哥布林\n");
			Sleep(1000);
			printf("他們正守著豪華的寶箱\n");
			Sleep(1000);
			printf("面對他們 你決定...\n");
			while(1){
				int event_choice;
				printf("==========================================\n");
                printf("[1] 趕緊離開 (無事發生)\n");
                Sleep(500);
                printf("[2] 嘗試偷走寶箱 (進入戰鬥 獲勝能獲得一張強化後的牌)\n");
                Sleep(500); 
                printf("[3] 正面突破 (進入更難的戰鬥 獲勝能獲得兩張強化後的牌)\n");
                printf("==========================================\n");
				scanf("%d",&event_choice);
				switch(event_choice){
					case(1):
						printf("你認為盡早離開比較好\n");
						Sleep(1000);
						printf("再見了 豪華的寶箱\n");
						return;
					case(2):
						printf("你決定悄悄的偷走寶箱\n");
						Sleep(1000);
						printf("不過 你被發現了\n");
						Sleep(1000);
						monster_list[locate%39][0] = 6;
						monster_list[locate%39][1] = 6;
						monster_list[locate%39][2] = 6;
						monster_speed[locate%39][0] = 0;
						monster_speed[locate%39][0] = 1;
						monster_speed[locate%39][0] = 0;
						attack_time();
                        if(player.health > 0){
                            ex_card_reward();
                        }
						return;
					case(3):
						printf("你決定正面突破\n");
						Sleep(1000);
						printf("而他們的槍口 正對著你\n");
						Sleep(1000);
						monster_list[locate%39][0] = 6;
						monster_list[locate%39][1] = 6;
						monster_list[locate%39][2] = 6;
						monster_speed[locate%39][0] = 2; 
						monster_speed[locate%39][0] = 3;
						monster_speed[locate%39][0] = 1;
						attack_time();
                        if(player.health > 0){
                            ex_card_reward();
                            ex_card_reward();
                        }
						return;
					default:
						printf("輸入錯誤 請重新再試\n");
                        Sleep(1000);
				} 
			}
		}
		case(5):{//遇到蜘蛛群
			printf("你發現了一個蜘蛛洞\n");
			Sleep(1000);
			printf("裡面有數不清的蜘蛛在爬行著\n");
			Sleep(1000);
			printf("看著這個洞 你決定...\n");
			Sleep(1000);
			while(1){
				int event_choice;
				printf("==========================================\n");
                printf("[1] 與他們決鬥 (與三隻毒蜘蛛戰鬥)\n");
                Sleep(500);
                printf("[2] 趕緊離開 (無事發生)\n");
                printf("==========================================\n");
				scanf("%d",&event_choice);
				switch(event_choice){
					case(1):
						printf("你認為放任他們在這裡是個危害\n");
						Sleep(1000);
						printf("所以你打算將他們清理乾淨\n");
						Sleep(1000);
						monster_list[locate%39][0] = 15;
						monster_list[locate%39][1] = 15;
						monster_list[locate%39][2] = 15;
						monster_speed[locate%39][0] = 1;
						monster_speed[locate%39][1] = 0;
						monster_speed[locate%39][2] = 1;
                        attack_time();
                        if(player.health > 0){
                            ex_card_reward();
                        }
                        return;
					case(2):
						printf("面對未知的情形\n");
						Sleep(1000);
						printf("你覺得趕緊離開更好\n");
						Sleep(1000);
						return;
					default:
						printf("輸入錯誤 請重新再試\n");
                        Sleep(1000);
				} 
			}
		}
		case(6):{//商人 
			printf("你遇到一個商人\n");
			Sleep(1000);
			printf("他的馬車上有不少珍奇的東西\n");
			Sleep(1000);
			printf("說不定能與他交易些什麼?\n");
			Sleep(1000);
			while(1){
				int event_choice;
				int i;
				printf("==========================================\n");
                printf("[1] 用一張牌換一張帶詞條的牌(無法返回)\n");
                Sleep(500);
                printf("[2] 用三張牌換一個遺物(無法返回)\n");
                Sleep(500);
                printf("[3] 轉身離開 \n");
                printf("==========================================\n");
                scanf("%d",&event_choice);
				switch(event_choice){
					case(1):
						if(skill_num>=8){
							for(i=0;i<1;){
								int result = remove_card();
								if(result==1){
									i++;
								}
								else{
									printf("商人: 別想賒帳\n");
									Sleep(1000);
								}
							}
							ex_card_reward();
							return;
						}
						else{
							printf("你似乎沒有足夠的資源來買下這個東西\n");
							Sleep(1000);
							break;
						} 
					case(2):
						if(skill_num>=10){
							for(i=0;i<3;){
								int result = remove_card();
								if(result==1){
									i++;
								}
								else{
									printf("商人: 別想賒帳\n");
									Sleep(1000);
								}
							}
							relic_reward();
							return;
						}
						else{
							printf("你似乎沒有足夠的資源來買下這個東西\n");
							Sleep(1000);
							break;
						} 
					case(3):
						printf("你離開了這裡\n");
						Sleep(1000);
						return;
					default:
						printf("輸入錯誤 請重新再試\n");
                        Sleep(1000);
                }
			}
		}
		case(7):{
		}
	}
}

void choice_profession(){
	int profession;
	while(1){
		printf("==========================================\n");
        printf("請選擇你的職業\n\n");
        Sleep(800);
        
        printf("[1] 戰士--以狂怒和多段攻擊為主\n我打打打打打打\n起始手牌:\n");
        print_skill_info(101); // 冷靜 
        print_skill_info(32); // 狂化
        print_skill_info(104); // 雙刀流 
        printf("特殊能力: %s\n",relic[1].info);
        printf("\n");
        Sleep(800);
        
        printf("[2] 火焰法師--以燃燒為主\n將世界燃燒殆盡吧\n起始手牌:\n");
        print_skill_info(112); // 爆燃
        print_skill_info(114); // 火球 
        print_skill_info(111); // 炎槍 
        printf("特殊能力: %s\n",relic[2].info);
        printf("\n");
        Sleep(800);
        
        printf("[3] 武士--以連擊為主\n瘋狂出牌使自己變強\n起始手牌:\n");
        print_skill_info(121); // 全彈發射
        print_skill_info(125); // 蓄力重擊
        print_skill_info(34); // 補給
        printf("特殊能力: %s\n",relic[3].info);
        printf("\n");
        Sleep(800);
        
        printf("[4] 刺客--以重複出牌為主\n使自成長的牌快速變強\n起始手牌:\n");
        print_skill_info(135); // 策略進攻
        print_skill_info(131); // 連環斬擊
        print_skill_info(132); // 重複保護
        printf("特殊能力: %s\n",relic[4].info);
        printf("\n");
        Sleep(800);
        
        printf("[5] 女巫--以消耗品卡牌為主\n瘋狂消耗卡牌使卡牌變強\n起始手牌:\n");
        print_skill_info(146); // 藥水袋 
        print_skill_info(141); // 藥水雨 
        print_skill_info(142); // 女巫的佳釀 
        printf("特殊能力: %s\n",relic[5].info);
        printf("\n");
        Sleep(800);
        
        printf("[6] 聖騎士--以防禦和生命為主\n消耗自己的防禦和生命來換取傷害\n起始手牌:\n");
        print_skill_info(28); // 庇佑
        print_skill_info(152); // 犧牲打擊 
        print_skill_info(154); // 盾擊
        printf("特殊能力: %s\n",relic[6].info);
        printf("\n");
        Sleep(800);
        
        printf("==========================================\n");
        scanf("%d",&profession);
        system("cls");
        switch(profession){
        	case(1):
        		add_skill(101);
        		add_skill(32);
        		add_skill(104);
        		relic[1].have=1;
        		printf("你決定成為戰士\n");
				Sleep(1000); 
        		return;
        	case(2):
        		add_skill(112);
        		add_skill(114);
        		add_skill(111);
        		relic[2].have=1;
        		printf("你決定成為火焰法師\n");
				Sleep(1000); 
        		return;
        	case(3):
        		add_skill(121);
        		add_skill(125);
        		add_skill(34);
        		relic[3].have=1;
        		printf("你決定成為武士\n");
				Sleep(1000); 
        		return;
        	case(4):
        		add_skill(135);
        		add_skill(131);
        		add_skill(132);
        		relic[4].have=1;
        		printf("你決定成為刺客\n");
				Sleep(1000); 
        		return;
        	case(5):
        		add_skill(146);
        		add_skill(141);
        		add_skill(142);
        		relic[5].have=1;
        		printf("你決定成為女巫\n");
				Sleep(1000); 
        		return;
        	case(6):
        		add_skill(28);
        		add_skill(152);
        		add_skill(154);
        		relic[6].have=1;
        		printf("你決定成為聖騎士\n");
				Sleep(1000); 
        		return;
        	default:
        		printf("輸入錯誤 請重新再試\n");
        		Sleep(800);
		}
	}
}

void choice_locate(){
	while(1){
		int input=0;
		int i;
		printf("請選擇起始地區\n");
		Sleep(1000);
		printf("==========================================\n\n");
		printf("[1]區域1 原初森林\n");
		printf("補償: 無\n\n");
		Sleep(1000);
		printf("[2]區域2 魔物平原\n");
		printf("補償: 1個遺物 3張帶詞條的牌 5次選牌機會 3次強化機會\n\n");
		Sleep(1000);
		printf("[3]區域3 黑暗王城\n");
		printf("補償: 2個遺物 6張帶詞條的牌 10次選牌機會 6次強化機會\n\n");
		printf("==========================================\n");
		scanf("%d",&input);
		switch(input){
			case(1):
				system("cls");
				printf("前往原初森林中");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".\n");
				Sleep(1000);
				return;
			case(2):
				locate=13;
				player.max_move_point+=1;
				player.max_health+=10;
				player.health+=5;
				relic_reward();
				for(i=0;i<3;i++){
					ex_card_reward();
				}
				for(i=0;i<5;i++){
					card_reward();
				}
				for(i=0;i<3;i++){
					upgrade();
				}
				system("cls");
				printf("前往魔物平原中");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".\n");
				Sleep(1000);
				return;
			case(3):
				locate=26;
				player.max_move_point+=1;
				player.max_health+=20;
				player.health+=20;
				player.pull_num+=1;
				relic_reward();
				relic_reward();
				for(i=0;i<6;i++){
					ex_card_reward();
				}
				for(i=0;i<10;i++){
					card_reward();
				}
				for(i=0;i<6;i++){
					upgrade();
				}
				system("cls");
				printf("前往黑暗王城中");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".");
				Sleep(700);
				printf(".\n");
				Sleep(1000);
				return;
		}
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	starting();
	choice_profession();
	choice_locate();
	while(die==0){
		switch(map[locate%13]){
			case(1):
				printf("遭遇戰鬥 即將迎敵\n");
				Sleep(1000);
				attack_time();
				card_reward();
				printf("前往下一個房間中\n");
				locate++;
				Sleep(1000);
				break;
			case(3):
				event();
				printf("前往下一個房間中\n");
				locate++;
				Sleep(1000);
				break;
			case(4):
				printf("遭遇戰鬥 即將迎敵\n");
				Sleep(1000);
				attack_time();
				ex_card_reward();
				printf("前往下一個房間中\n");
				locate++;
				Sleep(1000);
				break;
			case(5):
				printf("遭遇戰鬥 即將迎敵\n");
				Sleep(1000);
				attack_time();
				if(player.health > 0){
				switch(locate){
					case(12):
						printf("恭喜你 你擊敗了哥布林首領\n");
						Sleep(1000);
						printf("獲得遺物[首領的皇冠]\n");
						Sleep(1000);
						printf("最大生命值+10\n");
						player.max_health+=10;
						Sleep(1000);
						printf("回合行動值+1\n");
						player.max_move_point+=1;
						Sleep(1000);
						printf("現有生命值回復到滿血\n");
						player.health=player.max_health;
						Sleep(1000);
						relic_reward();
						ex_card_reward();
						upgrade();
						break;
					case(25):
						printf("恭喜你 你擊敗了荒野領主\n");
						Sleep(1000);
						printf("獲得遺物[領主的權杖]\n");
						Sleep(1000);
						printf("最大生命值+10\n");
						player.max_health+=10;
						Sleep(1000);
						printf("手牌獲得量+1\n");
						player.pull_num+=1;
						Sleep(1000);
						printf("現有生命值回復到滿血\n");
						player.health=player.max_health;
						Sleep(1000);
						relic_reward();
						ex_card_reward();
						upgrade();
						break;
					case(38):
						printf("恭喜你 你擊敗了深暗巨龍\n");
						Sleep(1000);
						printf("這個世界再次迎來了和平\n");
						Sleep(1000);
						return 0;
				}
				printf("前往下一個區域中\n");
				locate++;
				Sleep(1000);
				}
				break;
			case(2):
				printf("你來到了一個溫馨的營地\n");
				Sleep(1000);
				upgrade();
				printf("前往下一個房間中\n");
				locate++;
				Sleep(1000);
				break;
			default:
				printf("這裡沒什麼值得探索的\n");
				Sleep(1000);
				printf("趕緊離開吧\n");
				Sleep(1000);
				printf("前往下一個房間中\n");
				locate++;
				Sleep(1000);
		} 
	}
}
