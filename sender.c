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
    // TO DO
}

// Helpful functions
