module pc( clk, rst, npc, pc);
    input           clk,rst;        // clock reset
    input[31:2]     npc;            // next pc

    output[31:2]    pc;

    reg[31:2]       data;           // store pc (lase clock posedge)
    

    always @(posedge clk or posedge rst)
    begin
        if (rst)
        //  data <= 32'h0000_3000;  // real address -> 3000 -> 00 11_00 00_00 00_00 00_00 -> 0000_0000_0000_c000

        // real pc addr -> {0c00, 00} -> 00 00_11 00_00 00_00 00_00 -> 3000 --> to match with the Memory Configuratio of MARS
            data <= 32'h0000_0c00;      
        else
            data <= npc;
    end

    assign pc = data;


endmodule