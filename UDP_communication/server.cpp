#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
 
#define BUFSIZE 512

int main()
{

     // --- create socket() ---
     //(address system, socket type, protocol) AF_INET == IPv4, SOCK_DGRAM == UDP, return socket descriptor 
     int socket_descriptor=socket(AF_INET, SOCK_DGRAM, 0); 
     int return_value;
     
     // --- bind() ---
     //socket structure sockaddr_in: sin_familty(address system), sin_port(port_number), sin_addr(32bit IP address), sin_zero[8](not used)
     sockaddr_in serveraddr;
     bzero(&serveraddr, sizeof(serveraddr)); //Initialize to zero
     serveraddr.sin_family=AF_INET;
     serveraddr.sin_port=htons(8901); //transform host Byte order into Network Byte order(Big Endian)
     serveraddr.sin_addr.s_addr=htonl(INADDR_ANY); //transform host Byte order into Network Byte order(Big Endian)
     return_value=bind(socket_descriptor, (sockaddr*)&serveraddr, sizeof(serveraddr)); //designate IP address and Port number for socket.
     
     // variables for communication with clients.
     sockaddr_in clientaddr;
     int addrlen;
     char buf[BUFSIZE+1];
     
     char can_byte[8];


    // --- communication with client ---
    while(1)
    { 

          // --- recieve data from socket ---
          addrlen=sizeof(clientaddr);
          //return value is length of recieved Byte
          return_value=recvfrom(socket_descriptor, buf, BUFSIZE, 0, (sockaddr*)&clientaddr, (socklen_t*)&addrlen);

          //print recieved data
          buf[return_value]='\0';
          //inet_ntoa == transform network Byte order(Big Endian) to Decimal address.
	  //ntohs == transform Network Byte order(Big Endian) into host Byte order. 
          cout << "[UDP/" << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << endl;
      
          // CAN test ------
          copy(buf, buf+8, can_byte);
          
          for(int i=0;i<8;i++){
             cout << "[" << i << "]: " << can_byte[i] << " " << endl;
          }
          // -------

        
          // --- send data ---
          //sendto == send data to socket
	  //socket_descriptor, address to send data, data length, 0, object address, size of object address) 
          return_value=sendto(socket_descriptor, buf, return_value, 0, (sockaddr*)&clientaddr, sizeof(clientaddr));

     }
     
     // socket connection terminate
     close(socket_descriptor);

 return 0;
} 
