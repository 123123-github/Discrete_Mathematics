/*
    date:   2018/05/32
    tag:    mux2 with parameter testbench
*/

module t_mux2();
    reg         sl;
    reg[5:0]    a,b;

    wire[5:0]   y;

    initial
    begin
        sl = 0;
        a = 6'b000000;
        b = 6'b111111;
    end
    
    always #20 sl = ~sl;
    
    mux2 #(5)   m1( .sl(sl), .a(a), .b(b), .y(y));
    mux2 #(6)   m2( .sl(sl), .a(a), .b(b), .y(y));
    mux2 #(32)  m3( .sl(sl), .a(a), .b(b), .y(y));
    
endmodule
