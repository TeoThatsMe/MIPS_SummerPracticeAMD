#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>   

int GIgel(int In0,int In1,int AluCtrl)
{
    printf("asd %d %d %d\n",In0,In1,AluCtrl);
    switch (AluCtrl)
    {
    case 0:
        printf("asd\n");
        return In0&In1;
        break;
    case 0b001:
        return In0|In1;
        break;
    case 0b010:
        return In0+In1;
        break;
    case 0b011:
        return In0&In1;
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
    int regs[32],mem[32];
        for(int i=0;i<32;++i)
        {
            regs[i]=0;
            mem[i]=0;
        }

    char readbuff[256];
    while(1)
        {
            //read(p_DEC_EX[0],readbuff,sizeof(readbuff));
            unsigned long int rcvd=567473805376;
            printf("received: %ld\n",rcvd);
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
                 MemRead=rcvd&(0b1<<3)>>3,
                 Jump=rcvd&(0b1<<4)>>4,
                 AluOp=rcvd&(0b11<<5)>>5,
                 MemToReg=rcvd&(0b1<<7)>>7,
                 MemWrite=rcvd&(0b1<<8)>>8,
                 Branch=rcvd&(0b1<<9)>>9,
                 AluSrc=rcvd&(0b1<<10)>>10,
                 RegDst=rcvd&(0b1<<11)>>11,
                 RegWrite=rcvd&(0b1<<12)>>12,
                 Type=rcvd&(0b11<<13)>>13,
                 instr16=instr&(65535);
            
            unsigned long int ret=0;
            unsigned int rez=0;
            printf("AAAAAAAAAAAAA%lu  \n",instr);
            printf("BBBBBBBBBB%lu  \n",instr);
            printf("!!!%d %d %d %d\n",rs,rt,AluCtrl,Type);
            int a=GIgel(regs[rs],regs[rt],AluCtrl);
            printf("CCCCCCCCCCCc%lu  \n",instr);
            printf("%d %d %d %d\n",Jump,Branch,2,Type);
            if(Jump)
            {
                ret+=instr;
                ret<<=2;
                ret+=0b01;
            }
            else if (Branch && (GIgel(regs[rs],regs[rt],AluCtrl))==0)
            {
                ret+=instr16;
                ret<<=18;
                ret+=0b10;

            }
            else if(Type==0b00)
            {
                if(!RegDst)
                    rd=rt;
                rez=GIgel(regs[rs],rt,AluCtrl);
                if(RegWrite)
                    regs[rd]=rez;
            }
            else if(Type==0b01)
            {
                unsigned int MemOut=0,MuxMemOut=0;
                if(!RegDst)
                    rd=rt;
                rez=GIgel(regs[rs],AluSrc==1?(instr16<<16):regs[rt],AluCtrl);

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
            printf("!!%u\n",rez);

        }
}