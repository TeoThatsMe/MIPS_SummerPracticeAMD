#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>   

int nrLinii()
{
    FILE *instructiuni=fopen("iesire.hex","r");
    char c;
    int count=0;
    for (c = getc(instructiuni); c != EOF; c = getc(instructiuni))
        if (c == '\n')
            count = count + 1;
    return count;
}


int main()
{
    int p_PC_IM[2],p_IM_DEC[2],p_DEC_EX[2];

    if(pipe(p_PC_IM)<0)
        return EXIT_FAILURE;

    if(pipe(p_IM_DEC)<0)
        return EXIT_FAILURE;
    
    if(pipe(p_DEC_EX)<0)
        return EXIT_FAILURE;
    


    if(fork()==0)
    {
        char s[256];
        int count=0;
        close(p_PC_IM[0]);
        while(1)
        {


            sprintf(s,"%d",count);
            //printf("@@@%s\n",s);

            write(p_PC_IM[1],s,sizeof(s));
            sleep(2);
            count++;
            if(count==5)
            {
                //printf("COunt==5 out");
                break;
            }
        }

        return 0;
    }



    if(fork()==0)
    {
        char s[256];

        close(p_PC_IM[1]);
        close(p_IM_DEC[0]);

        char readbuff[256];
        char * stop;

        int nrInstr=nrLinii();
        nrInstr+=1;
        u_int32_t *arr=(u_int32_t *)malloc(nrInstr*sizeof(u_int32_t));

        FILE *instructiuni=fopen("iesire.hex","r");

        for(int i=0;i<nrInstr;++i)
        {
            fgets(readbuff,255,instructiuni);
            if(i!=nrInstr-1)
                readbuff[strlen(readbuff)-1]='\0';
            arr[i]=strtoul(readbuff,&stop,16);
        }

        while(1)
        {
            read(p_PC_IM[0],readbuff,sizeof(readbuff));


            //printf("###%d\n",atoi(readbuff));
            sprintf(s,"%u",arr[atoi(readbuff)]);
            sleep(2);
            write(p_IM_DEC[1],s,sizeof(s));



            if(atoi(readbuff)==4)
                break;
            strcpy(readbuff,"");
        }
        return 0;
    }



    if(fork()==0)
    {
        char readbuff[256];

        close(p_IM_DEC[1]);
        close(p_DEC_EX[0]);
        while (1)
        {

            read(p_IM_DEC[0],readbuff,sizeof(readbuff));
            //printf("!!!%x\n",atoi(readbuff));
            //sleep(3);
            unsigned int instr=atoi(readbuff);
            unsigned long int semnale=0; 
            unsigned int opcode=(instr&(0b111111<<26)),funct=(instr&0b11111);
            unsigned int AluOp=0;
            /*
                 funct=(instr&0b11111),
                 rs=(instr&(0b11111<<21)),
                 rt=(instr&(0b11111<<16)),
                 rd=(instr&(0b11111<<11)),
                 shamt=(instr&(0b11111<<6)),
                 imm=(instr&65535),
                 addr=(instr&67108863);
                 */
            switch (opcode)
            {//RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead
                case 0b000000: //R type
                    semnale=0b1100001000;
                    break;
                
                case 0b000010: //jmp
                    semnale=0b10;
                    break;
                
                case 0b000100: //beq
                    semnale=0b0001000100;
                    break;
                
                case 0b001000: //addi
                    semnale=0b1010000000;
                    break;
                case 0b100011: //lw
                    semnale=0b1010010001;
                    break;
                
                case 0b101011: //sw
                    semnale=0b0010100000;
                    break;
            }
            AluOp=semnale&(0b11<<2);
            unsigned int temp=funct+AluOp<<6;
            unsigned int AluCtrl=0;
            switch (AluOp)
            {
            case 0b00:
                AluCtrl=0b010;
                break;
            
            case 0b01:
                AluCtrl=0b110;
                break;
            
            case 0b10:
                switch (funct)
                {
                case 0b100000:
                    AluCtrl=0b010;
                    break;
                case 0b100010:
                    AluCtrl=0b110;
                    break;
                case 0b100100:
                    AluCtrl=0b000;
                    break;
                case 0b100101:
                    AluCtrl=0b001;
                    break;
                case 0b100110:
                    AluCtrl=0b011;
                    break;
                case 0b101010:
                    AluCtrl=0b111;
                    break;
                }
                break;
            }
            semnale<<=3;
            semnale+=AluCtrl;
            unsigned long int res=instr&67108863;
            res<<=13;
            semnale+=res;
            //26b instr,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead,3b AluCtrl
            sprintf(readbuff,"%lu",semnale);
            write(p_DEC_EX[1],readbuff,strlen(readbuff)+1);

           

            if(instr==0xac080001)
                break;
                
        }
        
        return 0;
    }


        if(fork()==0)
    {
        char readbuff[256];

        close(p_DEC_EX[1]);
        while(1)
        {
            read(p_DEC_EX[0],readbuff,sizeof(readbuff));
            unsigned long int rcvd=atol(readbuff); //26b instr,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead,3b AluCtrl
            printf("received: %ld\n",rcvd);
            unsigned long int instr=67108863;
            
            instr<<=13;
            instr&=rcvd;
            instr>>=13;
            int funct=(instr&0b11111),
                 rs=(instr&(0b11111<<21)),
                 rt=(instr&(0b11111<<16)),
                 rd=(instr&(0b11111<<11)),
                 shamt=(instr&(0b11111<<6)),
                 imm=(instr&65535),
                 AluCtrl=rcvd&0b111,
                 MemRead=rcvd&(0b1<<3),
                 Jump=rcvd&(0b1<<4),
                 AluOp=rcvd&(0b11<<5),
                 MemToReg=rcvd&(0b1<<7),
                 MemWrite=rcvd&(0b1<<8),
                 Branch=rcvd&(0b1<<9),
                 AluSrc=rcvd&(0b1<<10),
                 RegDst=rcvd&(0b1<<11),
                 RegWrite=rcvd&(0b1<<12);
            printf("!!%ld\n",instr);

        }
        return 0;
    }
    else
    {

        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}