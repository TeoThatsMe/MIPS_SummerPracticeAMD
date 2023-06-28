// Code your design here
module BistabilD(input A,
                 input clk,
                 output reg O);
  always @(posedge clk)
    O<=A;
endmodule


module RegPIPO(input [3:0]A,
               input clk,
               input CS,
               output [3:0] O);
  
  BistabilD B0(A[0],clk & CS,O[0]);
  BistabilD B1(A[1],clk & CS,O[1]);
  BistabilD B2(A[2],clk & CS,O[2]);
  BistabilD B3(A[3],clk & CS,O[3]);

endmodule 

module RegSIPO(input [3:0]A,
               input clk,
               input CS,
               output [3:0] O);
  integer n=0;
  always @(posedge (clk & CS))
    n<=n+1;
  always @(A)
    n=0;
  
  wire t;
  assign t=A>>(4-n);
  BistabilD B0(t,clk & CS,O[0]);
  BistabilD B1(O[0],clk & CS,O[1]);
  BistabilD B2(O[1],clk & CS,O[2]);
  BistabilD B3(O[2],clk & CS,O[3]);
endmodule

module Comparator(input [3:0]A,
                  input [3:0]B,
                  output O);
  assign O = ((A==B)? 1'b1 : 1'b0);
endmodule


module RegSISO(input [3:0]A,
               input clk,
               input CS,
               output O);
  integer n=0;
  always @(posedge (clk & CS))
    n<=n+1;
  always @(A)
    n=0;
  
  wire t;
  wire w1,w2,w3;
  assign t=A>>(4-n);
  BistabilD B0(t,clk & CS,w1);
  BistabilD B1(w1,clk & CS,w2);
  BistabilD B2(w2,clk & CS,w3);
  BistabilD B3(w3,clk & CS,O);
endmodule

module RegPISO(input [3:0]A,
               input clk,
               input CS,
               input L,
               output O);
  wire w1,w2,w3;
  BistabilD B0(A[0],clk & CS,w1);
  BistabilD B1(((L==1)? A[1] : w1),clk & CS,w2);
  BistabilD B2(((L==1)? A[2] : w2),clk & CS,w3);
  BistabilD B3(((L==1)? A[3] : w3),clk & CS,O);
endmodule

module ComparatorSI(input A,
                    input [3:0] B,
                    input clk,
                    input CS,
                    output O);
  reg [3:0] temp;
  reg [3:0] temp2;
  integer n=0;
  initial temp=4'b0000;
  always @(B)
    n=0;
  always @(posedge (clk & CS))
	begin
      n<=n+1;
      temp[0]=A;
      if(n!=4)
      	temp=temp<<1;
    end
  assign O = ((temp==B) ? 1'b1 : 1'b0);
  
endmodule


module ComparatorSIV2(input A,
                    input [3:0] B,
                    input clk,
                    input CS,
                    output O);
  reg [3:0] temp;
  reg [3:0] temp2;
  integer n=0;
  initial temp=4'b0000;
  always @(B)
    n=0;
  always @(posedge (clk & CS))
	begin
      n<=n+1;
      temp[0]=A;
      if(n<3)
      	temp=temp<<1;
    end
  assign O = ((temp==B) ? 1'b1 : 1'b0);
  
endmodule