#include "wait_for_client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>

int wait_for_client(int sfd)
{
        struct sockaddr_in6 src_addr;
        socklen_t addrlen = sizeof(src_addr);
        size_t recv = recvfrom(sfd, NULL, 0, MSG_PEEK, (struct sockaddr*) &src_addr, &addrlen);
        if(recv != 0)
        {
                fprintf(stderr, "Erreur lors de la réception du message depuis le soquet.");
                return -1;
        }

        int cnct = connect(sfd, (struct sockaddr*) &src_addr, sizeof(src_addr));
        if(cnct != 0)
        {
                fprintf(stderr,"Erreur lors de la connexion avec le soquet.");
                return -1;
        }
        return 0;
}
