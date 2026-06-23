#include <stdio.h>

#define SIZE 10

typedef struct
{
    int position;   // 位置，例如第 9 列第 7 行存成 97
    int lineCount;  // 可以形成幾條五子連線
} Result;

int board[SIZE][SIZE];

/* 判斷位置是否在棋盤內 */
int inBoard(int row, int col)
{
    return row >= 0 && row < SIZE &&col >= 0 && col < SIZE;
}

/* 計算某個方向連續有幾顆棋子 */
int countOneSide(int row, int col, int dr, int dc)
{
    int count = 0;

    row += dr;
    col += dc;

    while (inBoard(row, col) && board[row][col] == 1)
    {
        count++;
        row += dr;
        col += dc;
    }

    return count;
}

/* 計算在 board[row][col] 放入棋子後，可以形成幾條五子連線 */
int countLinesAfterPut(int row, int col)
{
    /* 水平、垂直、右斜、左斜 */
    int directions[4][2] = {
        {0, 1},   // 水平 →
        {1, 0},   // 垂直 ↓
        {1, 1},   // 右斜 ↘
        {1, -1}   // 左斜 ↙
    };

    int lineCount = 0;
    int i;

    for (i = 0; i < 4; i++)
    {
        int dr = directions[i][0];
        int dc = directions[i][1];

        /*
         * total 一開始是 1，
         * 代表準備放入的這一顆棋子。
         */
        int total = 1;

        /* 檢查正方向 */
        total += countOneSide(row, col, dr, dc);

        /* 檢查反方向 */
        total += countOneSide(row, col, -dr, -dc);

        /*
         * 必須剛好等於 5。
         * 如果是 6 或更多，就不算五子連線。
         */
        if (total == 5)
        {
            lineCount++;
        }
    }

    return lineCount;
}

int main(void)
{
    Result results[SIZE * SIZE];
    int resultCount = 0;
    int row, col;
    int i, j;

    /* 輸入10×10棋盤 */
    for (row = 0; row < SIZE; row++)
    {
        for (col = 0; col < SIZE; col++)
        {
            scanf("%d", &board[row][col]);
        }
    }

    /* 檢查每一個空的位置 */
    for (row = 0; row < SIZE; row++)
    {
        for (col = 0; col < SIZE; col++)
        {
            int lineCount;

            /* 已經有棋子的地方不能再放 */
            if (board[row][col] == 1)
            {
                continue;
            }

            lineCount = countLinesAfterPut(row, col);

            /* 至少可以形成一條五子連線才記錄 */
            if (lineCount > 0)
            {
                results[resultCount].position = row * 10 + col;
                results[resultCount].lineCount = lineCount;
                resultCount++;
            }
        }
    }

    /*
     * 排序：
     * 1. 五子連線數量由多到少
     * 2. 數量相同時，位置由小到大
     */
    for (i = 0; i < resultCount - 1; i++)
    {
        for (j = i + 1; j < resultCount; j++)
        {
            if (results[i].lineCount < results[j].lineCount ||
                (results[i].lineCount == results[j].lineCount &&
                 results[i].position > results[j].position))
            {
                Result temp = results[i];
                results[i] = results[j];
                results[j] = temp;
            }
        }
    }

    /* 輸出結果 */
    for (i = 0; i < resultCount; i++)
    {
        printf("%02d %d\n",
               results[i].position,
               results[i].lineCount);
    }

    return 0;
}