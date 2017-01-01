#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
int main()
{
int result,len,sfd;
struct sockaddr_in address;
sfd=socket(AF_INET,SOCK_STREAM,0);
if(sfd==-1)
printf("\n Sorry socket is not created \n");
/*else
printf("\n Socket is created \n");
*/
address.sin_family=AF_INET;
address.sin_port=htons(7000);
address.sin_addr.s_addr=inet_addr("127.0.0.1");
len=sizeof(address);
result=connect(sfd,(struct sockaddr *)&address,len);
int i=1;
while(1)
{
char ch[50];
char ch1[50];
if(i==1)
{
printf("\n Login Portal");
i=0;
printf("\n\n Enter your name - ");
gets(ch1);
write(sfd,&ch1,50);
printf("\n Connected to server....\n\n");
strcpy(ch1,"\0");
write(sfd,&ch1,50);
}
else
{
//printf("\n Enter your message - ");
strcpy(ch,"x");
while(strcmp(ch,"\0")!=0)
//while(1)
{
printf("\n Me --> ");
gets(ch);
if(strcmp(ch,"\0")==0)
printf("\n");
if(strcmp(ch,"end_chat")==0)
{
printf("\n\n Chat Terminated by you...\n\n");
write(sfd,&ch,50);
close(sfd);
exit(0);
}
if(strcmp(ch,"send_file")==0)
{
write(sfd,&ch,50);
FILE *fs;
char ch3;
char ch2[50];
printf("\n Enter the file path -");
gets(ch2);
fs=fopen(ch2,"r");
while((ch3=fgetc(fs))!=EOF)
{
write(sfd,&ch3,1);
}
ch3='\0';
write(sfd,&ch3,1);
fclose(fs);
printf("\n\n File sent successfully.... \n\n Enter the message - \n\n");
}
else
write(sfd,&ch,50);
}
}
strcpy(ch1,"x");
while(strcmp(ch1,"\0")!=0)
{
read(sfd,&ch1,50);
if(strcmp(ch1,"end_chat")==0)
{
printf("\n\n Chat Terminated by server\n\n");
close(sfd);
exit(0);
}
if(strcmp(ch1,"send_file")==0)
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
read(sfd,&ch3,1);
if(ch3!='\0')
fputc(ch3,fp);
}
fclose(fp);
}
else
{
if(strcmp(ch1,"\0")!=0)
{
printf(" Server --> ");
printf("%s\n\n",ch1);
}
}
}
}
close(sfd);
}
