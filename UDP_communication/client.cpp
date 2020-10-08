#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
using namespace std;
 
#define BUFSIZE 512
 
int main()
{

     // --- create socket() ---
     //(address system, socket type, protocol) AF_INET == IPv4, SOCK_DGRAM == UDP, return socket descriptor 
     int socket_descriptor=socket(AF_INET, SOCK_DGRAM, 0);
     
     // --- bind() ---
     //socket structure sockaddr_in: sin_familty(address system), sin_port(port_number), sin_addr(32bit IP address), sin_zero[8](not used)
     sockaddr_in serveraddr;
     bzero(&serveraddr, sizeof(serveraddr)); //Initialize to zero
     serveraddr.sin_family=AF_INET;
     serveraddr.sin_port=htons(8901); //transform host Byte order into Network Byte order(Big Endian)
     serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1"); // "127.0.0.1"(Host PC)
     
     // variables for communication with clients.
     sockaddr_in peeraddr; //declare to save for sender's address.
     int addrlen;
     char buf[BUFSIZE+1]; //declare buffer to save data to send
     int len;
     
     // --- communication with server ---
     while(1)
     { 
         //input data
         cout << endl << "[data to send] ";
         if(fgets(buf, BUFSIZE+1, stdin) == NULL) {
 	        break;
	}
         
          // remove '\n'
          len=strlen(buf);
          if(buf[len-1]=='\n')
          buf[len-1]='\0';
          if(strlen(buf)==0)
          break;
         
          // --- send data ---
          int return_value;
          return_value = sendto(socket_descriptor, buf, strlen(buf), 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
          cout<<"[UDP Client] " << return_value << "Byte sent." << endl;
         
          // --- recieve data ---
          addrlen = sizeof(peeraddr);
          return_value = recvfrom(socket_descriptor, buf, BUFSIZE, 0, (sockaddr*)&peeraddr, (socklen_t*)&addrlen);

          if(return_value==-1){
	      cout << "failed sending msg. retry" << endl;
	      continue;
          }

          //print recieved data
          buf[return_value]='\0';
          cout << "[UDP Client] " << return_value << "Byte recieved." << endl;
          cout << "[recieved data] "<< buf << endl;
         }

     // socket connection terminate
     close(socket_descriptor);

 return 0;
}
