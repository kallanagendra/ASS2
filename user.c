#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc,char *argv[])
{
   //while(1); // To test 2 sec termination
   
    int shmid = atoi(argv[1]);
    int n = atoi(argv[2]);
    int i;
 
    long *a, *b;
    long la,lb;
    
    a = shmat(shmid, NULL, 0);
    b = a+1;
    
    la = *a;
    lb = *b;
    
    la = la + n;
    
	
    if(la > 1000000000)
    {
        la %= 1000000000;
        lb +=1; 
    } 
 

    while(1)
    {
        if(*b >= lb)
        {
            if(*a >= la)
                break;
        }
    }
    
    fprintf(stderr,"\n Terminating Pid :%d User :  Nano = %d, Sec = %d\n",getpid(),*a,*b);
    shmdt(a); shmdt(b);
}
