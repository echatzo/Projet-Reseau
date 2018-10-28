/* @Title : Project for the course "[LINGI1341] Computer Networks : information transfer" (receiver part)
 * @Authors : Edouard Chatzopoulos (3740 16 00) && Benoit Michel (2397 16 00)
 * @Date : 22/10/2018
 */

/*
 * Receiver part : receive data packets from the network.
 */

 #include "create_socket.h"
 #include "packet_interface.h"
 #include "real_address.h"
 #include "read_write_loop.h"
 #include "wait_for_client.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>

#define PKT_MAX_PAYLOAD 512
#define MSG_LEN 524
#define WINDOW_MAX_SIZE 31

/*
 * Main Function
 */

int isavalidfile(char *file);

int main(int argc, char *argv[])
{
        if(argc < 2)
        {
                fprintf(stderr, "Too few arguments\n Correct usage :\n $ receiver [-f] [filename] <hostname> <port>\n");
                return 0;
        }

        int opt;
        int port;

        char *fichier = malloc(1024);
        if(fichier == NULL)
        {
                fprint(stderr, "Erreur lors de l'initialisation de fichier.");
                return 1;
        }
        char *host = (char *) malloc(1024);
        if(host == NULL)
        {
                fprint(stderr, "Erreur lors de l'initialisation de host.");
                free(fichier);
                return 1;
        }

        uint8_t filelinked=0; //is a file already specified
        while ((opt = getopt(argc, argv, "f:")) != -1)
        {
                switch (opt)
                {

                //looks for a specified file
                case 'f':
                        fichier=optarg;
                        if (filelinked==1)
                        {
                                fprintf(stderr, "A file is already specified");
                                exit(EXIT_FAILURE);
                        }
                        else if(isavalidfile(fichier)<0)
                        {
                                fprintf(stderr, "Not a valid file");
                                exit(EXIT_FAILURE);
                        }
                        else
                        {
                                filelinked==1;
                                break;
                        }

                //looks for unknown arguments
                case '?':
                        fprintf(stderr, "Unknown argument");
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        if(strstr(argv[optind], "::") != NULL)
        {
        }
        else
        {
                host = argv[optind];
        }
        //looks for the port number
        port = atoi(argv[optind+1]);
        //check if the port and host are absent
        if(!host) fprintf(stderr, "Hostname is NULL\n");
        if(!port) fprintf(stderr, "Port is 0\n");

        //generation of the address
        struct sockaddr_in6 src_addr;
        const char *ra = real_address(host,&src_addr);
        if (ra)
        {
                fprintf(stderr, "Cannot resolve the hostname %s : %s\n",host,err);
                return EXIT_FAILURE;
        }

        //creation of the socket
        int sfd = create_socket(&src_addr, port, NULL, -1);
        if (sfd > 0 && wait_for_client(sfd) < 0)
        {
                fprintf(stderr, "Could not connect the socket.\n");
                close(sfd);
                return EXIT_FAILURE;
        }
        if (sfd < 0)
        {
                fprintf(stderr, "Cannot create socket\n");
                return EXIT_FAILURE;
        }

        //Création du buffer
        char *r_buffer = (char*) malloc(sizeof(char)*MSG_LEN);
        if(r_buffer == NULL)
        {
                fprint(stderr, "Erreur lors de l'initialisation de r_buffer.");
                return 1;
        }
        pkt_status_code pkt_err;

        pkt_t_node **head = (pkt_t_node **) malloc(sizeof(pkt_t_node**));
        if(list_head == NULL)
        {
                fprint(stderr, "Erreur lors de l'initialisation de head.");
                return 1;
        }
        *head = NULL;

        pkt_t_node **tail = (pkt_t_node **) malloc(sizeof(pkt_t_node**));
        if(list_tail == NULL)
        {
                fprint(stderr, "Erreur lors de l'initialisation de tail.");
                return 1;
        }
        *list_tail = NULL;

        uint8_t current_seq = 0;
        uint8_t current_window_size = WINDOW_MAX_SIZE;
        int eof = 0;

        while(!eof)
        {
                pkt_t *pkt = pkt_new();
                pkt_err = pkt_decode(buffer, size_read, pkt);

                if(pkt_stat == E_LENGTH) fprintf(stderr, "decode : erreur avec le champ length\n");
                if(pkt_stat == E_UNCONSISTENT) fprintf(stderr, "decode : paquet inconsistent\n");
                if(pkt_stat == E_NOHEADER) fprintf(stderr, "decode : pas de header\n");
                if(pkt_stat == E_CRC) fprintf(stderr, "decode : erreur de CRC, : %d\n", seq_actual);
                if(pkt_stat == E_WINDOW) fprintf(stderr, "decode : erreur avec le champ window\n");
                if(pkt_stat == E_TYPE) fprintf(stderr, "decode : erreur avec le champ type\n");
        }


        //Fermeture du fichier
        if(filelinked==1)
        {
                int close_err = close(fd);
                if(close_err == -1)
                {
                        fprintf(stderr, "Erreur lors de la fermeture du fichier.\n");
                        return -1;
                }
        }

        free(head);
        free(tail);
        free(r_buffer);
        free(host);
        free(fichier);

        //Fermeture du socket
        int last_err = close(sfd);
        if(last_err == -1)
        {
                fprintf(stderr, "Erreur lors de la fermeture du socket.\n");
                return -1;
        }

        return 0;
}




//checks if the file is valid
int isavalidfile(char *file)
{
        int fd=fopen(file, "r")
                if(fd>0)
        {
                if(fclose(fd)>0)
                {
                        return 1;
                }
        }
        return 0;
}
