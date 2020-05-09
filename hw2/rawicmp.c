#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#define PACK_LEN 1024

// function to calculate Internet checksum
unsigned short in_cksum(unsigned short *addr, int len)
{
  int nleft = len;
  int sum = 0;
  unsigned short *w = addr;
  unsigned short answer = 0;

  while (nleft > 1) 
  {
    sum += *w++;
    nleft -= 2;
  }

  if (nleft == 1) 
  {
    *(unsigned char *) (&answer) = *(unsigned char *) w;
    sum += answer;
  }
  
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;
  return (answer);
}
void  print_usage()
{
    printf("USAGE: sudo ./rawicmp [SRC_IP] [DST_IP]");
    return;
}
// main function of spoofing program
int main(int argc, char **argv)
{
  struct ip ip;
  struct udphdr udp;
  struct icmp icmp;
  int sd;
  int len;
  struct sockaddr_in sin;
  u_char* packet = NULL;
  char msg[20];

  char *src_ip = NULL;
  char *dst_ip = NULL;
  // handle inputted IP
  if (argc != 3)
  {
    print_usage();
    exit(0);
  }
  src_ip = argv[1];
  dst_ip = argv[2];

  strncpy(msg, "CSCE665", strlen("CECE665"));
  // allocate space for spoofing packet
  len = PACK_LEN;
  packet = (u_char *)malloc(len);
  if (packet == NULL) {
    perror("allocate failed!");
    exit(-1);
  }
  
  //IP header construct
  ip.ip_v 	= 	0x4;		// IPv4
  ip.ip_hl 	= 	0x5;		// Internet Header Length
  ip.ip_tos 	= 	0x0;		// DSCP
  ip.ip_len 	= 	sizeof(struct ip) + sizeof(struct icmp) + strlen(msg);		// Total length
  ip.ip_id 	= 	0;		// Identification
  ip.ip_off 	= 	0x0;		// Fragment offset
  ip.ip_ttl 	= 	64;		// Time To Live
  ip.ip_p 	= 	IPPROTO_ICMP;	// Protocal used in data portion
  ip.ip_sum 	= 	0x0;		// Checksum. Set to zero first
  ip.ip_src.s_addr = inet_addr(src_ip);	// Source IP
  ip.ip_dst.s_addr = inet_addr(dst_ip);	// Destination IP
  
  memcpy(packet, &ip, sizeof(ip));
  
  
  //ICMP header construct
  icmp.icmp_type 	= 	ICMP_ECHO;	// ICMP type
  icmp.icmp_code 	= 	0;		// ICMP subtype
  icmp.icmp_cksum 	= 	0;		// Checksum. 
  icmp.icmp_id 		= 	3333;		// Identifier. Set it to a random number first
  icmp.icmp_seq 	= 	0x0;		// Sequence number
  // calculate checksum value of ICMP header    ??? htons
  icmp.icmp_cksum = in_cksum((unsigned short *)&icmp, sizeof(struct icmp));
  // ICMP header done. Append the ICMP header to the buffer after IP header
  memcpy(packet + sizeof(struct ip), &icmp, sizeof(icmp));
  

  // Create a raw socket with IP protocal
  if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) 
  {
    perror("socket() error");
    exit(-1);
  }
  // data structure for raw socket 
  sin.sin_family = AF_INET;
  // send out the packet
  if (sendto(sd, packet, len, 0, (struct sockaddr *)&sin, 
	     sizeof(struct sockaddr)) < 0)  
  {
    perror("sendto() error");
    exit(-1);
  }
  
  return 0;
}
