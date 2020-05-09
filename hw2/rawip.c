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

void  print_usage()
{
    printf("USAGE: sudo ./rawip [SRC_IP] [DST_IP]");
    return;
}

int main(int argc, char* argv[])
{
    struct ip ip;
    
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

    len = PACK_LEN;

    packet = (u_char *)malloc(len);
    if (packet == NULL) {
        perror("failed to alloc ");
        return -1;
    }
    //IP header construct
    ip.ip_v 	= 	0x4;		// IPv4
    ip.ip_hl 	= 	0x5;		// Internet Header Length
    ip.ip_tos 	= 	0x0;		// DSCP
    ip.ip_len 	= 	len;		// Total length
    ip.ip_id 	= 	0;		// Identification
    ip.ip_off 	= 	0x0;		// Fragment offset
    ip.ip_ttl 	= 	64;		// Time To Live
    ip.ip_p 	= 	IPPROTO_RAW;	// Protocal used in data portion
    ip.ip_sum 	= 	0x0;		// Checksum. Set to zero first
    ip.ip_src.s_addr = inet_addr(src_ip);	// Source IP
    ip.ip_dst.s_addr = inet_addr(dst_ip);	// Destination IP
    // calculate checksum value of IP header
    // ip.ip_sum = in_cksum((unsigned short *)&ip, sizeof(ip));
    // IP header done. Copy to the buffer
    memcpy(packet, &ip, sizeof(ip));

    //fill raw IP packet
    strcpy(msg, "hahaha");
    memcpy(packet + sizeof(struct ip), msg, strlen(msg));

    // Create a raw socket with IP protocal
    if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) 
    {
        perror("socket() error");
        exit(-1);
    }
    // data structure for raw socket 
    sin.sin_family = AF_INET;
    // send out the packet
    if (sendto(sd, packet, len, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)  
    {
        perror("sendto() error");
        exit(-1);
    }
  
    return 0;
}