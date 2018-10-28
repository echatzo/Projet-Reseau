#include "packet_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <arpa/inet.h>
#include <math.h>

struct __attribute__((__packed__)) pkt
{
								char* payload;
								// on n'oublie pas d'inverser le sens des bits pour le premier byte
								uint8_t window : 5;
								uint8_t tr : 1;
								ptypes_t type : 2;
								uint8_t seq;
								uint16_t length;
								uint32_t timestamp;
								uint32_t crc1;
								uint32_t crc2;
};


pkt_t* pkt_new()
{
								return calloc(1, sizeof(pkt_t));
}

void pkt_del(pkt_t *pkt)
{
								if(pkt->type == PTYPE_DATA)
								{
																free(pkt->payload);
								}
								free(pkt);
}






pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
								unsigned char cpy;

								// on vérifie qu'on a paquet cohérent
								if(len <= 0)
								{
																return E_UNCONSISTENT;
								}
								// on vérifie qu'on un header
								if(len <= 12)
								{
																return E_NOHEADER;
								}

								// on récupère le premier byte
								memcpy(&cpy, data, 1);
								// on récupère le second byte
								memcpy(&pkt->seq, data+1, 1);
								// on récupère le troisième et quatrième byte
								memcpy(&pkt->length, data+2, 2);
								pkt->length = ntohs(pkt->length);
								// on récupère les 4 bytes suivants
								memcpy(&pkt->timestamp, data+4, 4);

								// on divise le premier byte
								pkt->type = cpy >> 6;
								pkt->tr = cpy >> 5;
								pkt->window = cpy;


								fprintf(stderr, "Valeur du type : %d\n", pkt->type);
								fprintf(stderr, "Valeur du tr : %d\n", pkt->tr);
								fprintf(stderr, "Valeur du window : %d\n", pkt->window);
								fprintf(stderr, "Valeur du seq : %d\n", pkt->seq);
								fprintf(stderr, "Valeur du length : %d\n", pkt->length);
								fprintf(stderr, "Valeur du timestamp : %d\n", pkt->timestamp);


								// on vérifie qu'on a un paquet valide
								if((pkt_get_type(pkt) != PTYPE_DATA) && (pkt_get_type(pkt) != PTYPE_ACK) && (pkt_get_type(pkt) != PTYPE_ACK))
								{
																return E_TYPE;
								}

								// on regarde la longueur du payload et si le paquet est tronqué
								//if (pkt_get_length(pkt) == 0)
								//{
								//if(pkt_get_tr(pkt) != 0)
								//{
								//return E_TR;
								//}
								//fprintf(stderr, "ici ?");
								//return E_LENGTH;
								//}

								// on génère la vérification du crc1
								uint32_t new_crc1 = crc32(0L, (const unsigned char*) data, 8);

								// on récupère le crc1
								memcpy(&pkt->crc1, data+8, 4);
								pkt->crc1 = ntohl(pkt->crc1);


								fprintf(stderr, "Valeur du crc1 : %d\n", pkt->crc1);
								fprintf(stderr, "Valeur du new_crc1 : %d\n", new_crc1);


								// comparaison crc1 et vérification
								if (pkt->crc1 != new_crc1)
								{
																return E_CRC;
								}

								// on vérifie, si on a un paquet tronqué, qu'il est cohérent
								if(pkt_get_tr(pkt) != 0)
								{
																if(pkt_get_type(pkt) != PTYPE_DATA)
																{
																								return E_UNCONSISTENT;
																}
																if(pkt->length != 0)
																{
																								return E_LENGTH;
																}
																pkt_set_payload(pkt, NULL, 0);
																return PKT_OK;
								}

								// on génère la vérification du crc2
								uint32_t new_crc2 = crc32(0L, (const unsigned char*) data+12,                     (pkt_get_length(pkt)/256));

								// on récupère le crc2
								memcpy(&pkt->crc2, data+(12+pkt->length), 4);
								fprintf(stderr, "Valeur du crc2 : %d\n", pkt->crc2);
								pkt->crc2 = ntohl(pkt->crc2);


								fprintf(stderr, "Valeur du crc2 : %d\n", pkt->crc2);
								fprintf(stderr, "Valeur du new_crc2 : %d\n", new_crc2);
								fprintf(stderr, "Valeur du new_crc2 : %d\n", htonl(new_crc2));




								// On récupère le payload
								//if((len-16)*256 != pkt_get_length(pkt))
								//{
								//fprintf(stderr, "Valeur de (len-16) : %d et valeur de length : %d", (int)         (len-16), pkt_get_length(pkt));
								//fprintf(stderr, "ou là ?");
								//return E_LENGTH;
								//}

								pkt_set_payload(pkt, data+12, pkt->length);


								fprintf(stderr, "Contenu du chargement : %s \n", pkt->payload);
								fprintf(stderr, "Valeur de (len-16) : %d et valeur de length : %d", (int)         (len-16), pkt_get_length(pkt));

								return PKT_OK;
}








pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
								uint16_t length;
								uint32_t timestamp = pkt_get_timestamp(pkt);
								size_t size_payload = pkt_get_length(pkt);

								// on vérifie qu'on a un paquet de données
								if(pkt_get_type(pkt) != PTYPE_DATA)
								{
																return E_TYPE;
								}
								// on vérifie qu'on a une taille correcte
								if(pkt_get_length(pkt) > MAX_PAYLOAD_SIZE || size_payload > (*len)-12)
								{
																return E_LENGTH;
								}
								// on met à jour la taille du payload
								if(size_payload == 0)
								{
																length = 0;
								}
								else
								{
																length = pkt_get_length(pkt);
								}
								length = htons(length);

								// on encode le premier byte
								*buf = (pkt_get_type(pkt) << 6) + (pkt_get_tr(pkt) << 5) + pkt_get_window(pkt);
								// on encode le second byte
								*(buf+1) = pkt_get_seqnum(pkt);
								// on encode les troisième et quatrième bytes
								memcpy(buf+2, &length, 2);
								// on encode les 4 bytes suivants
								memcpy(buf+4, &timestamp, 4);

								// génération du CRC 1
								uint32_t crc1 = crc32(0L, (unsigned char*) buf, 8);
								crc1 = htonl(crc1);

								// on encode le crc1
								memcpy(buf+8, &crc1, 4);

								if(length != 0) // si on a un payload
								{
																// on encode le payload
																memcpy(buf+12, pkt->payload, size_payload);
								}

								if(pkt_get_tr(pkt) == 0) // si le paquet n'est pas tronqué
								{
																// génération du CRC 2
																uint32_t crc2 = crc32(0L, (unsigned char*) buf+12, size_payload);
																crc2 = htonl(crc2);

																// on encode le crc2
																memcpy(buf+size_payload+12, &crc2, 4);
								}
								*len = size_payload + 16;
								return PKT_OK;
}


ptypes_t pkt_get_type(const pkt_t* pkt)
{
								return pkt->type;
}
uint8_t  pkt_get_tr(const pkt_t* pkt)
{
								return pkt->tr;
}
uint8_t  pkt_get_window(const pkt_t* pkt)
{
								return pkt->window;
}
uint8_t  pkt_get_seqnum(const pkt_t* pkt)
{
								return pkt->seq;
}
uint16_t pkt_get_length(const pkt_t* pkt)
{
								return ntohs(pkt->length);
}
uint32_t pkt_get_timestamp(const pkt_t* pkt)
{
								return pkt->timestamp;
}
uint32_t pkt_get_crc1(const pkt_t* pkt)
{
								return ntohl(pkt->crc1);
}
uint32_t pkt_get_crc2(const pkt_t* pkt)
{
								if(pkt_get_length(pkt) == 0)
								{
																return E_CRC;
								}
								return ntohl(pkt->crc2);
}
const char* pkt_get_payload(const pkt_t* pkt)
{
								return pkt->payload;
}


pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
								pkt->type = type;
								return PKT_OK;
}
pkt_status_code pkt_set_tr(pkt_t *pkt, const uint8_t tr)
{
								pkt->tr = tr;
								return PKT_OK;
}
pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
								pkt->window = window;
								return PKT_OK;
}
pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
								pkt->seq = seqnum;
								return PKT_OK;
}
pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
								pkt->length = htons(length);
								return PKT_OK;
}
pkt_status_code pkt_set_timestamp(pkt_t *pkt, const uint32_t timestamp)
{
								pkt->timestamp = timestamp;
								return PKT_OK;
}
pkt_status_code pkt_set_crc1(pkt_t *pkt, const uint32_t crc1)
{
								pkt->crc1 = htonl(crc1);
								return PKT_OK;
}
pkt_status_code pkt_set_crc2(pkt_t *pkt, const uint32_t crc2)
{
								if(pkt->length == 0)
								{
																return E_CRC;
								}
								pkt->crc2 = htonl(crc2);
								return PKT_OK;
}
pkt_status_code pkt_set_payload(pkt_t *pkt, const char *data, const uint16_t length)
{
								pkt->payload  = (char*) malloc(length*sizeof(char));
								if(pkt->payload == NULL)
								{
																return E_NOMEM;
								}
								pkt_set_length(pkt, length);
								memcpy(pkt->payload, data, length);
								return PKT_OK;
}
