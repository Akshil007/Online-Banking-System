#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include<string.h>
#include<stdbool.h>
struct password_change
{
	char oldpassword[1024];
	char newpassword[1024];
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
	}user;
struct customer modification(struct customer data,int sd)
{
	struct modify{
		char oldusername[1024];
		char newusername[1024];
		char password[1024];
		char type;
		int update;
	};
	struct modify new;
	int c=1;
	while(c==1)
	{
	printf("\nChoose What Wants To Update\n");
	printf("1.Update UserName\n");
	printf("2.Password\n");
	printf("3.Usertype\n");
	int choice;
	scanf("%d",&choice);
	
	
	printf("\nPlease Enter following details\n");
	printf("\nEnter original username(UNIQUE):");
	scanf("%s",new.oldusername);
	switch(choice)
	{
		case 1:
			printf("\nEnter new username(UNIQUE):");
			scanf("%s",new.newusername);
			new.update=1;
			c=0;
			break;	
		case 2:
			printf("\nEnter new password:");
			scanf("%s",new.password);
			new.update=2;
			c=0;
			break;	
		case 3:
			printf("\nEnter usertype(n:normal a:admin j joint):");
			scanf(" %c",&new.type);
			new.update=3;
			c=0;
			break;	
		default:
			printf("\nInvalid choice");
			break;
	
	
	}
	}
	write(sd,&new,sizeof(new));
	struct modify res;
	read(sd,&res,sizeof(res));
	if(strcmp(res.oldusername,"ok")==0)
	{
 		printf("\n\nYour Information is modified successfully.\n\n");
	}
	else
	if(strcmp(res.oldusername,"exist")==0)
	{
		printf("\n\nUsername exist. please use diffrenet username\n\n");
	}
	else
	if(strcmp(res.oldusername,"Not found")==0)
	{
		printf("\n\nUsername not found :(\n\n");
	}
}
int main()
{
	
	
	
	struct sockaddr_in serv;

	int sd = socket (AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	
	//insert new port no here
	serv.sin_port = htons(7940);

	int connect_result=connect (sd, (void *)(&serv), sizeof(serv));
	printf("Connection result:%d\n",connect_result);
	/*write(sd,"Hello server\n",14);
	read(sd,buf,100);
	printf("%s",buf);*/
	printf("\n\n\n***************** WELCOME TO ONLINE BANK ********************\n");
	char ch='Y';
	while(ch=='Y'){
		// do you client stuff here
		
		//write(sd,"Hello server\n",14);
		
		char name[1024],pass[1024];
		printf("\nEnter useranme:");
		scanf("%s",user.username);
		printf("Enter Password:");
		scanf("%s",user.password);
		write(sd,&user,sizeof(user));
		int size;
		char buf[100];
		read(sd,buf,100);
		//printf("%s\n",buf);
		//admin....
		if(strcmp(buf,"admin")==0)//compare with msg send by server
		{
			printf("\n\n\t\tADMIN LOGIN");
			bool inside=1;
			while(inside==1)
			{
				printf("\n--------------------------------------------------");
				printf("\nChoose Option:\n");
				printf("1.Add New Account\n2.Modify Account\n3.Search an Account\n4.Delete Account\n5.Quit\n");
				int choice;
				scanf("%d",&choice);
				switch(choice)
				{
					//add account
					case 1:
						write(sd,"add",4);	
						while(1)
						{
							struct customer new,newj;
							printf("\nPlease Enter following details\n");
							printf("\nEnter username(UNIQUE):");
							scanf("%s",new.username);
							printf("\nEnter password:");
							scanf("%s",new.password);
							printf("\nEnter Customer type (a:admin n:normal j:joint):");
							scanf(" %c",&new.type);
							if(new.type=='j')//not completed
							{
								printf("\nEnter second username(UNIQUE) for joint account:");
								scanf("%s",newj.username);
								printf("\nEnter password:");
								scanf("%s",newj.password);
								newj.type='j';
								write(sd,&new,sizeof(new));
								write(sd,&newj,sizeof(newj));
							}
							else
							{
								write(sd,&new,sizeof(new));
							}
							char res[100];
							read(sd,&res,100);
							if(strcmp(res,"ok")==0)
							{
								struct acc_no a;
								int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
								read(fd2,&a,sizeof(a));
								printf("\n\nCongratulations...\t:)\nAccount created successfully ... Your account number is %lld\n\n",a.account_number);
								break;
							}
							else
							if(strcmp(res,"exist")==0)
							{
								printf("\n\nUsername exist. please use diffrenet username\n");
								continue;
							}
						}
						break;
						
					//modification
					case 2:
						write(sd,"mod",4);
						struct customer mod;
						mod=modification(mod,sd);
						break;
						
						
					//search
					//problem is when you search for name which is not in database
					case 3:
						write(sd,"srh",4);
						struct customer ser;
						printf("Please Enter following details\n");
						printf("\nEnter username to be searched:");
						scanf("%s",ser.username);
						write(sd,&ser,sizeof(ser));
						int ssize;
						while((ssize=read(sd,&ser,sizeof(ser)))==0);
						printf("%s",ser.username);
						if(strcmp(ser.username,"Not found")==0)//username ==not found
						{
							printf("\n\nUsername does not Exist :(\n\n");
						}
						else
						{
							printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
							printf("\nUser information:\n");
							printf("\tUsername:%s\n",ser.username);
							//printf("%s\n",ser.password);
							printf("\tAccount no:%lld\n",ser.account_number);
							printf("\tBalance:%f\n",ser.balance);
							printf("\tUser type(j:joint,n:normal,a:admin) %c",ser.type);
							printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
						}
						break;
					//delete
					case 4:
						write(sd,"del",4);
						struct customer del;
						printf("\nPlease Enter following details\n");
						printf("\nEnter username to be deleted:");
						scanf("%s",del.username);
						write(sd,&del,sizeof(del));
						int dsize;
						while((dsize=read(sd,&del,sizeof(del)))==0);
						if(strcmp(del.username,"Not found")==0)//username ==not found
						{
							printf("\n\nUsername does not Exist :(\n\n");
						}
						else
						{
							printf("\n\nAccount deleted successfully :)\n\n");
						}
						
						break;
					//exit
					case 5:	
							write(sd,"out",4);
							inside=0;
							break;
					default:
						printf("\n\nInvalid choice\n\n");
						
						
					
						
				}
			}
		}
		else if(strcmp(buf,"normal")==0||strcmp(buf,"joint")==0)
		{
			if(strcmp(buf,"joint")==0)
			{
				printf("\n\n\t\tJOINT USER");
			}
			else
			{
				printf("\n\n\t\tNORMAL USER");
			}
			
			int inside=1;
			while(inside==1)
			{
				printf("\n--------------------------------------------------");
				printf("\nEnter Choice\n");
				printf("1.Deposit\n");
				printf("2.Withdraw\n");
				printf("3.Balance Enquiry\n");
				printf("4.Password Change\n");
				printf("5.View Passbook\n");
				printf("6.Exit\n");
				int choice;
				scanf("%d",&choice);
				switch(choice)
				{
					case 1:
						write(sd,"dep",4);
						struct customer depo,res;
						//struct responce res;
						printf("Enter Deposit Amount:\n");
						scanf("%lf",&depo.balance);
						printf("%lf",depo.balance);
						write(sd,&depo,sizeof(depo));
						read(sd,&res,sizeof(res));
						if(strcmp(res.username,"unsuccess")==0)
						{
							printf("\n\nTransaction Unsuccessful :(\n\n");
						}
						else if(strcmp(res.username,"success")==0)
						{
							printf("\n\nTransaction successfull :)\nYour current balance is %f\n\n",res.balance);
						}
						break;
						
					case 2:
						write(sd,"wit",4);
						struct customer wit,res1;
						//struct responce res;
						printf("Enter Withdraw Amount:\n");
						scanf("%lf",&wit.balance);
						write(sd,&wit,sizeof(wit));
						read(sd,&res1,sizeof(res1));
						if(strcmp(res1.username,"notenough")==0)
						{
							printf("\n\nNot enough money in your account  :(\n\n");
						}
						else if(strcmp(res1.username,"success")==0)
						{
							printf("\n\nTransaction successfull :)\nYour current balance is %f\n\n",res1.balance);
						}
						break;
					case 3:
						write(sd,"bal",4);
						struct customer res2;
						read(sd,&res2,sizeof(res2));
						if(strcmp(res2.username,"unsuccess")==0)
						{
							printf("\n\nRequest Unsuccessful :(\n\n");
						}
						else if(strcmp(res2.username,"success")==0)
						{
							printf("\n\nYour current balance is %f\n\n",res2.balance);
						}
						break;
					case 4:
						write(sd,"pas",4);
						struct password_change ps;
						struct customer res3;
						printf("\nPlease enter old password:");
						scanf("%s",ps.oldpassword);
						printf("\nPlease enter new password:");
						scanf("%s",ps.newpassword);
						write(sd,&ps,sizeof(ps));
						read(sd,&res3,sizeof(res3));
						if(strcmp(res3.username,"notmatch")==0)
						{
							printf("\n\nYour old password does not match :(\n\n");
						}
						else if(strcmp(res3.username,"success")==0)
						{
							printf("\n\nYour password is changed successfully :)\n\n");
						}
						break;
					case 5:
						write(sd,"vie",4);
						char buf[10000];
						int len=read(sd,buf,10000);
						if(strcmp(buf,"empty")==0)
						{
							printf("\nYour passbook is empty :(\n\n");
						}
						else
						{
							printf("\n\t\tPassbook");
							printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
							printf("\n%s",buf);
							printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
						}
						break;
					case 6:
						write(sd,"out",4);
						inside=0;
						break;
					default:
						printf("\n\nInvalid choice\n\n");
						break;
				}
			
			
			}
			
		}
		else if(strcmp(buf,"Invalid")==0)
		{
			printf("\n\nUsername or Password is Invalid. Please try again :)\n\n");
			continue;
		}
		else if(strcmp(buf,"logged")==0)
		{
			printf("\n\nUser is already logged in. Please try later :)\n\n");
			continue;
		}
		else
		{
			printf("\ndefault\n");
		}
	

	printf("\n\nYou are logged out\n\n");
	printf("Do you want to continue(Y/N)?");
	scanf(" %c",&ch);
	write()
	}



}

