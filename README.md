# Online-Banking-System
## Main objective of this project is to implement it using System calls instead of library functions.
## Other Objectives
<b>a.</b> All account transactional details and customer information are stored in file Customer.<br>
<b>b.</b> Account holders have to pass through a login system to enter their accounts.<br>
<b>c.</b>  The application should possess password-protected administrative access; thus
preventing the whole management system from unauthorized access.<br>
<b>d.</b> Three types of login should be created: normal user, joint account user; administrator;
<b>e.</b>  Once you login as administrator, you should be able to add, delete, modify, search for a
specific account details.<br>
<b>f.</b>  Once the customer connect to the server, you should get login and password prompt.<br>
After successful login, you should get menu for example:<br>
Do you want to:<br>
 Deposit<br>
 Withdraw<br>
 Balance Enquiry<br>
 Password Change<br>
 Exit<br>
<b>g.</b>  If you login as a joint account holder, proper file locking should be implemented. If you
want to view the account details then read lock is to be set else if you want to withdraw
or deposit then write lock should be used to protect the critical data section.<br>
<b>h.</b> Use socket programming – Server maintains the data base and service multiple clients
concurrently. Client program can connect to the server and access their specific account
details.<br>
<b>i.</b> Use system calls instead of Library functions wherever it is possible in the project:
Process Management, File Management, File Locking, Multithreading and Inter Process
Communication Mechanisms.<br>

## Run through guide
1) First compile and run add.c file to create admin.
2) Insert same  port no in client.c(line no 110) and server.c(line no 433) 
3) Now run server.c 
4) open new terminal and run client.c</br>
Enjoy!!
