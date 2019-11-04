
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "dijkistra.cpp"
using namespace std;

//#define PORT	 8080 
#define MAXLINE 1024 
#define IP "127.0.0.1"
// Driver code 

vector<int> readRoutingFile(vector<vector<int>> &matrix,vector<int> &clinetArray)
{
	vector<int> PORTNO;
	ifstream objread("RouterTable.txt");
	int noOfPort,index=0;
    
	if(objread.is_open())
	{
		int val;
		objread>>noOfPort;
		for(int i=0;i<noOfPort;i++)
			{
				objread>>val;
				PORTNO.push_back(val);
			}
		vector<int> clientArray1;
		objread>>index;
		for(int i=0;i<index;i++)
			{
				objread>>val;
				clientArray1.push_back(val);
			}
		vector<vector<int>> temp(noOfPort);
		for(int i=0;i<noOfPort;i++)
		{
			temp[i]=vector<int>(noOfPort);
			for(int j=0;j<noOfPort;j++)
				{
					objread>>val;
					temp[i][j]=val;
				}
		}
		clinetArray=clientArray1;
		matrix=temp;
	}
	objread.close();
	return PORTNO;
}
int indexOfPort(vector<int> PORTNO,int port)
{
	for(int i=0;i<PORTNO.size();i++)
		if(PORTNO[i]==port)
			return i;
	return -1;
}

vector<string> removeDupWord(string str) 
{ 
	vector<string> pars;
   string word = ""; 
   for (int i=0;i<str.length();i++) 
   { 
       if (str[i] == ' ') 
       { 
           pars.push_back(word); 
           word = ""; 
       } 
       else
       { 
           word = word + str[i]; 
       } 
   }  
   pars.push_back(word);
	return pars;
}

void writeRoutingFile(vector<int> PORTNO,vector<vector<int>> matrix,vector<int> clientArray)
{
	ofstream obj;
    obj.open("RouterTable.txt");
	obj<<PORTNO.size()<<endl;
	for(int i=0;i<PORTNO.size();i++)
	{
		if(i!=PORTNO.size()-1)
			obj<<PORTNO[i]<<" ";
		else
			obj<<PORTNO[i]<<endl;
	}
	obj<<clientArray.size()<<endl;
	for(int i=0;i<clientArray.size();i++)
	{
		if(i!=clientArray.size()-1)
			obj<<clientArray[i]<<" ";
		else
			obj<<clientArray[i]<<endl;
	}
	for(int i=0;i<matrix.size();i++)
	{
		for(int j=0;j<matrix.size();j++)
			obj<<matrix[i][j]<<" ";
		obj<<endl;
	}
    obj.close();
}

int main() { 
	int PORT=8080;
	vector<int> avalibleClients;
	vector<int> blockedClients;
   	int sockfd;
	int n;
	int position;
	socklen_t len; 
	char buffer[1024];
	struct sockaddr_in servaddr,cliaddr,servaddr1;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	bzero(&servaddr,sizeof(servaddr));
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = inet_addr(IP); 
	servaddr.sin_port = htons(PORT); 
	// Bind the socket with the server address
	 cout<<"Hello in am udp Server"<<endl;
	bool check;
    do
	{
        check=false;
		if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ) 
		{ 
		perror("bind failed\n"); 
		++PORT;
		servaddr.sin_port = htons(PORT);
        check=true;
        }
        
	}while(check);
	
	vector<vector<int>> matrix;
	vector<int> clientArray;
	vector<int> PORTNO=readRoutingFile(matrix,clientArray);
	PORTNO.push_back(PORT);
	position=PORTNO.size()-1;
	if(matrix.size()!=0)
	{
		vector<int> temp(matrix.size());
		for(int i=0;i<temp.size();i++)
			temp[i]=0;
		matrix.push_back(temp);
		for(int i=0;i<matrix.size();i++)
			matrix[i].push_back(0);
	}
	else
	{
		vector<int> temp={0};
		matrix.push_back(temp);		
	}
	writeRoutingFile(PORTNO,matrix,clientArray);

	PORTNO=readRoutingFile(matrix,clientArray);
		/* code */
	cout<<"My Port no is: "<<PORT<<endl;
    int choice;
    
	while(1)
	{
		cout<<"If you Want to direct communicate with other server/Routers than press 1"<<endl;
		cout<<"If you Want to set Block Clients for This Router than press 2"<<endl;
		cout<<"If you want to Wait Press 3"<<endl;
		cin>>choice;
		if(choice ==1)
		{
			PORTNO=readRoutingFile(matrix,clientArray);
			for(int i=0,j=0;i<PORTNO.size();i++)
				{
					if(clientArray.size()!=0)
						if(i!=clientArray[j])
							cout<<i<<": PORT NO"<<PORTNO[i]<<endl;
						else
							j++;
					else
						cout<<i<<": PORT NO"<<PORTNO[i]<<endl;
				}
			int choice=0;
			while(1){
			cout<<"Enter port no for direct connection or press -1 for continue"<<endl;
			cin>>choice;
			if(choice<0)
				break;
			memset(&servaddr1, 0, sizeof(servaddr1)); 
			servaddr1.sin_family = AF_INET; // IPv4 
			servaddr1.sin_addr.s_addr = inet_addr(IP); 
			servaddr1.sin_port = htons(PORTNO[choice]);
			cout<<PORTNO[choice]<<endl;
			strcpy(buffer,">I am connected with u");
			cout<<"Requested for connection"<<endl;
			sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&servaddr1,sizeof(servaddr1));
			cout<<htons(servaddr1.sin_port)<<endl;	
			n = recvfrom(sockfd,buffer,MAXLINE,0,(struct sockaddr*)&servaddr1,&len);
			buffer[n] = '\0';
			cout<<"buffer: "<<buffer<<endl;
			cout<<"Connection Succsessfully Establised"<<endl;
			matrix[position][choice]=1;
			matrix[choice][position]=1;
			writeRoutingFile(PORTNO,matrix,clientArray);	
			}
		}
		else if(choice==2)
		{
			int blockingPort=0;
			do
			{
				cout<<"Enter Client Port No you want to Block or press -1 for exit"<<endl;
				cin>>blockingPort;
				if(blockingPort!=-1)
					blockedClients.push_back(blockingPort);
				/* code */
			} while (blockingPort!=-1);
			
		}
		else
		{
			cout<<"I am waiting for receiveing"<<endl;
			len=sizeof(cliaddr);
			n=recvfrom(sockfd,buffer,MAXLINE,0,(struct sockaddr*)&cliaddr,&len);
			buffer[n] = '\0';
			cout<<buffer<<endl;

			if(buffer[0]=='>')
			{
				strcpy(buffer,"I am also connected with you");
				sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&cliaddr,len);
				cout<<"Sended Requested back for connection"<<endl;
			}
			else if(!strcmp(buffer,"Connection"))
			{
				avalibleClients.push_back(htons(cliaddr.sin_port));
				PORTNO=readRoutingFile(matrix,clientArray);
				vector<int> temp(matrix.size());
				for(int i=0;i<temp.size();i++)
					temp[i]=0;
				matrix.push_back(temp);
				for(int i=0;i<matrix.size();i++)
					matrix[i].push_back(0);
				PORTNO.push_back(htons(cliaddr.sin_port));
				clientArray.push_back(PORTNO.size()-1);
				matrix[position][PORTNO.size()-1]=1;
				matrix[PORTNO.size()-1][position]=1;
				writeRoutingFile(PORTNO,matrix,clientArray);

			cout<<"Client connected to server: "<<htons(cliaddr.sin_port)<<endl;

			}
			else if(buffer[0]=='C')
			{
				int portno;
				string sportno="";
				string msg="",msg1="";
				int i=4;
				while(buffer[i]!=')')
					{
						sportno+=buffer[i];
						i++;
					}
				i+=3;
				while(i<strlen(buffer))
				{
					msg1+=buffer[i];
					i++;
				}
				portno=stoi(sportno);
				strcpy(buffer,"Hello from Server");
				PORTNO=readRoutingFile(matrix,clientArray);
				vector<int> path=dijkstra(matrix,PORTNO.size(),indexOfPort(PORTNO,portno),indexOfPort(PORTNO,PORT));
				
				msg="F 2 "+to_string(path.size()+1)+" "+to_string(htons(cliaddr.sin_port))+" ";
				for(int i=0;i<path.size();i++)
					msg+=to_string(PORTNO[path[i]])+" ";
				msg+="|"+msg1;
				for(int i=0;i<msg.length();i++)
					buffer[i]=msg[i];
				buffer[msg.length()]='\0';
				buffer[strlen(buffer)-1]=buffer[strlen(buffer)-1]-1;
				cliaddr.sin_port=htons(PORTNO[path[1]]);
				sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&cliaddr,len);
			}
			else if(buffer[0]=='F')
			{
				if(buffer[strlen(buffer)-1]!='0')
				{
				string msg="";
				for(int i=0;buffer[i+1]!='|';i++)
					msg+=buffer[i];
				vector<string> parsed=removeDupWord(msg);
				int msgforword=stoi(parsed[1])+1;
				int leng=stoi(parsed[2]);
				vector<int> path(leng);
				for(int i=0;i<leng;i++)
					path[i]=stoi(parsed[i+3]);
				bool check=1;
				if(path[leng-2]==PORT)
					{
					for(int i=0;i<blockedClients.size();i++)
						if(blockedClients[i]==path[0])
							{
								check=0;
								break;
							}
					}
				if(check==1)
				{
					buffer[2]=to_string(msgforword)[0];
					cliaddr.sin_port=htons(path[msgforword]);
					buffer[strlen(buffer)-1]=buffer[strlen(buffer)-1]-1;
					sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&cliaddr,len);
				}
				else
				{
					cout<<"Message Blocked"<<endl;
				}
				
				
				}
				else
				{
					cout<<"Message Lost"<<endl;
				}
			} 
		}
		
	}	
	return 0; 
} 
