#include <stdio.h>
#include <string.h>

/* 打擊結果類型：
   OUT      表示出局
   BASE_HIT 表示安打 */
typedef enum {
    OUT,
    BASE_HIT
} play_type_t;

/* 使用 union 儲存打擊結果：
   安打時使用 base_hit
   出局時使用 out_kind */
typedef union {
    int base_hit;     // 1~4：前進幾個壘包
    char out_kind;    // 'F'、'S'、'G'
} play_result_t;

/* 一次打擊結果 */
typedef struct {
    play_type_t type;     // 打擊類型：出局或安打
    play_result_t result; // 打擊的詳細結果
} play_t;

/* 一位球員的資料 */
typedef struct {
    int no;           // 這位球員擁有幾筆打擊結果
    play_t data[5];   // 最多儲存5次打擊結果
} basePlayer_t;


/* 將輸入的字串轉換成 play_t 結構 */
play_t makeplay(char result[])
{
    play_t play;

    /* HR代表全壘打，前進4個壘包 */
    if (strcmp(result, "HR") == 0) {
        play.type = BASE_HIT;
        play.result.base_hit = 4;
    }
    /* 例如1B、2B、3B */
    else if (result[1] == 'B') {
        play.type = BASE_HIT;

        /* 將字元'1'、'2'、'3'轉成整數1、2、3 */
        play.result.base_hit = result[0] - '0';
    }
    /* 其他情況代表出局，例如F、S、G */
    else {
        play.type = OUT;
        play.result.out_kind = result[0];
    }

    return play;
}


/* 處理安打後的跑壘情況
   base[0]：一壘
   base[1]：二壘
   base[2]：三壘
   hit：安打壘打數
   回傳本次打擊得到的分數 */
int moverunner(int base[3], int hit)
{
    /* 儲存所有跑者移動後的新壘包狀態 */
    int newbase[3] = {0, 0, 0};

    /* 本次打擊得到的分數 */
    int score = 0;

    /* 處理原本已經在壘包上的跑者 */
    for (int i = 0; i < 3; i++) {

        /* base[i]為1，表示這個壘包上有跑者 */
        if (base[i] == 1) {

            /* 陣列索引0、1、2分別代表一、二、三壘 */
            int oldposition = i + 1;

            /* 計算跑者安打後會到達的位置 */
            int newposition = oldposition + hit;

            /* 到達或超過本壘，得到1分 */
            if (newposition >= 4) {
                score++;
            }
            /* 還沒回到本壘，記錄新的壘包位置 */
            else {
                newbase[newposition - 1] = 1;
            }
        }
    }

    /* 處理擊球員本身 */
    if (hit == 4) {
        /* 全壘打，擊球員自己也得到1分 */
        score++;
    }
    else {
        /* 擊球員站上對應的壘包
           1B站一壘、2B站二壘、3B站三壘 */
        newbase[hit - 1] = 1;
    }

    /* 將新的壘包狀態複製回原本的base陣列 */
    for (int i = 0; i < 3; i++) {
        base[i] = newbase[i];
    }

    return score;
}


int main()
{
    /* 九位球員的資料 */
    basePlayer_t player[9];

    /* 記錄每位球員下一次要使用第幾筆打擊結果 */
    int nextplay[9] = {0};

    /* 一、二、三壘是否有跑者 */
    int base[3] = {0, 0, 0};

    /* 比賽進行到指定的出局數 */
    int targetout;

    /* 目前累積出局數 */
    int totalout = 0;

    /* 球隊總得分 */
    int score = 0;

    /* 目前輪到第幾位球員打擊 */
    int batter = 0;

    /* 儲存打擊結果字串，例如1B、2B、HR、F */
    char result[3];

    /* 輸入九位球員的打擊資料 */
    for (int i = 0; i < 9; i++) {

        /* 輸入第i位球員的打擊次數 */
        scanf("%d", &player[i].no);

        /* 輸入這位球員的每次打擊結果 */
        for (int j = 0; j < player[i].no; j++) {
            scanf("%2s", result);

            /* 將字串轉換成play_t後儲存 */
            player[i].data[j] = makeplay(result);
        }
    }

    /* 輸入比賽要進行到幾個出局數 */
    scanf("%d", &targetout);

    /* 持續比賽，直到累積出局數到達targetout */
    while (totalout < targetout) {
        play_t play;

        /* 取得目前擊球員的下一筆打擊結果 */
        play = player[batter].data[nextplay[batter]];

        /* 下一次輪到這位球員時，使用下一筆資料 */
        nextplay[batter]++;

        /* 如果這次打擊結果是出局 */
        if (play.type == OUT) {
            totalout++;

            /* 每三個出局數結束一局，清空所有壘包 */
            if (totalout % 3 == 0) {
                base[0] = 0;
                base[1] = 0;
                base[2] = 0;
            }
        }
        /* 如果這次打擊結果是安打 */
        else {
            score += moverunner(base, play.result.base_hit);
        }

        /* 換下一位球員打擊
           第九位球員後重新回到第一位球員 */
        batter = (batter + 1) % 9;
    }

    /* 輸出總得分 */
    printf("%d\n", score);

    return 0;
}