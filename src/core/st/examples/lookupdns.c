/*
 * Portions created by SGI are Copyright (C) 2000 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Silicon Graphics, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "st.h"

#if !defined(NETDB_INTERNAL) && defined(h_NETDB_INTERNAL)
#define NETDB_INTERNAL h_NETDB_INTERNAL
#endif

/* Resolution timeout (in microseconds) */
#define TIMEOUT (2*1000000LL)

/* External function defined in the res.c file */
int dns_getaddr(const char *host, struct in_addr *addr, st_utime_t timeout);


void *do_resolve(void *host)
{
  struct in_addr addr;

  printf("resolving %-40s\n", (char *)host);

  /* Use dns_getaddr() instead of gethostbyname(3) to get IP address */
  if (dns_getaddr(host, &addr, TIMEOUT) < 0) {
    fprintf(stderr, "dns_getaddr: can't resolve %s: ", (char *)host);
    if (h_errno == NETDB_INTERNAL)
      perror("");
    else
      herror("");
  } else
    printf("%-40s %s\n", (char *)host, inet_ntoa(addr));

  return NULL;
}

void bar(int a, int b, int c, int d)
{
    int i = rand();
    printf("%d: %d,%d,%d,%d,%d,\n", __LINE__, i,a,b,c,d);
}
//st_mutex_t lock;
void *unlock(void *p)
{
//    st_mutex_unlock(lock);
//    st_thread_exit(NULL);
    bar(2,4,6,8);
    float f = 1234.5678;
    st_usleep(1);
}


void foo(int a, int b, int c, int d)
{
    int i = rand();
    printf("%d: %d,%d,%d,%d,%d,\n", __LINE__, i,a,b,c,d);
  //  st_mutex_lock(lock);
    st_usleep(1);
    printf("%d: %d,%d,%d,%d,%d,\n", __LINE__, i,a,b,c,d);
}
/*
 * Asynchronous DNS host name resolution. This program creates one
 * ST thread for each host name (specified as command line arguments).
 * All threads do host name resolution concurrently.
 */
int main(int argc, char *argv[])
{

    if (st_init() < 0) {
      perror("st_init");
      exit(1);
    }
#if 0
  int i;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <hostname1> [<hostname2>] ...\n", argv[0]);
    exit(1);
  }


  for (i = 1; i < argc; i++) {
    /* Create a separate thread for each host name */
    if (st_thread_create(do_resolve, argv[i], 0, 0) == NULL) {
      perror("st_thread_create");
      exit(1);
    }
  }
#else
//  lock = st_mutex_new();
//  st_mutex_lock(lock);
  st_thread_create(unlock, NULL, 0, 0);
  double f = 8765.4321;
  f += 1234.9999;
  foo(1,3,5,7);
#endif

  st_thread_exit(NULL);

  /* NOTREACHED */
  return 1;
}

