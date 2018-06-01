module regs( clk, we, rs, rt, rw, din, a, b);
    input           clk;                // clock
    input           we;                 // write enable
    input[4:0]      rs, rt;             // read from:   rs, rt
    input[4:0]      rw;                 // write to:    rw (rd or rt)
    input[31:0]     din;                // the data which is written to registers

    output[31:0]    a,b;                // [rs] --> a;  [rt] --> b

    reg[31:0]       regf[31:0];         // 32 MIPS general-purpose register

    initial
    begin
        regf[0] = 32'b0;                // $zero
        regf[28] = 32'h1000_8000;       // $gp
    end

    assign a = regf[rs];
    assign b = regf[rt];

    always @(posedge clk)
        if (we && (!rw))                // avoid modifying the value of $zero
            regf[rw] <= din;


endmodule

/*
    rs----->    busA
                >       read
    rt----->    busB
-----------------------------------------
    rt----->
rw              >mux2   write
    rd----->
*/