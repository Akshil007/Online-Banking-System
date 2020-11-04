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
	struct customer{
		char username[1024];
		char password[1024];
		long long int account_number;
		double balance;
		char type;
	}r;
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		printf("%s\n",r.username);
		printf("%s\n",r.password);
		printf("%lld\n",r.account_number);
		printf("%f\n",r.balance);
		printf(" %c\n",r.type);
	}

	return 0;
	
}
