/*
 * hash.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_HASH_H_
#define SRC_CORE_JET_HASH_H_

struct jhlist_head_s {
        struct jhlist_node_s *first;
};

struct jhlist_node_s {
        struct jhlist_node_s *next, **pprev;
};


/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */

#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct jhlist_head_s name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
static void
INIT_HLIST_NODE(struct jhlist_node_s *h)
{
        h->next = NULL;
        h->pprev = NULL;
}

static int
hlist_unhashed(const struct jhlist_node_s *h)
{
        return !h->pprev;
}

static int
hlist_empty(const struct jhlist_head_s *h)
{
        return !h->first;
}

static void
__hlist_del(struct jhlist_node_s *n)
{
        struct jhlist_node_s *next = n->next;
        struct jhlist_node_s **pprev = n->pprev;
        *pprev = next;
        if (next)
                next->pprev = pprev;
}

static void
hlist_del(struct jhlist_node_s *n)
{
        __hlist_del(n);
//        n->next = LIST_POISON1;
//        n->pprev = LIST_POISON2;
}

static void
hlist_del_init(struct jhlist_node_s *n)
{
        if (!hlist_unhashed(n)) {
                __hlist_del(n);
                INIT_HLIST_NODE(n);
        }
}

static void
hlist_add_head(struct jhlist_node_s *n, struct jhlist_head_s *h)
{
        struct jhlist_node_s *first = h->first;
        n->next = first;
        if (first)
                first->pprev = &n->next;
        h->first = n;
        n->pprev = &h->first;
}

#define hlist_hash(key, c)   ( key * 31 + c)

static uint32_t
hlist_hash_key(void *data, size_t len)
{
    uint32_t  i, key;
    uint8_t *d;

    key = 0;
    d = data;

    for (i = 0; i < len; i++) {
        key = hlist_hash(key, d[i]);
    }

    return key;
}


#endif /* SRC_CORE_JET_HASH_H_ */
