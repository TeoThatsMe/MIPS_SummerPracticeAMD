// Code your testbench here
// or browse Examples


/*
module tbPIPO();
  
  
  RegPIPO Testchip(A,clk,CS,D);
  Comparator Validator(A,D,Iesire); 
  reg [3:0] A;
  reg clk;
  reg CS;
  wire [3:0] D;
  wire Iesire;
  
  initial A=4'b1101;
  initial CS=1'b0;
  initial clk=1'b0;
  initial forever #15 clk=~clk;
  
  initial
  begin
    $dumpfile("dump.vcd");
    $dumpvars;
    #30 CS=1'b1;
    #20 CS=1'b0;
    #50 A=4'b0001;
    #10 CS=1'b1;
    #200 $finish;
  end 
endmodule

module tbSIPO();
  
  
  RegSIPO Testchip(A,clk,CS,D);
  Comparator Validator(A,D,Iesire); 
  reg [3:0] A;
  reg clk;
  reg CS;
  wire [3:0] D;
  wire Iesire;
  
  initial A=4'b1101;
  initial CS=1'b1;
  initial clk=1'b0;
  initial forever #15 clk=~clk;
  
  initial
  begin
    $dumpfile("dump.vcd");
    $dumpvars;
    #165 CS=1'b0;
    #30 A=4'b0011;
    #15 CS=1'b1;
    #165 CS=1'b0;
    #200 $finish;
  end 
endmodule
module tbSISO();
  
  
  RegSISO Testchip(A,clk,CS,D);
  ComparatorSI Validator(D,A,clk,CS1,Iesire); 
  reg [3:0] A;
  reg clk;
  reg CS,CS1;
  wire D;
  wire Iesire;
  
  initial A=4'b1101;
  initial CS=1'b1;
  initial CS1=1'b0;
  initial clk=1'b1;
  initial forever #15 clk=~clk;
  
  initial
  begin
    $dumpfile("dump.vcd");
    $dumpvars;
    #120 CS1=1'b1;
    #135 CS1=1'b0;
    #30 A=4'b0010;
    #120 CS1=1'b1;
    #135 CS1=1'b0;
    #30 $finish;
  end 
endmodule
*/
module tbPISO();
  
  
  RegPISO Testchip(A,clk,CS,L,D);
  ComparatorSIV2 Validator(D,A,clk,CS1,Iesire); 
  reg [3:0] A;
  reg clk;
  reg CS,CS1;
  reg L;
  wire D;
  wire Iesire;
  
  initial A=4'b1101;
  initial CS=1'b1;
  initial CS1=1'b0;
  initial clk=1'b1;
  initial L=1'b1;
  initial forever #15 clk=~clk;
  
  initial
  begin
    $dumpfile("dump.vcd");
    $dumpvars;
    #30 L=1'b0; CS1=1'b1;
    #135 CS1=1'b0;
    #30 $finish;
  end 
endmodule