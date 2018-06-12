`define a func[3]
`define b func[2]
`define c func[1]
`define d func[0]  
`define p ALUOp[1]  
`define q ALUOp[0]

module aluctr( ALUOp, func, alu_op);
    input[1:0]  ALUOp;      // come from main control
    input[5:0]  func;       // instruction[5:0]

    output[3:0] alu_op;      //alu operation type

    assign alu_op[3] = 1'b0;
    assign alu_op[2] = `q | (`c & `p);
    assign alu_op[1] = ~(`b & `p);
    assign alu_op[0] = (`a&`p) | (`d&`p);


endmodule

/*
module aluctr( ALUOp, func, alu_op);
    input[1:0]      ALUOp;      // come from main control
    input[5:0]      func;       // instruction[5:0]

    output reg[3:0] alu_op;     //alu operation type

    // ALUOp    type    1-0         (input)
    parameter   LSW     = 2'b00;
    parameter   R_TYPE  = 2'b10;
    parameter   BEQ     = 2'b01;

    // ALUctr   type    3-0         (output)    
    parameter   AND     = 4'b0000;
    parameter   OR      = 4'b0001;
    parameter   ADD     = 4'b0010;
    parameter   SUB     = 4'b0110;
    parameter   SLT     = 4'b0111;    

    // R-TYPE   func    5-0
    parameter   ADD_F = 6'b100000;
    parameter   SUB_F = 6'b100010;
    parameter   AND_F = 6'b100100;
    parameter   OR_F  = 6'b100101;
    parameter   SLT_F = 6'b101010;
               

    always @(ALUOp or func)
    begin
        case (ALUOp)
            LSW:        alu_op = ADD;
            BEQ:        alu_op = SUB;
            R_TYPE:
                case (func)
                    AND_F:  alu_op = AND; 
                    OR_F:   alu_op = OR;
                    ADD_F:  alu_op = ADD;
                    SUB_F:  alu_op = SUB;
                    SLT_F:  alu_op = SLT;
                    default:alu_op = 4'b????;
                endcase
            default:
                        alu_op = 4'b????; 
        endcase    
    end
    

endmodule
*/

/*------ aluctr ------

    Input(func)     
    
    Operation           Output(to ALU-ALUOp)
    lw/sw           add                 0010
    beq             sub                 0110
    R-type          func     >>
                                AND     0000
                                OR      0001
                                add     0010
                                sub     0110
                                slt     0111
--------------------------------------------------    


            ALUOp   func        output(should be)-alu_op   
    lw/sw   00      XXXXXX      add 0010
    
    beq     01      XXXX        sub 0110
    
    add     10  (10)0000        add 0010
    sub     10      0010        sub 0110
    slt     10      1010        slt 0111
    AND     10      0100        AND 0000
    OR      10      0101        OR  0001
*/