// Code your design her
`include "Mux_2x1.sv"
`include"Adder.sv"
`include"Registru.sv"
`include "InstructionMemory.sv"
`include"Alu.sv"
`include"AluControl.sv"
`include"Control.sv"
`include"Registers"
`include"DataMem.sv"

module MIPS(input [31:0] DI,
  input clk,start);
  
  
  wire [31:0] outReg;
  wire [31:0] outAdder4,outAdderBranch,outReadData1,outReadData2,outAlu;
  wire [31:0] outMuxIntrare,outMux1,outMux2,outMuxWR,outMuxAlu,outMuxDM;
  wire [31:0] outIM,outDM;
  wire [4:0] WriteReg;
  wire RegDst,Jump,Branch,MemRead,MemToReg,MemWrite,AluSrc,RegWrite,Zero;
  wire [1:0] AluOp;
  wire [2:0] AluCtrl;

  
  //Cale PC 
  Mux_2x1 muxIntrare(outMux2,DI,start,outMuxIntrare);
  Registru re(outMuxIntrare,clk,1'b1,outReg);
  Adder adder4(outReg,4,outAdder4);
  Adder adderBranch(outAdder4,{{16{outIM[15]}},outIM[15:0]}<<2,outAdderBranch); //offset branch
  Mux_2x1 muxPC1(outAdder4,outAdderBranch,Branch&Zero,outMux1); // semnal branch si zero
  Mux_2x1 muxPC2(outMux1,{outAdder4[31:28],outIM[25:0]}<<2,Jump,outMux2); // adresa jump si semnal jump
  
  //Instruction Memory
  InstructionMemory im(outReg,outIM);
  
  //Regs + ALU + Control
  Control ctrl(outIM[31:26],RegDst,Jump,Branch,MemRead,MemToReg,MemWrite,AluSrc,RegWrite,AluOp);
  AluControl aluControl(AluOp,outIM[5:0],AluCtrl);
  
  Mux_2x1 muxWR({27'b0,outIM[20:16]},{27'b0,outIM[15:11]},RegDst,outMuxWR);
  Registers registers(RegWrite,outIM[25:21],outIM[20:16],outMuxWR[4:0],outMuxDM,clk,outReadData1,outReadData2);
  
  Mux_2x1 muxAlu(outReadData2,{{16{outIM[15]}},outIM[15:0]},AluSrc,outMuxAlu);
  Alu alu(outReadData1,outMuxAlu,AluCtrl,Zero,outAlu);
  
  
  //Data Memory
  DataMem dm(outAlu,outReadData2,MemWrite,MemRead,clk,outDM);
  Mux_2x1 muxDM(outAlu,outDM,MemToReg,outMuxDM);
  
  
endmodule