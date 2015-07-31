
#ifndef JET_LIST_H_
#define JET_LIST_H_


struct jlist_head_s {
    jlist_head_t *next, *prev;
};


static inline void
jlist_init (jlist_head_t *l)
{
    l->next = l;
    l->prev = l;
}

static inline void
__list_add (jlist_head_t *new, jlist_head_t *prev, jlist_head_t *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void
jlist_add (jlist_head_t *new, jlist_head_t *head)
{
    __list_add (new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void
jlist_add_tail (jlist_head_t *new, jlist_head_t *head)
{
    __list_add (new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void
__list_del (jlist_head_t *prev, jlist_head_t *next)
{
    next->prev = prev;
    prev->next = next;
}

//static inline void
//__list_del_entry (struct list_head *entry)
//{
//    __list_del (entry->prev, entry->next);
//}

static inline void
jlist_del (jlist_head_t *entry)
{
    __list_del (entry->prev, entry->next);
//    entry->next = LIST_POISON1;
//    entry->prev = LIST_POISON2;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int
jlist_empty (const jlist_head_t *head)
{
    return head->next == head;
}

#endif // JET_LIST_H_

