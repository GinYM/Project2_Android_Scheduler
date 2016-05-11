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


int findTestID(struct prinfo* tmp,int size)
{
	int i;
	char test_name[17] = "est.processtest";
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
	int maxpri_FIFO,maxpri_NORMAL,maxpri_RR;
	maxpri_FIFO = sched_get_priority_max(SCHED_FIFO);
	maxpri_NORMAL = sched_get_priority_max(SCHED_NORMAL);
	maxpri_RR = sched_get_priority_max(SCHED_RR);
	int current_type;
	if(maxpri_NORMAL == -1 || maxpri_FIFO == -1 || maxpri_RR == -1)
	{
		perror("sched_get_priority_max() failed!\n");
	}
	int pid;
	int type;
	pid = findTestID(store,size);
	if(pid == -1)
	{
		printf("Error! Didn't start the app!\n");
		return 0;
	}
	printf("the test pid of test app is: %d\n",pid);
//	printf("the name is :%s\n",store[pid].comm);
	for(;;)      //1,2 requirement
	{
	//	printf("Please input the pid(-1 to quit):");
	//	scanf("%d",&pid);
	//	if(pid == -1)
	//	{
	//		break;
	//	}

		current_type = sched_getscheduler(pid);
		printf("Current scheduler policy: %d\n",current_type);
		printf("Please input the priority type\n");
		printf("0:Normal 1:FIFO 2:RR -1:quit\n");
		scanf("%d",&type);
		if(type ==0 )
		{
			param.sched_priority=maxpri_NORMAL;
			if(sched_setscheduler(pid,SCHED_NORMAL,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}

		}
		else if(type == 1)
		{
			param.sched_priority=maxpri_FIFO;
			if(sched_setscheduler(pid,SCHED_FIFO,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}

		}
		else if(type == 2)
		{
			param.sched_priority=maxpri_RR;
			if(sched_setscheduler(pid,SCHED_RR,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
		}
		else
		{
			printf("finished requirement 1,2\n");
			break;
		}
		printf("New policy: %d\n",sched_getscheduler(pid));
	}

//requirement 3
	int i = 0;
	printf("requirement 3 \n");
	char ch_tmp[6] = "main";
	int pid_tmp;
	int type_3;
	for(;;)
	{
		printf("Please input the type for requirement 3(0:NORMAL 1:RR -1:quit): ");
		scanf("%d",&type_3);
		pid_tmp=-1;
		if(type_3 == 1)
		{
			param.sched_priority=maxpri_RR-1;
		}
		else if(type_3 == 0)
		{
			param.sched_priority=maxpri_NORMAL;
		}
		else
		{
			break;
		}
		for(i=0;i<size;i++)
		{
			//printf("here!\n");
			if(pid_tmp == -1 && isEqual(store[i].comm,ch_tmp) == 1)
			{
			//	printf("here\n");
				pid_tmp=store[i].parent_pid;
				printf("find the string:%s\n",store[i].comm);
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
			param.sched_priority=maxpri_RR;
		else
			param.sched_priority=maxpri_NORMAL;
		if(type_3 == 1)
		{
			if(sched_setscheduler(pid,SCHED_RR,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
			printf("test app is changed to RR\n");
		}
		else
		{
			if(sched_setscheduler(pid,SCHED_NORMAL,&param) ==-1)
			{
				perror("sched_setscheduler() failed!\n");
			}
			printf("test app is changed to NORMAL\n");

		}
	}	


	return 0;
}
