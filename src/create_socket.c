#include "create_socket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int create_socket(struct sockaddr_in6 *source_addr, int src_port, struct sockaddr_in6 *dest_addr, int dst_port)
{
    int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        fprintf(stderr,"Une erreur est survenue lors de la création du soquet.");
        return -1;
    }

    if(src_port > 0)
    {
        if(source_addr != NULL)
        {
            source_addr->sin6_port = htons(src_port);
            int bd = bind(sock, (struct sockaddr*) source_addr, sizeof(struct sockaddr_in6));
            if(bd != 0)
            {
                fprintf(stderr, "Une erreur est survenue lors de l'assignation de l'adresse au soquet.");
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "Une erreur est survenue : la source n'a pas d'adresse.");
            return -1;
        }
    }
    else
    {
        if(dest_addr != NULL)
        {
            dest_addr->sin6_port = htons(dst_port);
            int cnct = connect(sock, (struct sockaddr*) dest_addr, sizeof(struct sockaddr_in6));
            if(cnct != 0)
            {
                fprintf(stderr, "Une erreur est survenue lors de la connexion du socket.");
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "Une erreur est survenue : la destination n'a pas d'adresse.");
            return -1;
        }
    }
    return sock;
}
