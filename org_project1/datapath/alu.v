module alu( op, A, B, y, zero);
    input[3:0]      op;     // operation
    input[31:0]     A,B;    // operand

    output[31:0]    y;      // result
    reg[31:0]       y;      // !!!  [31:0]
    output          zero;

    parameter
    AND = 4'b0000,
    OR  = 4'b0001,
    ADD = 4'b0010,
    SUB = 4'b0110,
    SLT = 4'b0111;

    //always @(op or A or B)
    always @(*)
    begin
        case (op)
            AND:  y = A & B;
            OR:   y = A | B;
            ADD:  y = A + B;
            SUB:  y = A - B;
            SLT:  y = (A < B)? 32'b1: 32'b0;
            default:  y = 32'b?;            // undefine
        endcase
    end

    assign zero = (A == B)? 1'b1: 1'b0;

endmodule

/*------ ALUop ------

    AND     0000
    OR      0001
    ADD     0010
    SUB     0110
    SLT     0111

*/