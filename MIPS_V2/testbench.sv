// Code your testbench here
// or browse Examples
/*
module tb_Mux;
  reg [31:0] I0,I1;
  reg sel;
  wire [31:0] out;
  
  Mux_2x1 test(I0,I1,sel,out);
  
  initial
    begin
      I0=32'b0;
      I1=5;
      sel=1'b1;
    end
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 sel=1'b0;
      #20 sel=1'b1;
      #20 $finish;
    end
endmodule



module tb_Adder;
  reg [31:0] I0,I1;
  wire [31:0] out;
  
  Adder test(I0,I1,out);
  
  initial
    begin
      I0=32'b0;
      I1=5;
    end
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 I0=11;I1=21;
      #20 $finish;
    end
endmodule


module tb_Registru;
  reg [31:0] I0;
  reg clk,pl;
  wire [31:0] out;
  
  Registru test(I0,clk,pl,out);
  
  initial
    begin
      I0=25;
      clk=1'b0;
      pl=1'b0;
    end
  
  initial forever #10 clk=~clk;
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 pl=1'b1;
      #20 pl=1'b0;
      #20 I0=1;
      #20 pl=1'b1;
      #20 $finish;
    end
endmodule

module AluTb;
  reg [31:0] I0,I1;
  reg [2:0] AluCtrl;
  wire Zero;
  wire [31:0] AluResult;
  
  
  Alu test(I0,I1,AluCtrl,Zero,AluResult);
  
  initial I0=32'h02021212;
  initial I1=32'haaaaaaaa;
  initial AluCtrl=3'b000;
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 AluCtrl=3'b001;
      #20 AluCtrl=3'b010;
      #20 AluCtrl=3'b110;
      #20 I1=32'h02021212;
      #20 I1=I1+1;
      #20 AluCtrl=3'b111;
      #20 I1=I1-2;
      
      #20 $finish(1);
      
    end
endmodule



module DataMemTb;
  reg [31:0] Addr,WriteData;
  reg MemWrite,MemRead;
  wire [31:0] ReadData;
	
  DataMem test(Addr,WriteData,MemWrite,MemRead,ReadData);

  initial Addr=32'b0;
  initial WriteData=32'h02020202;
  initial MemWrite=1'b0;
  initial MemRead=1'b0;
  
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 MemWrite=1'b1;
      #20 Addr=1;WriteData=20;
      #20 MemWrite=1'b0;Addr=0;
      #20 MemRead=1'b1;
      #20 Addr=1;
      #20 $finish(1);
    end  
endmodule
*/  



module tb;
  reg [31:0] I0;
  reg clk,start;
  wire [31:0] out;
  
  MIPS test(32'b0,clk,start);
  
  initial
    begin
      I0=25;
      clk=1'b0;
      start=1'b1;
    end
  
  initial forever #10 clk=~clk;
  
  initial
    begin
      $dumpfile("dump.vcd");
      $dumpvars;
      #20 start=1'b0;
      #1000 $finish;
    end
endmodule

