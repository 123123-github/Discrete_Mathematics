/*
    date:   2018/06/01
    tag:    test data memory
*/

module t_dm();
    reg clk,we;
    reg[11:2] addr;
    reg[31:0] din;

    wire[31:0] dout;

    initial
    begin
        clk = 2'b0;
        we = 2'b0;
        din = 32'habcd_0eff;
        addr = 12'hc03;             // 1100_0000_0011 --> addr = 00_0000_0011 

        #100 we = 1;
    end

    always #10 clk = ~clk;
    always #200 addr = ~addr;
    always #500 din = ~din;

    dm_4k DM( .addr(addr), .din(din), .we(we), .clk(clk), .dout(dout));

endmodule
