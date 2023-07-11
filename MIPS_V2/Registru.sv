module Registru(input [31:0] di,
                input clk,pl,
                output reg [31:0] out);
  always @(posedge clk)
    out<=di;
endmodule