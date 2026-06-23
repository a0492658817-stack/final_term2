#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 10000
#define MAX_WORD 1000

/* Link List 節點 */
typedef struct Node {
    char *word;          // 儲存單詞
    struct Node *next;   // 指向下一個節點
} Node;

/* 複製字串 s 的前 n 個字元 */
char *copy(const char *s, size_t n)
{
    /* 多配置一格放 '\0' */
    char *p = malloc(n + 1);

    /* 複製 n 個字元 */
    memcpy(p, s, n);

    /* 加上字串結尾 */
    p[n] = '\0';

    return p;
}

/* 建立一個新節點 */
Node *newnode(const char *s, size_t n)
{
    Node *p = malloc(sizeof(Node));

    /* 複製單詞到節點中 */
    p->word = copy(s, n);

    /* 新節點目前沒有下一個節點 */
    p->next = NULL;

    return p;
}

/* 從串列尾端加入一個新節點 */
void append(Node **head, Node **tail, const char *s, size_t n)
{
    Node *p = newnode(s, n);

    /* 如果串列不是空的，讓原本尾節點指向新節點 */
    if (*tail != NULL) {
        (*tail)->next = p;
    }
    else {
        /* 空串列時，新節點同時是第一個節點 */
        *head = p;
    }

    /* 更新尾節點 */
    *tail = p;
}

/* 將文章依照空格分割，建立 Link List */
Node *buildlist(const char *article)
{
    Node *head = NULL;
    Node *tail = NULL;

    /*
     * strtok() 會修改原字串，
     * 所以先複製 article
     */
    char *text = copy(article, strlen(article));

    /* 取得第一個單詞 */
    char *word = strtok(text, " ");

    while (word != NULL) {
        /* 將單詞加入串列尾端 */
        append(&head, &tail, word, strlen(word));

        /* 取得下一個單詞 */
        word = strtok(NULL, " ");
    }

    /* text 已經不需要，釋放記憶體 */
    free(text);

    return head;
}

/* 輸出串列，並釋放所有節點 */
void output(Node *head)
{
    /* 判斷是不是第一個單詞，避免前面多一個空格 */
    int first = 1;

    while (head != NULL) {
        /* 先保存下一個節點 */
        Node *next = head->next;

        /*
         * 第一個單詞前不印空格，
         * 其他單詞前印一個空格
         */
        printf("%s%s", first ? "" : " ", head->word);
        first = 0;

        /* 先釋放字串，再釋放節點 */
        free(head->word);
        free(head);

        /* 移動到下一個節點 */
        head = next;
    }

    printf("\n");
}

/* 1. 將所有單詞 p 替換成 q */
void replaceword(Node *head, const char *p, const char *q)
{
    /* 逐一走訪所有節點 */
    for (; head != NULL; head = head->next) {
        /* 如果目前單詞等於 p */
        if (strcmp(head->word, p) == 0) {
            /* 先釋放原本的單詞 */
            free(head->word);

            /* 複製 q 作為新的單詞 */
            head->word = copy(q, strlen(q));
        }
    }
}

/* 2. 在所有單詞 p 前面插入 q */
void insertbefore(Node **head, const char *p, const char *q)
{
    while (*head != NULL) {
        /* 找到內容等於 p 的節點 */
        if (strcmp((*head)->word, p) == 0) {
            /* 建立內容為 q 的新節點 */
            Node *node = newnode(q, strlen(q));

            /* 新節點指向原本的 p 節點 */
            node->next = *head;

            /* 前面的指標改為指向新節點 */
            *head = node;

            /*
             * 移動到原本 p 的下一個節點，
             * 避免重複處理同一個 p
             */
            head = &node->next->next;
        }
        else {
            /* 不是 p，移動到下一個節點 */
            head = &(*head)->next;
        }
    }
}

/* 3. 刪除所有內容等於 p 的節點 */
void deleteword(Node **head, const char *p)
{
    while (*head != NULL) {
        /* 保存目前節點 */
        Node *node = *head;

        /* 如果目前單詞等於 p */
        if (strcmp(node->word, p) == 0) {
            /*
             * 讓前面的指標直接指向下一個節點，
             * 將目前節點從串列中移除
             */
            *head = node->next;

            /* 釋放單詞和節點 */
            free(node->word);
            free(node);
        }
        else {
            /* 不刪除，移動到下一個節點 */
            head = &node->next;
        }
    }
}

/* 4. 反轉 Link List */
void reverselist(Node **head)
{
    /* 指向已經反轉完成的部分 */
    Node *previous = NULL;

    /* 指向目前正在處理的節點 */
    Node *current = *head;

    while (current != NULL) {
        /* 先保存下一個節點 */
        Node *next = current->next;

        /* 目前節點改為指向前一個節點 */
        current->next = previous;

        /* previous 移動到目前節點 */
        previous = current;

        /* current 移動到原本的下一個節點 */
        current = next;
    }

    /* previous 最後會指向新的第一個節點 */
    *head = previous;
}

/* 5. 使用完整字串 s 分割每一個單詞 */
void splitwords(Node **head, const char *s)
{
    /* 儲存分割後的新串列 */
    Node *newhead = NULL;
    Node *tail = NULL;

    /* 分隔字串 s 的長度 */
    size_t length = strlen(s);

    /* 逐一處理舊串列中的節點 */
    while (*head != NULL) {
        /* 取出目前節點 */
        Node *node = *head;

        /* start 指向目前單詞尚未處理的位置 */
        char *start = node->word;

        /* position 儲存找到 s 的位置 */
        char *position;

        /* 原串列頭指標移到下一個節點 */
        *head = node->next;

        /*
         * 如果 s 是空字串，
         * 不進行分割，直接複製原單詞
         */
        if (length == 0) {
            append(&newhead, &tail, start, strlen(start));
        }
        else {
            /*
             * 在目前單詞中持續尋找分隔字串 s
             */
            while ((position = strstr(start, s)) != NULL) {
                /*
                 * 如果 s 前面有字元，
                 * 將前面的部分加入新串列
                 */
                if (position > start) {
                    append(&newhead,&tail,start,position - start
                    );
                }

                /* 跳過找到的分隔字串 s */
                start = position + length;
            }

            /*
             * 如果最後還有剩餘字元，
             * 將剩餘部分加入新串列
             */
            if (*start != '\0') {
                append(&newhead, &tail,start,strlen(start)
                );
            }
        }

        /* 原節點已經處理完成，釋放記憶體 */
        free(node->word);
        free(node);
    }

    /* 讓原本的 head 指向分割後的新串列 */
    *head = newhead;
}

int main(void)
{
    char article[MAX_LINE];
    char p[MAX_WORD];
    char q[MAX_WORD];
    char s[MAX_WORD];

    Node *list;

    /* 讀取整行文章 */
    fgets(article, sizeof(article), stdin);

    /* 移除 fgets 讀進來的換行符號 */
    article[strcspn(article, "\r\n")] = '\0';

    /*
     * 讀取：
     * p：要尋找的單詞
     * q：取代或插入的單詞
     * s：分割字串
     */
    scanf("%999s %999s %999s", p, q, s);

    /* 1. 單詞取代 */
    list = buildlist(article);
    replaceword(list, p, q);
    output(list);

    /* 2. 在 p 前插入 q */
    list = buildlist(article);
    insertbefore(&list, p, q);
    output(list);

    /* 3. 刪除 p */
    list = buildlist(article);
    deleteword(&list, p);
    output(list);

    /* 4. 反轉單詞順序 */
    list = buildlist(article);
    reverselist(&list);
    output(list);

    /* 5. 使用 s 分割單詞 */
    list = buildlist(article);
    splitwords(&list, s);
    output(list);

    return 0;
}