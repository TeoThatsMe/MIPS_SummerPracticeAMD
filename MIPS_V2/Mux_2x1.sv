module Mux_2x1(input [31:0] I0,I1,
               input sel,
               output [31:0] out);
  assign out= sel ? I1:I0;
endmodule