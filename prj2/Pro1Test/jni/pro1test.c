#include<stdio.h>
#include<sys/time.h>

int main()
{
    int exe_times;
    struct timeval start,end;
    for(;;)
    {
        printf("Please input the times you want to execute(-1 to quit): ");
        scanf("%d",&exe_times);
        if(exe_times == -1)
          break;
        int i;
        int calculate = -1;
        gettimeofday(&start,NULL);
        for(i=0;i<=exe_times;i++)
        {
            calculate++;
            printf("result: %d\n",calculate);
        }
        gettimeofday(&end,NULL);
        int time_interval = 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
        printf("execution time:%d\n",time_interval);
    }
    return 0;
}
