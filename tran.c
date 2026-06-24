#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_line 1000
#define max_word 1000
typedef struct Node
{
    char *word;
    struct Node *next;
} Node;

char *copy(const char *article, size_t length)
{
    char *p = malloc(length + 1);
    memcpy(p, article, length);
    p[length] = '\0';
    return p;
}
Node *newnode(const char *word, size_t length)
{
    Node *p = malloc(sizeof(Node));
    p->word = copy(word, length);
    p->next = NULL;
    return p;
}
void append(Node **head, Node **tail, const char *word, size_t length)
{
    Node *p = newnode(word, length);
    if (*tail != NULL)
    {
        (*tail)->next = p;
    }
    else
    {
        *head = p;
    }
    *tail = p;
}
Node *buildlist(const char *article)
{
    Node *head = NULL;
    Node *tail = NULL;
    char *text = copy(article, strlen(article));
    char *word = strtok(text, " ");
    while (word != NULL)
    {
        append(&head, &tail, word, strlen(word));
        word = strtok(NULL, " ");
    }
    free(text);
    return head;
}
void insert_before(Node **head, const char *p, const char *q)
{
    while (*head != NULL)
    {
        if (strcmp((*head)->word, p) == 0)
        {
            Node *new = newnode(q, strlen(q));
            new->next = *head;
            *head = new;
            head = &new->next->next;
        }
        else
        {
            head = &(*head)->next;
        }
    }
}
void insert_after(Node *head, const char *p, const char *q)
{
    while (head != NULL)
    {
        if (strcmp(head->word, p) == 0)
        {
            Node *new = newnode(q, strlen(q));
            new->next = head->next;
            head->next = new;
            head = new->next;
        }
        else
        {
            head = head->next;
        }
    }
}
void replace_word(Node *head, const char *p, const char *q)
{
    while (head != NULL)
    {
        if (strcmp(head->word, p) == 0)
        {
            free(head->word);
            head->word = copy(q, strlen(q));
        }
        head = head->next;
    }
}
void delete_word(Node **head, const char *p)
{
    while (*head != NULL)
    {
        if (strcmp((*head)->word, p) == 0)
        {
            Node *temp = *head;
            *head = temp->next;
            free(temp->word);
            free(temp);
        }
        else
        {
            head = &(*head)->next;
        }
    }
}
void reverse(Node **head)
{
    Node *previous = NULL;
    Node *current = *head;
    while (current != NULL)
    {
        Node *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    *head = previous;
}
void output(Node *head)
{
    int first = 1;
    while (head != NULL)
    {
        Node *next = head->next;
        printf("%s%s", first ? "" : " ", head->word);
        first = 0;
        free(head->word);
        free(head);
        head = next;
    }
    printf("\n");
}
int main()
{
    char article[max_line];
    char p[max_word];
    char q[max_word];
    fgets(article, sizeof(article), stdin);
    article[strcspn(article, "\n\r")] = '\0';
    scanf("%s", p);
    scanf("%s", q);
    Node *list;
    list = buildlist(article);
    insert_before(&list, p, q);
    output(list);

    list = buildlist(article);
    insert_after(list, p, q);
    output(list);

    list = buildlist(article);
    replace_word(list, p, q);
    output(list);

    list = buildlist(article);
    delete_word(&list, p);
    output(list);

    list = buildlist(article);
    reverse(&list);
    output(list);
}