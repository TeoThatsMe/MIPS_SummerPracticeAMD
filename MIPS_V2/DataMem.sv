module DataMem(input [31:0] Addr,WriteData,
               input MemWrite,MemRead,clk,
               output reg [31:0] ReadData);
  reg [31:0] mem [0:15];
  integer i,l0,l1,l2,l3,l4,l5,l6,l7,l8,l9;
  
  initial
    begin
      for(i=0;i<16;i=i+1)
        begin
          mem[i]=32'b0;
        end
    end
  
  always @(Addr, MemRead)
    begin     
      if(MemRead)
        ReadData=mem[Addr];
    end
  
  
  always @(negedge clk)
    begin
      if(MemWrite)
        mem[Addr]<=WriteData;

    end
  always @*
    begin
      l0=mem[0];
      l1=mem[1];
      l2=mem[2];
      l3=mem[3];
      l4=mem[4];
      l5=mem[5];
      l6=mem[6];
      l7=mem[7];
      l8=mem[8];
      l9=mem[9];
    end
endmodule
               