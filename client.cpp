#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#define MAXLINE 1024
#include <iostream>
using namespace std;
#define TIMEOUT_MS 200     /* Seconds between retransmits */
int main(int argc,char* argv[])
{
int PORT=8080;
int sockfd;
int n;
socklen_t len;
string msg;
char sendline[1024],recvline[1024];
struct sockaddr_in servaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");

string RecPORT,ClientRecPORT;
cout<<"Enter PORT NO OF SERVER For Connection"<<endl;
cin>>PORT;
servaddr.sin_port=htons(PORT);
connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
strcpy(sendline,"Connection");
len=sizeof(servaddr);
sendto(sockfd,sendline,MAXLINE,0,(struct sockaddr*)&servaddr,len);
cout<<"Done"<<endl;
while(1)
{
    int choice=0;
    cout<<"Do you want to send message or wait for receiving, 1 or send and 2 for recv"<<endl;
    cin>>choice;
    if(choice==1)
    {
        cout<<"Enter PORT No OF Client of Receiving Server"<<endl;
        cin>>ClientRecPORT;
        cout<<"Enter Your message"<<endl;
        cin>>msg;
        msg= "C-P("+ClientRecPORT+") :"+msg+"\nHope: 4"; 
        for(int i=0;i<msg.length();i++)
            sendline[i]=msg[i];
        sendline[msg.length()]='\0';
        cout<<sendline<<endl;
        sendto(sockfd,sendline,MAXLINE,0,(struct sockaddr*)&servaddr,len);
    }
   else
   {
       int num_timeouts=0;
      struct timeval tv;
       tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
       perror("Error");
       }
        bool check=0;
       cout<<"I am Wating for Receiving message"<<endl;
         while(recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL)<0)
         {
            num_timeouts++;
            if(num_timeouts==200)
                {
                    check=1;
                    break;
                }
         }
         if(check==1)
         {
             cout<<"Time Out"<<endl;
            cout<<num_timeouts<<endl;
         }
         else
         {
             recvline[strlen(recvline)]='\0';
             printf("\n Server's Echo : %s\n\n",recvline);
         }
         //recvline[n]='\0';  
   }
   
   

}



return 0;
}