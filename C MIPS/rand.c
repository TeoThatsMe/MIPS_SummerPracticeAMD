#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>   

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

int ALU(int In0,int In1,int AluCtrl)
{
    switch (AluCtrl)
    {
    case 0b000:
        return In0&In1;
        break;
    case 0b001:
        return In0|In1;
        break;
    case 0b010:
        return In0+In1;
        break;
    case 0b011:
        return In0^In1;
        break;
    case 0b110:
        return In0-In1;
        break;
    case 0b111:
        return In0<In1;
        break;
    
    }
}

int main()
{
    int p_PC_IM[2],p_IM_DEC[2],p_DEC_EX[2],p_EX_PC[2];
    int p_clk[4][2];

    if(pipe(p_clk[0])<0)
        return EXIT_FAILURE;
    if(pipe(p_clk[1])<0)
        return EXIT_FAILURE;
    if(pipe(p_clk[2])<0)
        return EXIT_FAILURE;
    if(pipe(p_clk[3])<0)
        return EXIT_FAILURE;

    if(pipe(p_PC_IM)<0)
        return EXIT_FAILURE;

    if(pipe(p_IM_DEC)<0)
        return EXIT_FAILURE;
    
    if(pipe(p_DEC_EX)<0)
        return EXIT_FAILURE;
    
    if(pipe(p_EX_PC)<0)
        return EXIT_FAILURE;
    


    if(fork()==0)
    {
        char s[256];
        int count=0;
        int clk=0;
        close(p_PC_IM[0]);
        close(p_IM_DEC[0]);
        close(p_IM_DEC[1]);
        close(p_DEC_EX[0]);
        close(p_DEC_EX[1]);
        close(p_EX_PC[1]);
        close(p_clk[0][1]);
        close(p_clk[1][0]);
        close(p_clk[1][1]);
        close(p_clk[2][0]);
        close(p_clk[2][1]);
        close(p_clk[3][0]);
        close(p_clk[3][1]);

        while(read(p_clk[0][0],&clk,sizeof(int)))
        {
            sprintf(s,"%d",count);
            printf("\tPC=%d\n\n",count);

            write(p_PC_IM[1],s,sizeof(s));
            //sleep(2);
            count++;
            unsigned long int citit=0;
            read(p_EX_PC[0],&citit,sizeof(unsigned long int));  
            if((citit&0b11)==0b01)
            {
                citit-=1;
                citit+=(count&(0b1111<<28));
                citit>>=2;
                count=citit;

            }
            else if((citit&0b11)==0b10)
            {
                citit>>=18;
                short int offset=((citit&(1<<15))>>15==1) ? citit | (65535<<16) : citit;
                count+=offset;
            }
        }
        close(p_clk[0][0]);
        close(p_EX_PC[0]);
        close(p_PC_IM[1]);
        return 0;
    }



    if(fork()==0)
    {
        char s[256];
        int clk=0;
        close(p_PC_IM[1]);

        close(p_IM_DEC[0]);

        close(p_DEC_EX[0]);
        close(p_DEC_EX[1]);

        close(p_EX_PC[1]);

        close(p_clk[0][1]);
        close(p_clk[0][0]);

        close(p_clk[1][1]);

        close(p_clk[2][0]);
        close(p_clk[2][1]);

        close(p_clk[3][0]);
        close(p_clk[3][1]);

        char readbuff[256];
        char * stop;

        int nrInstr=nrLinii("iesire.hex");
        int nrInstr1=nrLinii("test_prelucrat.asm");
        nrInstr+=1;
        nrInstr1+=1;
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
            fgets(readbuff,255,instructiuni1);
            if(i!=nrInstr1-1)
                readbuff[strlen(readbuff)-1]='\0';
            strcpy(arr1[i],readbuff);
        }

        while(read(p_clk[1][0],&clk,sizeof(int)))
        {
            read(p_PC_IM[0],readbuff,sizeof(readbuff));
            sprintf(s,"%u",arr[atoi(readbuff)]);
            printf("\tInstructiunea %x %s\n\n",arr[atoi(readbuff)],arr1[atoi(readbuff)]);
            write(p_IM_DEC[1],s,sizeof(s));
        }
        close(p_clk[1][0]);
        close(p_IM_DEC[1]);
        close(p_PC_IM[0]);
        return 0;
    }



    if(fork()==0)
    {
        char readbuff[256];
        int clk=0;

        close(p_PC_IM[0]);
        close(p_PC_IM[1]);

        close(p_IM_DEC[1]);

        close(p_DEC_EX[0]);

        close(p_EX_PC[0]);
        close(p_EX_PC[1]);
        close(p_clk[0][1]);
        close(p_clk[0][0]);
        close(p_clk[1][0]);
        close(p_clk[1][1]);
        close(p_clk[2][1]);
        close(p_clk[3][0]);
        close(p_clk[3][1]);

        while (read(p_clk[2][0],&clk,sizeof(int)))
        {

            read(p_IM_DEC[0],readbuff,sizeof(readbuff));
            unsigned int instr=atoi(readbuff);
            unsigned long int semnale=0; 
            unsigned int opcode=(instr&(0b111111<<26))>>26,funct=(instr&0b111111);
            unsigned int AluOp=0;
            switch (opcode)
            {//2b Type,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead
                case 0b000000: //R type
                    semnale=0b001100001000;
                    break;
                
                case 0b000010: //jmp
                    semnale=0b100000000010;
                    break;
                
                case 0b000100: //beq
                    semnale=0b010001000100;
                    break;
                
                case 0b001000: //addi
                    semnale=0b011010000000;
                    break;
                case 0b100011: //lw
                    semnale=0b011010010001;
                    break;
                
                case 0b101011: //sw
                    semnale=0b010010100000;
                    break;
            }
            AluOp=((semnale&(0b11<<2))>>2);
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
                    printf("funct=%d\n",funct);
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
            res<<=15;
            semnale+=res;
            //26b instr,2b Type,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead,3b AluCtrl
            printf("\t26b instr,2b Type,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead,3b AluCtrl\n");
            printf("\tSemnale control concatenate %lx\n\n",semnale);
            sprintf(readbuff,"%lu",semnale);
            write(p_DEC_EX[1],readbuff,strlen(readbuff)+1);
        }
        
        close(p_DEC_EX[1]);
        close(p_clk[2][0]);
        close(p_IM_DEC[0]);
        return 0;
    }


    
    
    
    if(fork()==0)
    {
        int regs[32],mem[32];
        for(int i=0;i<32;++i)
        {
            regs[i]=0;
            mem[i]=0;
        }

        char readbuff[256];
        int clk=0;

        close(p_PC_IM[0]);
        close(p_PC_IM[1]);

        close(p_IM_DEC[0]);
        close(p_IM_DEC[1]);

        close(p_DEC_EX[1]);

        close(p_EX_PC[0]);

        close(p_clk[0][1]);
        close(p_clk[0][0]);
        close(p_clk[1][0]);
        close(p_clk[1][1]);
        close(p_clk[2][0]);
        close(p_clk[2][1]);
        close(p_clk[3][1]);

        while(read(p_clk[3][0],&clk,sizeof(int)))
        {
            read(p_DEC_EX[0],readbuff,sizeof(readbuff));
            unsigned long int rcvd=atol(readbuff); //26b instr,2b Type,RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,2b AluOp,Jump,MemRead,3b AluCtrl 011010000000010
            unsigned long int instr=67108863;
            
            instr<<=15;
            instr&=rcvd;
            instr>>=15;
            int funct=(instr&0b11111),
                 rs=(instr&(0b11111<<21))>>21,
                 rt=(instr&(0b11111<<16))>>16,
                 rd=(instr&(0b11111<<11))>>11,
                 shamt=(instr&(0b11111<<6))>>6,
                 imm=(instr&65535),
                 AluCtrl=rcvd&0b111,
                 MemRead=(rcvd&(0b1<<3))>>3,
                 Jump=(rcvd&(0b1<<4))>>4,
                 AluOp=(rcvd&(0b11<<5))>>5,
                 MemToReg=(rcvd&(0b1<<7))>>7,
                 MemWrite=(rcvd&(0b1<<8))>>8,
                 Branch=(rcvd&(0b1<<9))>>9,
                 AluSrc=(rcvd&(0b1<<10))>>10,
                 RegDst=(rcvd&(0b1<<11))>>11,
                 RegWrite=(rcvd&(0b1<<12))>>12,
                 Type=(rcvd&(0b11<<13))>>13,
                 instr16=instr&(65535);
            //printf("%d",rcvd&0b111);
            unsigned long int ret=0;
            unsigned int rez=0;
            if(Jump)
            {
                printf("\tJump\n");
                ret+=instr;
                ret<<=2;
                ret+=0b01;
            }
            else if (Branch && (ALU(regs[rs],regs[rt],AluCtrl))==0)
            {
                printf("\tBranch\n");
                ret+=instr16;
                ret<<=18;
                ret+=0b10;

            }
            else if(Type==0b00)
            {
                printf("\tR type\n");
                if(!RegDst)
                    rd=rt;
                rez=ALU(regs[rs],regs[rt],AluCtrl);
                if(RegWrite)
                    regs[rd]=rez;
            }
            else if(Type==0b01)
            {
                printf("\tI type\n");
                unsigned int MemOut=0,MuxMemOut=0;
                if(!RegDst)
                    rd=rt;
                int asd=AluSrc==1?(instr16):regs[rt];
                rez=ALU(regs[rs],asd,AluCtrl);

                if(MemRead)
                    MemOut=mem[rez];
                
                if(MemWrite)
                    mem[rez]=regs[rt];
                
                if(MemToReg)
                    MuxMemOut=MemOut;
                else
                    MuxMemOut=rez;
                
                if(RegWrite)
                    regs[rd]=MuxMemOut;
                
                rez=MuxMemOut;

            }
            printf("\tIesire Mux DM %u\n\n\n\n",rez);
            write(p_EX_PC[1],&ret,sizeof(unsigned long int));

        }
        close(p_clk[3][0]);
        close(p_EX_PC[1]);
        close(p_DEC_EX[0]);
        return 0;
    }

    if(fork()==0)
    {
        int ind=0;
        float wait_time=1;
        
        close(p_clk[0][0]);
        close(p_clk[1][0]);
        close(p_clk[2][0]);
        close(p_clk[3][0]);
        
        close(p_PC_IM[0]);
        close(p_PC_IM[1]);
        
        close(p_IM_DEC[1]);
        close(p_IM_DEC[0]);
        
        close(p_DEC_EX[1]);
        close(p_DEC_EX[0]);

        close(p_EX_PC[0]);
        close(p_EX_PC[1]);
        while(1)
        {
            int rand=0;
            printf("--------------\nBlock PC Ciclul %d:\n",ind++);
            write(p_clk[rand++][1],&rand,sizeof(int));
            sleep(wait_time);
            printf("Block IM Ciclul %d:\n",ind++);
            write(p_clk[rand++][1],&rand,sizeof(int));
            sleep(wait_time);
            printf("Block DEC Ciclul %d:\n",ind++);
            write(p_clk[rand++][1],&rand,sizeof(int));
            sleep(wait_time);
            printf("Block EX Ciclul %d:\n",ind++);
            write(p_clk[rand++][1],&rand,sizeof(int));
            sleep(wait_time);
        }
        close(p_clk[0][1]);
        close(p_clk[1][1]);
        close(p_clk[2][1]);
        close(p_clk[3][1]);
        return 0;
    }
    else
    {

        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}