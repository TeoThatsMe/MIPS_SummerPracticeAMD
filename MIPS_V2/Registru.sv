module Registru(input [31:0] di,
                input clk,pl,
                output reg [31:0] PCout);
  always @(posedge clk)
    PCout<=di;
endmodule