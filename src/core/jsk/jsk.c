
int jsk_socket (int domain, int protocol)
{
    return 0;
}

int jsk_close (int s)
{
    return 0;
}

int jsk_setsockopt (int s, int level, int option, const void *optval, size_t optvallen)
{
    return 0;
}

int jsk_getsockopt (int s, int level, int option, void *optval, size_t *optvallen)
{
    return 0;
}

int jsk_bind (int s, const char *addr)
{
    return 0;
}

int jsk_connect (int s, const char *addr)
{
    return 0;
}

int jsk_shutdown (int s, int how)
{
    return 0;
}

int jsk_send (int s, const void *buf, size_t len, int flags)
{
    return 0;
}

int jsk_recv (int s, void *buf, size_t len, int flags)
{
    return 0;
}

int jsk_sendmsg (int s, const struct msghdr *msghdr, int flags)
{
    return 0;
}

int jsk_recvmsg (int s, struct msghdr *msghdr, int flags)
{
    return 0;
}
