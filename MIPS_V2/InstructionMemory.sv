module InstructionMemory(input [31:0] pc,
                         output [31:0] instr);
  reg [31:0] mem[0:500];
  initial $readmemh("instructiuni.txt",mem);
  assign instr=mem[pc>>2];
endmodule