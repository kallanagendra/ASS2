#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <signal.h> 
#include <string.h>
#include <unistd.h>

int shmid;
int *pids;
long *a,*b;
int n;  
FILE *infptr,*oufptr;
void sigintHandler(int sig_num) 
{ 
   int i;
   // signal(SIGINT, sigintHandler); 
    for(i = 0; i < n; i++)
    {
       if(pids[i] != 0)
       {
           fprintf(stderr,"\n Killing Child : %d",pids[i]);
           kill(pids[i], SIGTERM);
       }
    }
    if(sig_num == 2)
        fprintf(stderr,"\nProgram interrupted");
    else
    fprintf(stderr,"\n Terminated : 2 sec exceeded");
    printf("\n\n Master : Milli  = %d, Sec = %d\n",*a,*b);
    shmdt(a); shmdt(b);
	fclose(infptr);
	fclose(oufptr);
    shmctl(shmid,IPC_RMID,NULL);
    abort(); 
    fflush(stdout); 
} 
  
int main (int argc,char *argv[]) 
{ 

    char arg1[10];
    char arg2[10];
    int c, i , s = 1, count = 0;
    signal(SIGALRM, sigintHandler);
    signal(SIGINT, sigintHandler);
   char *input = "input.dat", *output = "output.dat";
    alarm(2);
    while ((c = getopt (argc, argv, "hn:s:")) != -1)
    switch (c)
      {
      case 'h':
        printf("\nOPTIONS :\n");
                printf("-h for HELP \n");
        printf("-n X -s Y (X & Y are the integer) X to create no.of process Y is to no.of child allowed at sametime\n");
        return 1;
      case 'n':
        n = atoi(optarg);
    if(n > 4)
    {
      fprintf(stderr,"\nInput file has upto 4 child data");
      n = 20;
    }
        break;
      case 's':
        s = atoi(optarg);
        break;
      case '?':
        if (optopt == 'n')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
      
    infptr = fopen(input, "r");
    if(infptr == NULL)
    {
      fprintf(stderr," Input File Open ERROR \n");
      exit(1);
    }
    oufptr = fopen(output, "w");
    if(oufptr == NULL)
    {
      fprintf(stderr,"Output File Open ERROR \n");
      exit(1);
    }
    
	
	
    int inc;
    fscanf (infptr, "%d", &inc);
  
    int se,m,temp,temp2;
    
    
	fscanf (infptr, "%d", &se);fscanf (infptr, "%d", &m);fscanf (infptr, "%d", &temp);
    
    pids = malloc(sizeof(int[n]));
    //sleep(10);
    key_t key = ftok(".",65);
    int shmid = shmget(key,2*sizeof(int),0666|IPC_CREAT);
    if ( shmid == -1 )
    {
    
    fprintf(stderr,"Parent : ... Error in shmget.");
    }

    a =shmat(shmid, NULL, 0);
    b = a+1;
    *a = 0;
    *b = 0;    

    snprintf(arg1,10,"%d", shmid);
   
    
    i = 0;
   
    while ( i < s )
    {
        *a += inc;
        
        
    if(*a >= 1000000000)
    {
        *a %= 1000000000;
                *b +=1; 
    } 
    if( *b >= se)
        if(*a >= m)
        {
			i++;
            count++;
			snprintf(arg2,10,"%d", temp);
			temp2 = temp;
            fscanf (infptr, "%d", &se);fscanf (infptr, "%d", &m);fscanf (infptr, "%d", &temp); 
			
        if ( (pids[i] = fork()) == 0 )
        {
           
            
            execlp("./user","./user",arg1,arg2,(char *)NULL);
            fprintf(stderr,"%s failed to exec user!\n",argv[0]);
    
            exit( 0 );
        }
        fprintf(oufptr,"\n OSS : Forking  Pid  %d at Nano = %d, Sec = %d sent duration : %d\n",pids[i],*a,*b,temp2);
        }
		
     }
    

	while(1)
	{ 

	*a += inc;
        
        
    if(*a >= 1000000000)
    {
        *a %= 1000000000;
                *b +=1; 
    } 
		
	}
    do{
		
   fprintf(stderr,"in while");
    *a += inc;
        
        
    if(*a >= 1000000000)
    {
        *a %= 1000000000;
                *b +=1; 
    } 
    if( *b >= se)
        if(*a >= m)
        {
			 wait(NULL);
            count++;
			snprintf(arg2,10,"%d", temp);
			temp2 = temp;
           fscanf (infptr, "%d", &se);fscanf (infptr, "%d", &m);fscanf (infptr, "%d", &temp);
            
    if ( (pids[count] = fork()) == 0 )
        {
            
            execlp("./user","./user",arg1,arg2,(char *)NULL);
            fprintf(stderr,"%s failed to exec worker!\n",argv[0]);

            exit( 0 );
        }
          fprintf(oufptr,"\n OSS : Forking  Pid  %d at Nano = %d, Sec = %d sent duration : %d\n",pids[i],*a,*b,temp2);
		}
    }while(count < n);

   int j = 1;
  while (wait(NULL) > 0)
  {
 //  printf("\t%d child completed\n", j++);
  }
   
    fprintf(stderr,"\n\n Master : Nano  = %d, Sec = %d\n",*a,*b);
    shmdt(a); shmdt(b);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
} 