#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
	struct acc_no
	{
		long long int account_number;
	}a;
	struct customer{
		char username[1024];
		char password[1024];
		long long int account_number;
		double balance;
		char type;
	}add;
	printf("\nEnter username:");
	scanf("%s",add.username);
	printf("\nEnter password:");
	scanf("%s",add.password);
	printf("\nEnter Customer type (a:admin n:normal j:join):");
	scanf(" %c",&add.type);
	add.balance=0.0;
	
	//account no
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	add.account_number=++a.account_number;
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	
	
	int fd=open("customer",O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,&add,sizeof(add));
	printf("Entry has been added into database.\n");
	

	return 0;
	
}
