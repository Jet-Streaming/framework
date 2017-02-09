
#ifndef JET_LIST_H_
#define JET_LIST_H_

#include "types.h"

struct jlist_head_s {
    jlist_head_t *next, *prev;
};


static void
jlist_init (jlist_head_t *l)
{
    l->next = l;
    l->prev = l;
}

#define JLIST_INIT_STATIC(_l) {(_l), (_l)}

static void
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
static void
jlist_add_after (jlist_head_t *new, jlist_head_t *head)
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
static void
jlist_add_before (jlist_head_t *new, jlist_head_t *head)
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
static void
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

static void
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
static int
jlist_empty (const jlist_head_t *head)
{
    return head->next == head;
}

/**
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:        the list head to take the element from.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

/**
 * list_next_entry - get the next element in list
 * @pos:        the type * to cursor
 * @member:     the name of the list_struct within the struct.
 */
#define list_next_entry(pos, member) \
        list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                          \
        for (pos = list_first_entry(head, typeof(*pos), member);        \
             &pos->member != (head);                                    \
             pos = list_next_entry(pos, member))

#endif // JET_LIST_H_
