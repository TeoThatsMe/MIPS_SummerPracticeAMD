module Control(input [5:0] instr,
               output reg RegDst,Jump,Branch,MemRead,MemToReg,MemWrite,AluSrc,RegWrite,
               output reg [1:0] AluOp);
  
  
  always @(instr)
    begin
      case(instr)
        6'b000000:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b1_1_0_0_0_0_10_0_0;//R type
        6'b100011:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b1_0_1_0_0_1_00_0_1;//1w
        6'b101011:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b0_0_1_0_1_0_00_0_0;//sw
        6'b000100:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b0_0_0_1_0_0_01_0_0;//beq
        6'b001000:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b1_0_1_0_0_0_00_0_0;//addi
        6'b000010:{RegWrite,RegDst,AluSrc,Branch,MemWrite,MemToReg,AluOp,Jump,MemRead}=10'b0_0_0_0_0_0_00_1_0;//jmp
          endcase
    end
  
endmodule