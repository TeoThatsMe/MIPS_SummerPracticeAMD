module Alu(input [31:0] In0,In1,
           input [2:0] AluCtrl,
           output reg Zero,
           output reg [31:0] AluResult);
  always @(In0,In1,AluCtrl)
    begin
      case(AluCtrl)
        3'b000:AluResult=In0&In1;
        3'b001:AluResult=In0|In1;
        3'b010:AluResult=In0+In1;
        3'b011:AluResult=In0^In1;
        3'b110:AluResult=In0-In1;
        3'b111:AluResult=(In0<In1);
      endcase
      Zero=AluResult==0;
    end
  
  
endmodule