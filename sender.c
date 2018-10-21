/* @Title : Project for the course "[LINGI1341] Computer Networks : information transfer" (sender part)
 * @Authors : Edouard Chatzopoulos (3740 16 00) && Beno�t Michel (2397 16 00)
 * @Date : 22/10/2018
 */

/*
 * Sender part : send data packets on the network.
 */

#include "create_socket.h"
#include "packet_interface.h"
#include "real_address.h"
#include "read_write_loop.h"

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
		fprintf(stderr, "Too few arguments\n Correct usage :\n $ sender [-f] [filename] <hostname> <port>\n");
		return 0;
	}

  int opt;
  char *fichier;
  char *hostname;
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
  hostname = argv[optind];
}
//looks for the port number
port = atoi(argv[optind+1]);
 }

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
