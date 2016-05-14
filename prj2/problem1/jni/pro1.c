#include<sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

struct prinfo{
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};

int isEqual(char *ch1,char *ch2)
{
	int i=0;
	for(i=0;ch1[i]!='\0' && ch2[i]!='\0';i++)
	{
		if(ch1[i]!=ch2[i])
		  return -1;
	}
	if(ch1[i] == '\0' && ch2[i] == '\0')
	{
		return 1;
	}
	else
	  return -1;
}


int findTestID(struct prinfo* tmp,int size,char*test_name)
{
	int i;
//	char test_name[17] = "est.processtest";
	for(i=0;i<size;i++)
	{
		if(isEqual(tmp[i].comm,test_name) == 1)
		{
			return tmp[i].pid;
		}
	}
	return -1;
}




int main(int argc,char *argv[])
{
	struct prinfo*store;
	store = malloc(100*sizeof(struct prinfo));
//	printf("\nRequirement 3\n");
	int size=0;
	syscall(356,store,&size);


	struct sched_param param;
    struct sched_param param1;
    struct sched_param param2;
	int maxpri_FIFO,maxpri_NORMAL,maxpri_RR;
	maxpri_FIFO = sched_get_priority_max(SCHED_FIFO);
	maxpri_NORMAL = sched_get_priority_max(SCHED_NORMAL);
	maxpri_RR = sched_get_priority_max(SCHED_RR);
    int priority;
	int current_type;
	int pid,pid1;
	int type;
    char test_name[17] = "est.processtest";
	pid = findTestID(store,size,test_name);
    char test_name1[17] = "pro1TestARM";
    pid1 = findTestID(store,size,test_name1);
	if(pid == -1)
	{
		printf("Error! Didn't start the app!\n");
		return 0;
	}
    if(pid1 == -1)
    {
        printf("Error! Didn't start the test code!\n");
        return 0;
    }
	printf("the test pid of test app is: %d\n",pid);
//	printf("the name is :%s\n",store[pid].comm);
	for(;;)      //1,2 requirement
	{
		current_type = sched_getscheduler(pid);
		printf("Current scheduler policy: %d\n",current_type);
		printf("Please input the priority type\n");
		printf("0:Normal 1:FIFO 2:RR -1:quit\n");
		scanf("%d",&type);
		if(type ==0 )
		{
            
			param.sched_priority=maxpri_NORMAL;
			if(sched_setscheduler(pid,SCHED_NORMAL,&param) ==-1 || sched_setscheduler(pid1,SCHED_NORMAL,&param) ==-1 )
			{
				perror("sched_setscheduler() failed!\n");
			}

		}
		else if(type == 1)
		{
            printf("Please input the priority for FIFO(0 to 99): ");
            scanf("%d",&priority);
            while(priority<0 || priority > 99)
            {
                printf("Please reinput the priority for FIFO(0 to 99)");
                scanf("%d",&priority);
            }
			param.sched_priority=priority;
			if(sched_setscheduler(pid,SCHED_FIFO,&param) ==-1 || sched_setscheduler(pid1,SCHED_FIFO,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}

		}
		else if(type == 2)
		{
            printf("Please input the priority for RR(0 to 99): ");
            scanf("%d",&priority);
            while(priority<0 || priority > 99)
            {
                printf("Please reinput the priority for RR(0 to 99): ");
                scanf("%d",&priority);
            }
			param.sched_priority=priority;
			if(sched_setscheduler(pid,SCHED_RR,&param) ==-1|| sched_setscheduler(pid1,SCHED_RR,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
		}
		else
		{
			printf("finished requirement 1,2\n");
			break;
		}
		printf("New policy of app and test:%d %d\n",sched_getscheduler(pid),sched_getscheduler(pid1));
	}

//requirement 3
	int i = 0;
	printf("requirement 3 \n");
	char ch_tmp[6] = "main";
	int pid_tmp;
	int type_3;
    int pri3,pri1,pri2;
	for(;;)
	{
		printf("Please input the type for requirement 3(0:NORMAL 1:RR -1:quit): ");
		scanf("%d",&type_3);
		pid_tmp=-1;
		if(type_3 == 1)
		{
            do
            {
               printf("Please input the priority for process and test:\n");
               scanf("%d%d",&pri1,&pri2);
               pri3 = pri2;
            }while(pri1<0 || pri1 > 99 || pri2<0 || pri2 > 99 || pri3 <0 || pri3 > 99 );
		}
		else if(type_3 == 0)
		{
			pri1=maxpri_NORMAL;
            pri2 = maxpri_NORMAL;
            pri3 = maxpri_NORMAL;
		}
		else
		{
			break;
		}
        param.sched_priority = pri1;
        param1.sched_priority=pri2;
        param2.sched_priority=pri3;
		for(i=0;i<size;i++)
		{
			if(pid_tmp == -1 && isEqual(store[i].comm,ch_tmp) == 1)
			{
				pid_tmp=store[i].parent_pid;
				printf("find the main:%s\n",store[i].comm);
				continue;
			}
			if(pid_tmp != -1)
			{
				if(store[i].parent_pid != pid_tmp)
				{
					if(type_3 == 1)
					{
						if(sched_setscheduler(store[i].pid,SCHED_RR,&param) ==-1)
						{
							perror("sched_setscheduler() failed!\n");
						}
						printf("process %d has been changed to RR!\n",store[i].pid);
					}
					else
					{
						if(sched_setscheduler(store[i].pid,SCHED_NORMAL,&param) ==-1)
						{
							perror("sched_setscheduler() failed!\n");
						}
						printf("process %d has been changed to NORMAL!\n",store[i].pid);
					}
				}
				else
				{
					break;
				}
			}
		}
		if(type_3 == 1)
		{
			if(sched_setscheduler(pid,SCHED_RR,&param1) ==-1 || sched_setscheduler(pid1,SCHED_RR,&param2) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
			printf("New policy of app and test:%d %d\n",sched_getscheduler(pid),sched_getscheduler(pid1));
		}
		else
		{
			if(sched_setscheduler(pid,SCHED_NORMAL,&param1) ==-1 || sched_setscheduler(pid1,SCHED_NORMAL,&param2) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
			printf("New policy of app and test:%d %d\n",sched_getscheduler(pid),sched_getscheduler(pid1));

		}
	}	


	return 0;
}
