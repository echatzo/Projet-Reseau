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

/*
 * Main Function
 */

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
		fprintf(stderr, "Too few arguments\n Correct usage :\n $ receiver [-f] [filename] <hostname> <port>\n");
		return 0;
	}
  else
  {
    int opt;
    char *fichier;
    char *host;
    int port;
    uint8_t filelinked=0; //is a file already specified
    while ((opt = getopt(argc, argv, "f:")) != -1) {
      switch (opt){

        //looks for a specified file
        case 'f':
        fichier=optarg;
        if (filelinked==1){
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
   if(strstr(argv[optind], "::") != NULL){
   }
   else{
     host = argv[optind];
   }
   //looks for the port number
   port = atoi(argv[optind+1]);
   //check if the port and host are absent
   if(!host) fprintf(stderr, "Hostname is NULL\n");

    if(!port) fprintf(stderr, "Port is 0\n");

    struct sockaddr_in6 src_addr;
    ra = real_address(host,&src_addr);
    if (ra){
      fprintf(stderr, "Cannot resolve the hostname %s : %s\n",sender,err);
    }
    //creation of the socket
    int sfd = create_socket(&src_addr, port,NULL, -1);
    if (sfd > 0 && wait_for_client(sfd) < 0) {
      fprintf(stderr, "Could not connect the socket.\n");
      close(sfd);
      return EXIT_FAILURE;
    }
  }

    // TO DO

}

//checks if the file is valid
int isavalidfile(char *file){
  int fd=fopen(file, "r")
  if(fd>0){
    if(fclose(fd)>0){
      return 1;
    }
  }
  return 0;
}
}
