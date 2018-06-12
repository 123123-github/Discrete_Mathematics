/*
    date:   06/02/2018
    tag:    test aluctr
*/

module t_aluctr();
    reg[1:0]  ALUop;      // come from main control
    reg[5:0]  func;       // instruction[5:0]

    wire[3:0] aluop;      //alu operation type

    // ALUop
    parameter LSw = 2'b00;
    parameter Beq = 2'b01;
    parameter Rtype = 2'b10;

    // func
    parameter ADD = 6'b10_0000;
    parameter SUB = 6'b10_0010;
    parameter SLT = 6'b10_1010;
    parameter AND = 6'b10_0100;
    parameter OR =  6'b10_0101;


    initial
    begin
        ALUop = LSw;
        func = SUB;
    end

    always #10 func = func + 1;
    always #200 ALUop = ALUop + 1;

    aluctr ALUctr( .ALUop(ALUop), .func(func), .aluop(aluop));

endmodule