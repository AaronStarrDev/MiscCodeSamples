#include <dll.h>

List *List_create()
{
    return malloc(sizeof(List));
}

/*void List_destroy(List *list)
{
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}


void List_clear(List *list)
{
    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}


void List_clear_destroy(List *list)
{
    List_clear(list);
    List_destroy(list);
}*/

// Pass in list, and the value you wish to add (push only)
void push(List *lst, void *value)
{   //allocate appropriate memory (create newnode)
    ListNode *newnode = malloc(sizeof(ListNode));
    
    //set the value of newnode
    newnode->value = value;
    
    //if the list coutn is 0, or nothing is in the list, make afirst and last point to the newnode
    if(lst->count == 0) {
        lst->first = node;
        lst->last = node;
    } else { //something is in the list, so we have to add after the list, so we forget first and deal with last only.
        lst->last->next = newnode;
        newnode->prev = lst->last;
        lst->last = newnode;
    }

    lst->count++;
}

void *pop(List *lst)
{
    // create copy of the last node (node to pop)
    // now, if that node is actually something (should be) then return the removal, else NULL.
    ListNode *popnode = lst->last;
    if(popnode != NULL){
        return remove(lst,popnode);
    }
    else{
        return NULL;
    }
}

void List_unshift(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *remove(List *lst, ListNode *node)
{
    void *result = NULL;

    if (lst->first == NULL || lst->last == NULL){
        printf("First or last is empty/null");
    }
    if (node == NULL){
        printf("Node is empty/null");
    }

    if(lst->count == 1) { // the list has one node
        lst->first = NULL;
        lst->last = NULL;
    } else if(node == lst->first) { // node at front of lst
        lst->first = node->next; // move first
        if(lst->first == NULL){
            printf("problem with remove")
        }
        lst->first->prev = NULL; //severed ties
    } else if (node == lst->last) { // node at end of lst
        lst->last = node->prev; // move last
        if(lst->last == NULL){
            printf("problem with remove")
        }
        lst->last->next = NULL; //severed ties
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    lst->count--;
    result = node->value;
    free(node); //"delete" node

    return result;
}