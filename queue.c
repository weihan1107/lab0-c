#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *tmp;
        tmp = q->head->next;
        if (q->head->value)
            free(q->head->value);
        free(q->head);
        q->head = tmp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    // Handle with if queue is NULL
    if (!q)
        return false;

    list_ele_t *newh;
    char *news;

    // Handle with allocate memory fail
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    news = malloc(strlen(s) + 1);
    if (!news) {
        free(newh);
        return false;
    }

    // Copy string
    memcpy(news, s, strlen(s) + 1);

    // Insert to head
    newh->value = news;
    newh->next = q->head;
    q->head = newh;

    // Record size and tail
    if (q->size == 0)
        q->tail = newh;
    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    // Handle with if queue is NULL
    if (!q)
        return false;

    list_ele_t *newh;
    char *news;

    // Handle with allocate memory fail
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    news = malloc(strlen(s) + 1);
    if (!news) {
        free(newh);
        return false;
    }

    // Copy string
    memcpy(news, s, strlen(s) + 1);

    // Insert to tail
    newh->value = news;
    newh->next = NULL;
    if (q->size == 0) {
        q->head = newh;
    } else {
        q->tail->next = newh;
    }

    // Record size and tail
    q->tail = newh;
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head || !sp)
        return false;

    list_ele_t *tmp = q->head->next;

    int size = strlen(q->head->value);

    if (size > bufsize - 1) {
        memcpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    } else {
        memcpy(sp, q->head->value, size + 1);
    }

    free(q->head->value);
    free(q->head);

    q->head = tmp;
    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    q->tail = q->head;
    list_ele_t *tmp, *node;
    node = q->head;
    q->head = NULL;

    while (node) {
        tmp = q->head;
        q->head = node;
        node = node->next;
        q->head->next = tmp;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;

    q->head = merge_sort(q->head);

    q->tail = q->head;
    while (q->tail->next)
        q->tail = q->tail->next;
}

list_ele_t *merge_sort(list_ele_t *node)
{
    list_ele_t *mid, *slow, *fast;
    if (!node || !node->next)
        return node;

    slow = node;
    fast = node->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    mid = slow->next;
    slow->next = NULL;

    return merge(merge_sort(node), merge_sort(mid));

}

list_ele_t *merge(list_ele_t *A, list_ele_t *B)
{
    list_ele_t *ans, *head, *tmp;

    head = NULL;
    ans = head;
    while (A && B) {
        if (strcmp(A->value, B->value) <= 0) {
            tmp = A->next;
            if (!head) {
                head = ans = A;
            } else {
                head->next = A;
                head = head->next;
            }
            A->next = NULL;
            A = tmp;
        } else {
            tmp = B->next;
            if (!head) {
                head = ans = B;
            } else {
                head->next = B;
                head = head->next;
            }
            B->next = NULL;
            B = tmp;
        }
    }

    if (A)
        head->next = A;
    if (B)
        head->next = B;

    return ans;
}
