#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int gfd;
struct flock lck,lck2;
struct password_change
{
	char oldpassword[1024];
	char newpassword[1024];
};
struct passbook
{
	char data[5000];
	

};
struct acc_no
	{
		long long int account_number;
	};
struct customer{
		char username[1024];
		char password[1024];
		long long int account_number;
		double balance;
		char type;
	}login_time_cp;
struct modify{
		char oldusername[1024];
		char newusername[1024];
		char password[1024];
		char type;
		int update;
	};
void lockjoint(struct customer joint)
{
	int size;
	lseek(gfd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(gfd,&r,sizeof(r)))!=0)
	{
		if(joint.account_number==r.account_number&&strcmp(joint.username,r.username)!=0)
		{
			write(0,"joint2\n",6);
			lseek(gfd,-1*sizeof(r),SEEK_CUR);
					
					
					lck2.l_type=F_WRLCK;
					lck2.l_whence=SEEK_CUR;
					lck2.l_start=0;
					lck2.l_len=sizeof(r);
					lck2.l_pid=getpid();
					fcntl(gfd,F_SETLK,&lck2);
					break;
		}
	}	

}
char login(struct customer request,int login)
{
	
	gfd=open("customer",O_RDWR,0744);
	if(login==1)
	{
		int size;
		lseek(gfd,0,SEEK_SET);
		struct customer r;
		char ans='I';
		int i;
		while((size=read(gfd,&r,sizeof(r)))!=0)
		{
			if(strcmp(request.username,r.username)==0)
			{
				if(strcmp(request.password,r.password)==0)
				{
					
					login_time_cp=r;//incase of normal user we need username so stored it in (login time copy)
					//record locking
					lseek(gfd,-1*sizeof(r),SEEK_CUR);
					
					
					lck.l_type=F_WRLCK;
					lck.l_whence=SEEK_CUR;
					lck.l_start=0;
					lck.l_len=sizeof(r);
					lck.l_pid=getpid();
					//before critical section
					write(0,"before\n",8);
					i=fcntl(gfd,F_SETLK,&lck);
					if(i==-1)
					{
						write(0,"locked\n",8);
						ans='L';
						perror("error:");
						break;
					}
					if(r.type=='j')
					{
						write(0,"rtype\n",6);
						lockjoint(r);	
					}
					
					//inside critical section
					write(0,"Inside\n",8);
					//getchar();
					ans=r.type;
					break;
				}
			}
		}
		//close(fd);
		return ans;
	}
	else
	{
		fcntl(gfd,F_UNLCK,&lck);
		close(gfd);
		return 's';
	}
}
struct customer delete(struct customer del)
{
	int fd=open("customer",O_RDWR,0744);
	int fd2=open("copy",O_CREAT|O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	int delt=0;
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(del.username,r.username)==0)//if match found then skipping
		{
			delt=1;
			continue;
		}
		write(fd2,&r,sizeof(r));//writing in copy file
	}
	close(fd);
	close(fd2);
	remove("customer");//removing old file
	rename("copy","customer");//renaming copied file to customer which does not contains deleted record
	if(delt==0)//if username not fond
	{
		strcpy(del.username,"Not found");
	}
	else
	{
		strcpy(del.username,"found");
	}
	
	
	return del;
}
struct customer search(struct customer ser)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(ser.username,r.username)==0)
		{
			return r;	//already exist in database
		}
	}
	strcpy(ser.username,"Not found");
	return ser;
}
char add(struct customer add_req)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(add_req.username,r.username)==0)
		{
			res='E';	//already exist in database
		}
	}
	if(res=='E')
	{
		return res;
	}
	
	//get an account no
	struct acc_no a;
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	add_req.account_number=++a.account_number;
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	lseek(fd,0,SEEK_END);
	write(fd,&add_req,sizeof(add_req));
	close(fd);
	close(fd2);
	login(add_req,1);
	return 'S';//success
	

}	
void addafterop(struct customer add_req)
{
	int fd=open("customer",O_RDWR,0744);	
	lseek(fd,0,SEEK_END);
	write(fd,&add_req,sizeof(add_req));
	//login(add_req,1);
	//login(add_req,1);
	//write(0,login_time_cp.username,sizeof(login_time_cp.username));
	//write(0,add_req.username,sizeof(login_time_cp.username));
	close(fd);
}


struct modify modification(struct modify m)
{
	int fd=open("customer",O_RDWR,0744);
	int size,size1;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	int found=0;
	struct customer updated;
	struct customer delet;
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(m.oldusername,r.username)==0)
		{
			found=1;
			updated=r;
			delet=r;
			switch(m.update)
			{
				case 1:
					
					lseek(fd,0,SEEK_SET);
					struct customer r1;
					while((size1=read(fd,&r1,sizeof(r1)))!=0)
					{
						if(strcmp(m.newusername,r1.username)==0)
						{
							strcpy(m.oldusername,"exist");
							return m;
						}
					}
					strcpy(updated.username,m.newusername);
					break;
				case 2:
					strcpy(updated.password,m.password);
					break;
				case 3:
					updated.type=m.type;
					break;
					
			}
			delete(delet);
			addafterop(updated);
			strcpy(m.oldusername,"ok");
			return m;
			
		}
	}
	if(found==0)
	{
		strcpy(m.oldusername,"Not found");
	}
	return m;
	
}
void writejoint2(struct customer joint)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(joint.account_number==r.account_number&&strcmp(joint.username,r.username)!=0)
		{
			r.balance=joint.balance;//for responce
			delete(r);
			addafterop(r);
			break;
		}
	}

}
void updatepassbook(struct customer data)
{
	char buf[100],path[100];
	sprintf(path,"passbook/%lld",data.account_number);
	time_t t;
	time(&t);
	int len=sprintf(buf,"%s Balance=%f\n",ctime(&t),data.balance);
	int fd=open(path,O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,buf,len);
	close(fd);
}

struct customer bank(struct customer data,int op,struct password_change pass)
{	double nbalance;
	switch(op)
	{
		case 1:
			nbalance=login_time_cp.balance+data.balance;//deposit
			login_time_cp.balance=nbalance;//to store in database
			data.balance=nbalance;//for responce
			delete(login_time_cp);
			addafterop(login_time_cp);
			if(login_time_cp.type=='j')
			{
				writejoint2(login_time_cp);
			}
			updatepassbook(login_time_cp);
			//login(login_time_cp,1);
			login(login_time_cp,1);
			strcpy(data.username,"success");
			break;
		case 2:
			if(login_time_cp.balance>=data.balance)
			{
				nbalance = login_time_cp.balance - data.balance;//withdraw
				login_time_cp.balance = nbalance;//to store in database
				data.balance = nbalance;//for responce
				delete(login_time_cp);
				addafterop(login_time_cp);
				if(login_time_cp.type=='j')
				{
					writejoint2(login_time_cp);
				}
				updatepassbook(login_time_cp);
				login(login_time_cp,1);
				strcpy(data.username,"success");
			}
			else{
				strcpy(data.username,"notenough");
			}
			break;
		case 3:
			data.balance=login_time_cp.balance;//balance query;
			strcpy(data.username,"success");
			break;
		case 4:
			if(strcmp(login_time_cp.password,pass.oldpassword)==0)
			{
				strcpy(login_time_cp.password,pass.newpassword);
				delete(login_time_cp);
				addafterop(login_time_cp);
				login(login_time_cp,1);
				strcpy(data.username,"success");
					
			}
			else
			{
				strcpy(data.username,"notmatch");
			}
			break;
			
		
	}
	return data;

}
char jointadd(struct customer new,struct customer newj)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	int i=2;
	struct customer test=new;
	while(i-->0)
	{
		lseek(fd,0,SEEK_SET);
		struct customer r;
		char res='N';
		while((size=read(fd,&r,sizeof(r)))!=0)
		{
			if(strcmp(test.username,r.username)==0)
			{
				res='E';
				return res;	//already exist in database
			}
		}
		test=newj;
	}
	
	//get an account no
	struct acc_no a;
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	new.account_number=++a.account_number;
	newj.account_number=new.account_number;//same acc_no
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	lseek(fd,0,SEEK_END);
	write(fd,&new,sizeof(new));
	write(fd,&newj,sizeof(newj));
	close(fd);
	close(fd2);
	return 'S';//success

}

int main()
{
	
	struct sockaddr_in serv, cli;
	mkdir("passbook",0777);
	int sd,nsd,size_client; 
	sd= socket (AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	
	//Insert new port no here
	serv.sin_port = htons(7940);
	
	int bind_result=bind(sd, (void *)(&serv), sizeof(serv));
	int listen_result=listen (sd, 5);
	printf("Server Initializing...\nSocket returns :: %d\nBind result:: %d\nListening status %d\n",sd,bind_result,listen_result);
	while(1) {
		int sz=sizeof(cli);
	nsd =accept (sd, (void *)&cli,&sz);
	printf("%d",nsd);
	if (!fork( )) {
		   while(1)
		   {
			   close(sd);
			   write(0,"start\n",7);//to check where control goes
			   struct customer request;
			   char c;
			   int size=read(nsd,&request,sizeof(request));
			 //  write(0,request.username,sizeof(request.username));
			   c=login(request,1);//login classified as a for admin,n for normal,j for joint
			   login(request,1);		//L for alreday logged in
			   if(c=='a')
			   {
			   	
			   		write(0,"adminstart\n",12);//to check where control goes
			   		write(nsd,"admin",6);
			   		struct customer new;
			   		int in=0,newsize;
			   		while(in==0)
			   		{
				   		char cmd[10];
				   		read(nsd,&cmd,10);
				   		if(strcmp(cmd,"add")==0)
				   		{
					   		int j=1;
					   		char c;
					   		struct customer newj;
					   		while(1)
					   		{
					   		while((newsize=read(nsd,&new,sizeof(new)))==0);
					   		if(new.type=='j')//for joint
					   		{
					   			read(nsd,&newj,sizeof(newj));
					   			c=jointadd(new,newj);
					   		}
					   		else
					   		{
					   			c=add(new);
					   		}
					   	
						   		switch(c)
						   		{
						   			case 'E':
						   				write(nsd,"exist",6);
						   				break;
						   			case 'S':
						   				write(nsd,"ok",3);
										break;
						   				break;
						   			default:
						   				write(nsd,"default",7);
						   				break;
						   				break;
						   		}
						   	}
					   	}
					   	else if(strcmp(cmd,"mod")==0)
					   	{
					   		struct modify m;
					   		read(nsd,&m,sizeof(m));
					   		m=modification(m);
					   		write(nsd,&m,sizeof(m));
					   	
					   	}
					   	else if(strcmp(cmd,"srh")==0)
					   	{
					   		struct customer ser;
					   		read(nsd,&ser,sizeof(ser));
					   		ser=search(ser);
					   		//write(0,ser.username,sizeof(ser.username));
					   		write(nsd,&ser,sizeof(ser));
					   	
					   	}
					   	else if(strcmp(cmd,"del")==0)
					   	{
					   		struct customer del;
					   		read(nsd,&del,sizeof(del));
					   		del=delete(del);
					   		//write(0,del.username,sizeof(ser.username));
					   		write(nsd,&del,sizeof(del));
					   	}
					   	else if(strcmp(cmd,"out")==0)
					   	{
					   		in=1;
					   	}
					   	
			   		}
			   	}
			   	else if(c=='n'||c=='j')
			   	{
			   		write(0,"normalstart\n",14);//to check where control goes
			   		if(c=='j')
			   		{
			   			write(nsd,"joint",6);
			   		}
			   		else
			   		{
			   			write(nsd,"normal",6);
			   		}
			   		char cmd[10];
			   		int in_n=1;
			   		
			   		while(in_n==1)
			   		{
			   			read(nsd,&cmd,10);
			   			struct password_change dummy;
			   			if(strcmp(cmd,"dep")==0)//deposit
			   			{
			   				//remaining from here
			   				struct customer dep;
			   				read(nsd,&dep,sizeof(dep));
			   				dep=bank(dep,1,dummy);
			   				write(nsd,&dep,sizeof(dep));
			   			}
			   			else if(strcmp(cmd,"wit")==0)//withdraw
			   			{
			   				struct customer wit;
			   				read(nsd,&wit,sizeof(wit));
			   				wit=bank(wit,2,dummy);
			   				write(nsd,&wit,sizeof(wit));
			   			}
			   			else if(strcmp(cmd,"bal")==0)
			   			{
			   				struct customer bal;
			   				bal=bank(bal,3,dummy);
			   				write(nsd,&bal,sizeof(bal));
			   			}
			   			else if(strcmp(cmd,"pas")==0)
			   			{
			   				struct password_change pass;
			   				struct customer res;
			   				read(nsd,&pass,sizeof(pass));
			   				res=bank(res,4,pass);
			   				write(nsd,&res,sizeof(res));
			   				
			   			}
			   			else if(strcmp(cmd,"vie")==0)
			   			{
			   				
							char path[100],buf[10000];
							sprintf(path,"passbook/%lld",login_time_cp.account_number);
							int fd=open(path,O_CREAT|O_RDONLY,0744);
							int len=read(fd,buf,10000);
							if(len==-1||len==0)
							{
								write(nsd,"empty",6);	
							}
							else
							{
								write(nsd,buf,len);
							}
							close(fd);
							
			   				
			   			}
			   			else if(strcmp(cmd,"out")==0)
			   			{
			   				in_n=0;
			   			}
			   			
			   		}
			   		write(0,"normalend\n",10);//to check where control goes
			   	}
			   	else if(c=='I')
			   	{
			   		write(nsd,"Invalid",7);
			   	}	
			   	else if(c=='L')
			   	{
			   	
			   		write(nsd,"logged",7);
			   	}
			   	else
			   	{
			   		write(nsd,"default\n",6);
			   	}
			   	struct customer dumy;
			   	login(dumy,0);
			   }
			   
			   
			
		   
		   
		   
	}
	else{
	 	close(nsd);
	 	}
	   
	}
	
	
	



}
