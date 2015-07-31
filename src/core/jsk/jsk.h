
#ifndef JSK_JSK_H_
#define JSK_JSK_H_

#include <jsk/types.h>

jfd_t jsk_socket (int domain, int protocol);
int jsk_close (jfd_t s);

int jsk_setsockopt (jfd_t s, int level, int option, const void *optval, size_t optvallen);
int jsk_getsockopt (jfd_t s, int level, int option, void *optval, size_t *optvallen);

int jsk_bind (jfd_t s, const char *addr);
int jsk_connect (jfd_t s, const char *addr);
int jsk_socketpair (jfd_t s1, jfd_t s2);
int jsk_shutdown (jfd_t s, int how);

int jsk_send (jfd_t s, const void *buf, size_t len, int flags);
int jsk_recv (jfd_t s, void *buf, size_t len, int flags);

//int jsk_sendmsg (int s, const struct msghdr *msghdr, int flags);
//int jsk_recvmsg (int s, struct msghdr *msghdr, int flags);

int jsk_socketpair();

#endif /* JSK_JSK_H_ */
