
//#include <st/st.h>
#include <utils/hash.h>
#include <utils/types.h>
#include "jsk.h"

//typedef struct jsocket_s jsocket_t;
//
//struct jsocket_s {
//    jhlist_node_t hnode;
//    int fd;
//    jerr_t (*buffer_handler)(jbuffer_t *buf);
//};
//
//static jhlist_head_t socket_heads[1024];
//
////static jsocket_t *
////to_socket(jsk_t s)
////{
////
////    return 0;
////}
////
////static jsk_t
////to_sk(jsocket_t *s)
////{
////    return 0;
////}
//
//
//jerr_t
//jsk_socket (int domain, int protocol, jsk_t *sk)
//{
//    jsocket_t *s;
//
//    BUILD_BUG_ON(sizeof(jsk_t) != sizeof(uint64_t));
//
//    s = malloc(sizeof(jsocket_t));
//    if (!s) {
//        return J_NOMEM;
//    }
//
//    jsk_t sss = {
//            .lcore = 0,
//            .task = 1,
//            .pad = 0,
//    };
//    *sk = sss;
//    return J_OK;
//}
//
//jerr_t
//jsk_close (jsk_t s)
//{
//    return J_OK;
//}
//
////int jsk_setsockopt (jsk_t s, int level, int option, const void *optval, size_t optvallen);
////int jsk_getsockopt (jsk_t s, int level, int option, void *optval, size_t *optvallen);
//
//jerr_t
//jsk_bind (jsk_t s, const char *addr)
//{
//    // publish this addr to the whole cluster
//    return J_OK;
//}
//
//jerr_t
//jsk_connect (jsk_t s, const char *addr)
//{
//    // subscribe this addr
//    return J_OK;
//}
//
//// s1 and s2 must be in the same thread.
//jerr_t
//jsk_socketpair (jsk_t s1, jsk_t s2)
//{
//    if (s1.lcore != s2.lcore) {
//        return J_BADFD;
//    }
//
//
//    return J_OK;
//}
//
////int jsk_shutdown (jsk_t s, int how);
//
//int
//jsk_send (jsk_t s, const jbuffer_t *buf, st_flag_t flags, jtime_t timeout)
//{
//    return 0;
//}
//
//int
//jsk_recv (jsk_t s, jbuffer_t **buf, st_flag_t flags, jtime_t timeout)
//{
//    return 0;
//}
//
//
//jerr_t
//jsk_call(jsk_t s)
//{
////    jsk_send(JSK_SYNC);
////    jsk_recv();
//    return J_OK;
//}
//
//



