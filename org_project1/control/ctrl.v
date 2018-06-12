module ctrl( op, regWrite, memtoReg, regDst, aluOp, aluSrc, branch, jump, memWrite);
    input[5:0]      op;

    output reg      regWrite, memtoReg, regDst;
    output reg      memWrite;
    output reg      branch, jump;
    output reg      aluSrc; 
    output reg[1:0] aluOp;


    // control signal
    parameter   ENA = 1'b1;
    parameter   DIS = 1'b0;

    // op type
    parameter   R_TYPE = 6'b000000;
    parameter   LW     = 6'b100011;  
    parameter   SW     = 6'b101011;
    parameter   BEQ    = 6'b000100;
    parameter   J      = 6'b000010;


    always @(op)
    begin
        //----- RegWrite -----
        case (op)
            R_TYPE,
            LW      :   regWrite = ENA;   
            SW,
            BEQ,
            J       :   regWrite = DIS;
            default :   regWrite = 1'b?;
        endcase
    
        //----- MemtoReg -----    
        case (op)
            R_TYPE  :   memtoReg = DIS;
            LW      :   memtoReg = ENA;   
            default :   memtoReg = 1'b?;
        endcase
    
        //----- RegDst -----
        case (op)
            R_TYPE  :   regDst = ENA;
            LW      :   regDst = DIS;   
            default :   regDst = 1'b?;
        endcase        
    
        //----- ALUop -----
        case (op)
            R_TYPE  :   aluOp = 2'b10;
            LW,
            SW      :   aluOp = 2'b00;
            BEQ     :   aluOp = 2'b01;
            default :   aluOp = 2'b??;
        endcase
        
        //----- ALUsrc -----
        case (op)
            R_TYPE,
            BEQ     :   aluSrc = DIS;
            LW,
            SW      :   aluSrc = ENA;
            default :   aluSrc = 1'b?;
        endcase
        
        //----- branch -----
        case (op)
            BEQ     :   branch = ENA;
            default :   branch = DIS;
        endcase
        
        //----- jump -----
        case (op)
            J       :   jump = ENA;
            default :   jump = DIS;
        endcase

        //----- MemWrite -----
        case (op)
            SW      :   memWrite = ENA;
            default :   memWrite = DIS;
        endcase
    end


endmodule
