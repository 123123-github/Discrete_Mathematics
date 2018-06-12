`timescale 1ns / 1ns

module t_mips();
    reg clk;
    reg rst;

    initial
    begin
        clk = 0;
        rst = 1;
        
        #100 rst = 0;
    end

    always #50 clk = ~clk;


    mips MIPS(.clk(clk), .rst(rst));

endmodule