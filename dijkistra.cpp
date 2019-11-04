#include<iostream>
#include <vector>
#include<stdio.h>
using namespace std;
 #define INFINITY 9999
// #define max 9
// void dijkstra(int G[max][max],int n,int destination,int source);
vector<int> dijkstra(vector<vector<int>> G,int n,int destination,int source) {
   vector<int> path;
   int max=n;
   int cost[max][max],distance[max],pred[max];
   int visited[max],count,mindistance,nextnode,i,j;
   for(i=0;i<n;i++)
      for(j=0;j<n;j++)
   if(G[i][j]==0)
      cost[i][j]=INFINITY;
   else
      cost[i][j]=G[i][j];
   for(i=0;i<n;i++) {
      distance[i]=cost[destination][i];
      pred[i]=destination;
      visited[i]=0;
   }
   distance[destination]=0;
   visited[destination]=1;
   count=1;
   while(count<n-1) {
      mindistance=INFINITY;
      for(i=0;i<n;i++)
         if(distance[i]<mindistance&&!visited[i]) {
         mindistance=distance[i];
         nextnode=i;
      }
      visited[nextnode]=1;
      for(i=0;i<n;i++)
         if(!visited[i])
      if(mindistance+cost[nextnode][i]<distance[i]) {
         distance[i]=mindistance+cost[nextnode][i];
         pred[i]=nextnode;
      }
      count++;
   }
   
   if(source!=destination) {
      path.push_back(source); 
      j=source;
      do {
         j=pred[j];
         path.push_back(j); 
      }while(j!=destination);
   }
   return path;
}
