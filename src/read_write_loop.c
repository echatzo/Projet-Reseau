#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

void read_write_loop(int sfd)
{
        struct pollfd fds[2];

        int timeout;
        int size;
        int ready;

        // Initialisation des buffers
        char* r_buf = (char*) malloc(1024*sizeof(char));
        char* s_buf = (char*) malloc(1024*sizeof(char));

        // Vérifications malloc()
        if(r_buf == NULL)
        {
                fprintf(stderr, "Erreur lors de la fonction malloc() pour r_buf.\n");
                free(s_buf);
                return;
        }
        if(s_buf == NULL)
        {
                fprintf(stderr, "Erreur lors de la fonction malloc() pour s_buf.\n");
                free(r_buf);
                return;
        }

        // boucle "infinie"
        while(1)
        {
                // initialisation du poll
                fds[0].fd = sfd;
                fds[1].fd = sfd;
                fds[0].events = POLLIN;
                fds[1].events = POLLOUT;

                // initialisation des variables nécessaires
                timeout = 10000;
                ready = poll(fds, 2, timeout);

                memset((void*) r_buf, 0, 1024);
                memset((void*) s_buf, 0, 1024);

                // Vérification qu'il y a encore à lire
                if(fgets(s_buf, 1024, stdin) == NULL)
                {
                        free(s_buf);
                        free(r_buf);
                        return;
                }
                size = strlen(s_buf);

                // envoi des données
                send(sfd, s_buf, size, 0);

                if(ready > 0) // tout s'est bien passé
                {
                        if(fds[0].revents != 0) // un événement a eu lieu en réception
                        {
                                recv(sfd, r_buf, 1024, 0);
                        }
                        if(fds[1].revents != 0) // un événement a eu lieu en envoi
                        {
                                int err = write(1, r_buf, size);
                                if(err == -1)
                                {
                                        free(s_buf);
                                        free(r_buf);
                                        return;
                                }
                        }
                }
                else if(ready == 0) // dépassement du délai d'attente
                {
                        fprintf(stderr, "Dépassement du délai d'attente, aucun descripteur de fichier n'était prêt.\n");
                }
                else
                {
                        fprintf(stderr, "Erreur lors de l'utilisation de poll() pour ready.\n");
                }

        }

        free(s_buf);
        free(r_buf);
        return;
}
