#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>   

int nrLinii(char a[])
{
    FILE *instructiuni=fopen(a,"r");
    char c;
    int count=0;
    for (c = getc(instructiuni); c != EOF; c = getc(instructiuni))
        if (c == '\n')
            count = count + 1;
    return count;
}
int main()
{
        char s[256];
        int clk=0;

        char readbuff[256];
        char * stop;

        int nrInstr=nrLinii("iesire.hex");
        int nrInstr1=nrLinii("test_prelucrat.asm");
        nrInstr+=1;
        u_int32_t *arr=(u_int32_t *)malloc(nrInstr*sizeof(u_int32_t));
        char **arr1=(char **)malloc(nrInstr1*sizeof(char *));

        FILE *instructiuni=fopen("iesire.hex","r");
        FILE *instructiuni1=fopen("test_prelucrat.asm","r");

        for(int i=0;i<nrInstr;++i)
        {
            fgets(readbuff,255,instructiuni);
            if(i!=nrInstr-1)
                readbuff[strlen(readbuff)-1]='\0';
            arr[i]=strtoul(readbuff,&stop,16);
        }
        for(int i=0;i<nrInstr1;++i)
        {
            arr1[i]=(char *)malloc(100 * sizeof(char));
            printf("asd\n");
            fgets(readbuff,255,instructiuni1);
            if(i!=nrInstr1-1)
                readbuff[strlen(readbuff)-1]='\0';
            printf("%s\n",readbuff);
            strcpy(arr1[i],readbuff);
        }

        while(1)
        {




            sprintf(s,"%u",arr[atoi(readbuff)]);
            //sleep(2);
            printf("\tInstructiunea %x %s\n\n",arr[atoi(readbuff)],arr1[atoi(readbuff)]);

        }
        return 0;
}