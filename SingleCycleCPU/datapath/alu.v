module alu( op, A, B, y, zero);
    input[3:0]      op;     // operation
    input[31:0]     A,B;    // operand

    output[31:0]    y;      // result
    reg[31:0]       y;      // !!!  [31:0]
    output          zero;

    parameter
    AND = 4'b0000,
    OR  = 4'b0001,
    add = 4'b0010,
    sub = 4'b0110,
    slt = 4'b0111;

    always @(op or A or B)
    begin
        case (op)
            AND:  y = A & B;
            OR:   y = A | B;
            add:  y = A + B;
            sub:  y = A - B;
            slt:  y = (A < B)? 32'b1: 32'b0;
            default:  y = 32'b?;            // undefine
        endcase
    end

    assign zero = (A == B)? 1'b1: 1'b0;

endmodule

/*------ ALUop ------

    AND     0000
    OR      0001
    add     0010
    sub     0110
    slt     0111

*/