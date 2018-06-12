/*
    date:   06/06/0218
    tag:    test ctrl
*/

module t_ctrl();
    reg[5:0]    op;

    wire        regWrite, memtoReg, regDst;
    wire        memWrite;
    wire        branch, jump;
    wire        aluSrc; 
    wire[1:0]   aluOp;

    parameter   R_TYPE = 6'b000000;
    parameter   LW     = 6'b100011;  
    parameter   SW     = 6'b101011;
    parameter   BEQ    = 6'b000100;
    parameter   J      = 6'b000010;

    initial
    begin
        op = R_TYPE;
        #50 op = LW;
        #50 op = SW;
        #50 op = BEQ;
        #50 op = J;
    end


    ctrl CTRL( op, regWrite, memtoReg, regDst, aluOp, aluSrc, branch, jump, memWrite);


endmodule