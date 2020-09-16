#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        q = NULL;
    else {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (q) {
        while (q->head) {
            list_ele_t *tmp;
            tmp = q->head;
            q->head = q->head->next;
            free(tmp->value);
            free(tmp);
        }
        free(q);
        // q->head = NULL;
        // q->tail = NULL;
        // q = NULL;
    }
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
    if (!q)
        return false;
    else {
        list_ele_t *newh;
        newh = malloc(sizeof(list_ele_t));
        if (!newh) {
            free(newh);
            return false;
        } else {
            char *news;
            news = malloc(sizeof(char) * (strlen(s) + 1));
            if (!news) {
                free(news);
                free(newh);
                return false;
            } else {
                strlcpy(news, s, strlen(s) + 1);
                newh->next = q->head;
                newh->value = news;
                if (q->size == 0)
                    q->tail = newh;
                q->head = newh;
                q->size += 1;
                return true;
            }
        }
    }
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
    if (!q)
        return false;
    else {
        list_ele_t *newt;
        newt = malloc(sizeof(list_ele_t));
        if (!newt) {
            free(newt);
            return false;
        } else {
            char *news;
            news = malloc(sizeof(char) * (strlen(s) + 1));
            if (!news) {
                free(newt);
                free(news);
                return false;
            } else {
                strlcpy(news, s, strlen(s) + 1);
                newt->next = NULL;
                newt->value = news;
                if (q->size == 0) {
                    q->head = newt;
                    q->tail = newt;
                } else {
                    q->tail->next = newt;
                    q->tail = newt;
                }
                q->size += 1;
                return true;
            }
        }
    }
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
    if (!q)
        return false;
    else if (!q->head) {
        return false;
    } else {
        list_ele_t *tmp;
        tmp = q->head;
        q->head = q->head->next;
        if (sp) {
            int strLen = strlen(tmp->value) + 1;
            size_t realBufSize = bufsize > strLen ? strLen : bufsize;
            strlcpy(sp, tmp->value, realBufSize);
        }
        free(tmp->value);
        free(tmp);
        if (!q->head)
            q->tail = NULL;
        q->size -= 1;
        return true;
    }
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    else if (!q->head)
        return 0;
    else
        return q->size;
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
    if (q) {
        list_ele_t *cursor = NULL;
        q->tail = q->head;
        while (q->head) {
            list_ele_t *next = q->head->next;
            q->head->next = cursor;
            cursor = q->head;
            q->head = next;
        }
        q->head = cursor;
    }

    /*
        if(q){
            list_ele_t **now = &q->head;
            list_ele_t *cursor = NULL;
            q->tail = q->head;
            while(*now){
                list_ele_t *tmp = (*now)->next;
                (*now)->next = cursor;
                cursor = *now;
                *now = tmp;
            }
            *now = cursor;
        }
   */
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;
    if (q->size == 1)
        return;
    // mergeSortList
    q->head = mergeSortList(q->head);
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *tmp;
    list_ele_t *head;
    if (strcmp(l1->value, l2->value) > 0) {  // l1 > l2
        tmp = l2;
        l2 = l2->next;
    } else {
        tmp = l1;
        l1 = l1->next;
    }
    head = tmp;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) > 0) {  // l1 > l2
            tmp->next = l2;
            tmp = tmp->next;
            l2 = l2->next;
        } else {  // l1 <= l2
            tmp->next = l1;
            tmp = tmp->next;
            l1 = l1->next;
        }
    }
    if (l1)
        tmp->next = l1;
    if (l2)
        tmp->next = l2;

    return head;
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);

    return merge(l1, l2);
}
