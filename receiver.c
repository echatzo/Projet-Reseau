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

    // TO DO
}

// Helpful functions
