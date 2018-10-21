#include "real_address.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

const char * real_address(const char *address, struct sockaddr_in6 *rval)
{
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  int err = getaddrinfo(address, NULL, &hints, &res);

  if (err != 0)
  {
    return gai_strerror(err);
  }

  memcpy(rval, res->ai_addr, res->ai_addrlen);
  free(res);
  return NULL;
}
