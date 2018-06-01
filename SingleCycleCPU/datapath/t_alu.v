/*
    date:   2018/05/31
    tag:    alu test file
*/
`include "alu.v"

module t_alu();
    reg[3:0]    op;
    reg[31:0]   A,B;

    wire[31:0]  y;
    wire        zero;

    parameter
    AND = 4'b0000,
    OR  = 4'b0001,
    add = 4'b0010,
    sub = 4'b0110,
    slt = 4'b0111;

    initial
    begin
      op = add;
      A = 32'd25;
      B = 32'd75;
    end

    always #50 op = op + 4'b1;
    always #500 A = 32'd75;
    always #500 B = 32'd25;

    alu ALU( .op(op), .A(A), .B(B), .y(y), .zero(zero));

endmodule