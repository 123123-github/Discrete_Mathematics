module regfile( clk, we, rs, rt, rw, din, a, b);
    input           clk;                // clock
    input           we;                 // write enable
    input[4:0]      rs, rt;             // read from:   rs, rt
    input[4:0]      rw;                 // write to:    rw (rd or rt)
    input[31:0]     din;                // the data which is written to registers

    output[31:0]    a,b;                // [rs] --> a;  [rt] --> b

    reg[31:0]       regs[31:0];         // 32 MIPS general-purpose registers

    initial
    begin
        regs[0] = 32'b0;                // $zero
        //regs[28] = 32'h1000_8000;       // $gp
    end

    assign a = regs[rs];
    assign b = regs[rt];

    always @(posedge clk)
        if (we && rw)                   // avoid modifying the value of $zero  !!! (rw) means rw is not zero
            regs[rw] <= din;


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