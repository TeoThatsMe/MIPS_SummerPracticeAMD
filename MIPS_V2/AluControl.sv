module AluControl(input [1:0] AluOp,
                  input [5:0] Func,
                  output reg [2:0] AluCtrl);
  always @(AluOp, Func)
    begin
      casex({AluOp,Func})
        8'b00_xxxxxx:AluCtrl=3'b010;
        8'b1x_100000:AluCtrl=3'b010;
        8'b1x_100010:AluCtrl=3'b110;
        8'b1x_100100:AluCtrl=3'b000;
        8'b1x_100101:AluCtrl=3'b001;
        8'b1x_100110:AluCtrl=3'b011;
        8'b1x_101010:AluCtrl=3'b111;
        8'bx1_xxxxxx:AluCtrl=3'b110;
      endcase
    end
endmodule