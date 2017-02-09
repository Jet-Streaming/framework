#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jet/pipeline.h>
#include <jet/packet.h>
//#include <jet/plugin.h>
//#include <st/st.h>
#include <netinet/in.h>

#include <st/unix/tcp.h>

st_send_t sendreq;

static void *
task_blocking (void *p)
{
    st_fd_t fd;
    int n, i;
    int addrlen;
    struct sockaddr_in inaddr;
    jmbuf_t *bufs[4] = { NULL };

    fd = st_socket (AF_INET, SOCK_STREAM, 0);

    memset (&inaddr, 0, sizeof(inaddr));
    inaddr.sin_family = AF_INET;
    inaddr.sin_port = htons (8888);
    inaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    st_bind (fd, (struct sockaddr *) &inaddr, sizeof(inaddr));

    st_listen (fd, 8, NULL);

    for (;;) {
        struct sockaddr addr[2] = { 0 };
        addrlen = sizeof(addr);

        st_fd_t client = st_accept (fd, addr, &addrlen);

#if 1
        bufs[0] = jpkt_alloc ();
        bufs[0]->data_len = bufs[0]->buf_len;
        n = st_send (client, &sendreq, bufs[0], NULL);
        printf ("sent return %d\n", n);
#endif
#if 0
        n = st_recvfrom(client, bufs, &addr, &addrlen, JTIME_INFINITE);
        printf ("received %d bytes\n", n);
#endif
#if 0
        n = st_rx_pkts(fd, bufs, ARRAY_SIZE(bufs), &addr, &addrlen, JTIME_INFINITE);
        for (i = 0; i != ARRAY_SIZE(bufs); ++i) {
            jmbuf_free(bufs[i]);
        }
#endif
    }

    return NULL;
}

static void
send_cb (uv_write_t* req, int status)
{
    ;
}

jmbuf_t *bufs[4] = { NULL };
st_recv_t rreq;

void read2_cb(uv_read_t* req, int status)
{
    ;
}


static void
connection_cb (uv_stream_t* server, int status)
{
    struct sockaddr addr[2] = { 0 };
    int addrlen, n;

    addrlen = sizeof(addr);
    st_fd_t client = st_accept ((st_fd_t) server, addr, &addrlen);

    for (;;) {
#if 0
        bufs[0] = jpkt_alloc ();
        bufs[0]->data_len = bufs[0]->buf_len;
        n = st_send (client, &sendreq, bufs[0], send_cb);
        printf ("sent return %d\n", n);
        break;
#endif
#if 0
        n = st_recvmsg(client, NULL, bufs, ARRAY_SIZE(bufs), NULL);
        printf ("received %d bytes\n", n);
        if (n <= 0) {
            break;
        }
#endif
#if 1
        n = st_recvmsg(client, &rreq, bufs, ARRAY_SIZE(bufs), read2_cb);
        break;
#endif
#if 0
        n = st_rx_pkts(fd, bufs, ARRAY_SIZE(bufs), &addr, &addrlen, JTIME_INFINITE);
        for (i = 0; i != ARRAY_SIZE(bufs); ++i) {
            jmbuf_free(bufs[i]);
        }
#endif
    }
}

static void *
task_callback (void *p)
{
    st_fd_t fd;
    int n, i;
    int addrlen;
    struct sockaddr_in inaddr;

    fd = st_socket (AF_INET, SOCK_STREAM, 0);

    memset (&inaddr, 0, sizeof(inaddr));
    inaddr.sin_family = AF_INET;
    inaddr.sin_port = htons (8888);
    inaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    st_bind (fd, (struct sockaddr *) &inaddr, sizeof(inaddr));

    st_listen (fd, 8, connection_cb);

    st_task_run ();

    return NULL;
}

int
main (int argc, char *argv[])
{
    jerr_t rc;
    st_task_t tid;

    jet_init ();

//    rc = st_task_create (&tid, NULL, task_blocking, NULL);
    rc = st_task_create(&tid, NULL, task_callback, NULL);

    jet_run ();

    return 0;
}
