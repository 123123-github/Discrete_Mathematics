/*
    date:   2018/06/01
    tag:    test im_4k
*/

module t_im();
    reg[11:2]   addr;           // address bus
    
    wire[31:0]  dout;           // 32-bit memory output

    parameter tst = "D:\\Exp\\Project1\\test.txt";
    parameter src = "D:\\Exp\\Project1\\code.txt";

    initial
    begin
        addr = 10'h000;
    end

    always #20 addr = addr + 1;

    //im_4k #(tst) IM( .addr(addr), .dout(dout));
    im_4k IM( .addr(addr), .dout(dout));    

endmodule