#include <stdlib.h>
#include <unistd.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    char value[];
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

// So I found this idea online and it is alot like java interfaces where you can define
// a default or even a java super class where you can define an endless amount of fucntions
// PSUEDOCODE:
// when lst_count(lst) called, return the count value.
// when lst_firstnode(lst) called, return first node value (if first isnt null, return value, else return null)
// when lst_lastnode(lst) called, return last node value (if last isnt null, return value, else return null)
#define lst_countnode(lst) ((lst)->count)
#define lst_firstnode(lst) ((lst)->first != NULL ? (lst)->first->value : NULL) //single conditional line
#define lst_lastnode(lst) ((lst)->last != NULL ? (lst)->last->value : NULL) //single conditional line

//function stricmp() :: strcmp or stricmp is insensitive

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_unshift(List *list, void *value);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)