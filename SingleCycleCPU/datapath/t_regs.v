module t_regs();
    reg clk,we;
    reg[31:0]   din;
    reg[4:0]    rs, rt, rw;

    wire[31:0]  a, b;
    

    initial
    fork
        clk = 0;
        we = 0;
        din = 32'abcd_1234_abcd_5678;
        rs = 5'd16;
        rt = 5'd0;

        #20 we = 1;

        rw = 5'd16;
        #20 din = 32'd15;       // $16 <- 15

        #30 rw = 5'd17;         // $17 <- 25
        #30 din = 32'd25;
        
        #100 rw = 5'd18
        #100 din = 32'd666;     // 100`time write
        
        
    join

    always #10 clk = ~clk;
    


endmodule