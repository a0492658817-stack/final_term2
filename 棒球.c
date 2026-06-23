#include <stdio.h>
#include <string.h>

/* 打擊結果的類型 */
typedef enum {
    OUT,        // 出局
    BASE_HIT    // 安打
} play_type_t;


/* 儲存打擊的詳細結果
   union 同一時間只會使用其中一個欄位 */
typedef union {
    int base_hit;      // 安打壘打數：1、2、3、4
    char out_kind;     // 出局類型：F、S、G
} play_result_t;


/* 儲存一次打擊結果 */
typedef struct {
    play_type_t type;      // 出局或安打
    play_result_t result;  // 詳細打擊結果
} play_t;


/* 儲存一位球員的所有打擊資料 */
typedef struct {
    int no;           // 打擊資料的數量
    play_t data[5];   // 最多5筆打擊結果
} basePlayer_t;


/* 將輸入字串轉換成 play_t 結構 */
play_t makePlay(char str[])
{
    play_t play;

    /* HR代表全壘打，壘打數為4 */
    if (strcmp(str, "HR") == 0) {
        play.type = BASE_HIT;
        play.result.base_hit = 4;
    }

    /* 字串第二個字元是B，代表1B、2B或3B */
    else if (str[1] == 'B') {
        play.type = BASE_HIT;

        /* 將字元轉換成整數
           '1' - '0' = 1
           '2' - '0' = 2
           '3' - '0' = 3 */
        play.result.base_hit = str[0] - '0';
    }

    /* 其他情況代表出局，例如F、S、G */
    else {
        play.type = OUT;
        play.result.out_kind = str[0];
    }

    /* 回傳轉換完成的打擊結果 */
    return play;
}


/* 處理安打後的跑壘情況
   base[0]代表一壘
   base[1]代表二壘
   base[2]代表三壘
   hit代表安打壘打數
   回傳這次打擊得到的分數 */
int moveRunner(int base[], int hit)
{
    int score = 0;

    /* 從三壘往一壘處理跑者
       倒著處理可以避免同一位跑者被移動兩次 */
    for (int i = 2; i >= 0; i--) {

        /* base[i]等於1，表示該壘包上有跑者 */
        if (base[i] == 1) {

            /* 跑者離開原本的壘包 */
            base[i] = 0;

            /* i + 1是跑者原本所在的壘包
               再加上hit就是移動後的位置
               到達或超過第4壘代表回到本壘得分 */
            if (i + 1 + hit >= 4) {
                score++;
            }

            /* 還沒回到本壘，將跑者放到新的壘包 */
            else {
                base[i + hit] = 1;
            }
        }
    }

    /* 處理擊球員本身 */
    if (hit == 4) {

        /* 全壘打，擊球員自己得到1分 */
        score++;
    }
    else {

        /* 一壘安打站上一壘
           二壘安打站上二壘
           三壘安打站上三壘 */
        base[hit - 1] = 1;
    }

    /* 回傳這次打擊得到的分數 */
    return score;
}


int main(void)
{
    /* 儲存九位球員的資料 */
    basePlayer_t player[9];

    /* 記錄每位球員下一次要使用第幾筆打擊資料 */
    int nextPlay[9] = {0};

    /* 記錄一、二、三壘是否有跑者
       0代表沒人，1代表有人 */
    int base[3] = {0};

    /* 比賽要進行到幾個出局數 */
    int targetOut;

    /* 目前累積的出局數 */
    int totalOut = 0;

    /* 球隊目前總得分 */
    int score = 0;

    /* 目前輪到哪位球員打擊
       0代表第一位球員
       8代表第九位球員 */
    int batter = 0;

    /* 暫存輸入的打擊結果，例如1B、2B、HR、F */
    char result[3];


    /* 輸入九位球員的打擊資料 */
    for (int i = 0; i < 9; i++) {

        /* 輸入第i位球員有幾筆打擊資料 */
        scanf("%d", &player[i].no);

        /* 輸入這位球員的每一次打擊結果 */
        for (int j = 0; j < player[i].no; j++) {

            /* 最多讀取2個字元，避免陣列超出範圍 */
            scanf("%2s", result);

            /* 將字串轉換成play_t並儲存 */
            player[i].data[j] = makePlay(result);
        }
    }

    /* 輸入比賽要進行到幾個出局數 */
    scanf("%d", &targetOut);


    /* 出局數還沒達到指定數量時，繼續比賽 */
    while (totalOut < targetOut) {

        /* 取得目前擊球員的下一筆打擊結果 */
        play_t play =
            player[batter].data[nextPlay[batter]++];

        /* 如果這次打擊是出局 */
        if (play.type == OUT) {

            /* 累積出局數加1 */
            totalOut++;

            /* 每三個出局結束一局
               新的一局開始時清空所有壘包 */
            if (totalOut % 3 == 0) {
                base[0] = base[1] = base[2] = 0;
            }
        }

        /* 如果這次打擊是安打 */
        else {

            /* 移動所有跑者，並累加得到的分數 */
            score += moveRunner(
                base,
                play.result.base_hit
            );
        }

        /* 換下一位球員打擊
           第九位球員打完後回到第一位球員 */
        batter = (batter + 1) % 9;
    }


    /* 輸出球隊總得分 */
    printf("%d\n", score);

    return 0;
}