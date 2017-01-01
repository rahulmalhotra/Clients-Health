#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<pthread.h>
int ping_all=0,ping_one=0;
int xe=0,xe1=0;
int loop=0,total_entry=0,loop1=0,active=0;
char name[50],name1[50];
void *connection_handler(void *);
int main()
{
pid_t pid;
int ssfd,csfd,*new_sock;
int s_len,c_len,bindfd,m;
struct sockaddr_in s_address;
struct sockaddr_in c_address;
ssfd=socket(AF_INET,SOCK_STREAM,0);
printf("\n Server Running...");
if(ssfd==-1)
printf("\n Sorry socket is not created !!! \n");
/*else
printf("\n Congo, Socket is created \n");
*/
s_address.sin_family=AF_INET;
s_len=sizeof(s_address);
s_address.sin_port=htons(7000);
s_address.sin_addr.s_addr=htonl(INADDR_ANY);
bindfd=bind(ssfd,(struct sockaddr *)&s_address,s_len);
if(bindfd==-1)
printf("\n Sorry bind is not performed \n");
//else
//printf("\n Congo. bind is performed successfully \n");
listen(ssfd,3);
c_len=sizeof(c_address);
/*csfd=4;
while(csfd==4)
{*/
while(csfd=accept(ssfd,(struct sockaddr *)&c_address,&c_len))
{
/*char *myipaddr;
myipaddr[0]=0;
sprintf(myipaddr,"%s", inet_ntoa(((struct sockaddr_in *)&c_address)->sin_addr));
*/
pthread_t sniffer_thread;
new_sock = malloc(1);
*new_sock = csfd;
if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
{
perror("could not create thread");
return 1;
}
}
return 0;
}
void *connection_handler(void *socket_desc)
{
//Get the socket descriptor
int csfd = *(int*)socket_desc;
//close(ssfd);
char ch2[50];
char ch3[50];
int i=1,x=0,y=0,done=0;
while(1)
{
if(xe==1)
{
done=0;
}
l:
if(ping_one==1)
{
FILE *fp;
char chr[50];
fp=fopen("clients.log","r");
if(loop1!=total_entry)
{
while(fgets(chr,50,fp)!=NULL)
{
if(strcmp(chr,ch3)==0)
{
if(strcmp(name1,ch2)==0)
{
if(active!=1)
printf("%s is active\n",name1);
active=1;
}
loop1++;
xe1=0;
}
}
}
else
{
printf("\n\n Manual checking ended...\n");
if(active!=1)
printf("\n\n client not found... \n\n");
ping_one=0;
active=0;
loop1=0;
xe1=1;
}
}
/*else
{
printf("\n\n Automatic checking ended...\n");
ping_all=0;
loop=0;
xe1=1;
}
FILE *fp;
char ch4[50];
char chr[50];
fp=fopen("clients.log","r");
if(loop1!=total_entry)
{
while(fgets(chr,50,fp)!=NULL)
{
if(strcmp(ch3,name)==0)
{
loop1++;
printf("\n\n Client %s is active \n",name1);
printf("\n\n");
ping_one=0;
loop1=0;
active=1;
}
//}
}*/
if(ping_all==1)
{
FILE *fp;
char ch4[50];
char chr[50];
fp=fopen("clients.log","r");
if(loop!=total_entry)
{
while(fgets(chr,50,fp)!=NULL)
{
if(strcmp(chr,ch3)==0)
{
FILE *fp;
fp=fopen("active clients.log","a");
strcpy(ch4,ch2);
strcat(ch4," is active\n");
if(done==0)
{
loop++;
fputs(ch4,fp);
done=1;
xe=0;
}
fclose(fp);
}
}
}
else
{
printf("\n\n Automatic checking ended...\n");
ping_all=0;
loop=0;
xe=1;
}
}
if(x==1)
{
x=0;
goto m;
}
if(y==1)
{
y=0;
goto n;
}
char ch[50];
char ch1[50];
//printf("\n Waiting for client to send message.... \n");
strcpy(ch,"x");
while(strcmp(ch,"\0")!=0)
{
if((ping_all==1)||(ping_one==1))
{
x=1;
goto l;
}
m:
read(csfd,&ch,50);
if(i==1)
{
printf("\n\n New client %s connected \n\n",ch);
total_entry++;
FILE *fp;
fp=fopen("clients.log","a");
strcpy(ch2,ch);
strcat(ch,"\n");
strcpy(ch3,ch);
fputs(ch,fp);
fclose(fp);
i=0;
}
else
{
if(strcmp(ch,"end_chat")==0)
{
close(csfd);
break;
}
if(strcmp(ch,"send_file")==0)
{
char ch3='l';
FILE *fp;
char ch2[50],ch5[30];
printf("\n Enter the location where you want to save your file - ");
gets(ch2);
printf("\n Enter the file name - ");
gets(ch5);
strcat(ch2,"/");
strcat(ch2,ch5);
fp=fopen(ch2,"w");
while(ch3!='\0')
{
read(csfd,&ch3,1);
if(ch3!='\0')
fputc(ch3,fp);
}
fclose(fp);
}
else if(strcmp(ch,"ping_all")==0)
{
printf("\n\n Automatic checking started....\n");
ping_all=1;
}
else if(strcmp(ch,"ping_one")==0)
{
printf("\n\n Enter the name of the client - ");
gets(name);
printf("\n");
strcpy(name1,name);
strcat(name,"\n");
ping_one=1;
}
else
{
if(strcmp(ch,"\0")!=0)
{
printf(" [ %s ] -> ",ch2);
printf("%s\n\n",ch);
}
}
}
}
if(strcmp(ch,"end_chat")==0)
{
close(csfd);
break;
}
/*if(ping_all==1)
{
y=1;
goto l;
}*/
strcpy(ch1,"x");
while(strcmp(ch1,"\0")!=0)
{
if((ping_all==1)||(ping_one==1))
{
y=1;
goto l;
}
n:
printf("\n Message for %s --> ",ch2);
gets(ch1);
if(strcmp(ch1,"\0")==0)
printf("\n");
if(strcmp(ch1,"end_chat")==0)
{
write(csfd,&ch1,50);
close(csfd);
break;
}
if(strcmp(ch1,"send_file")==0)
{
write(csfd,&ch1,50);
FILE *fs;
char ch3;
char ch2[50];
printf("\n Enter the file path -");
gets(ch2);
fs=fopen(ch2,"r");
while((ch3=fgetc(fs))!=EOF)
{
write(csfd,&ch3,1);
}
ch3='\0';
write(csfd,&ch3,1);
fclose(fs);
printf("\n\n File sent successfully.... \n\n Enter the message - \n\n");
}
else
write(csfd,&ch1,50);
}
if(strcmp(ch1,"end_chat")==0)
{
close(csfd);
break;
}
}
}

/*close(csfd);
}
}*/
