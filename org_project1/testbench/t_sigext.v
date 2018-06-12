/*
    date:   2018/06/02
    tag:    test sigext
*/
module t_sigext();
    reg[15:0]   a;

    wire[31:0]  y;

    initial
    begin
        a = 16'h0012;
        #50 a = 16'h0123; 
        #100 a = 16'hffff;
        #200 a = 16'h8000;
        #220 a = 16'h0000;
    end

    sigext signEXt( .a(a), .y(y));

endmodule