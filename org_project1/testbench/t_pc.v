/*
    date:   2018/06/02
    tag:    test pc
*/
module t_pc();
    reg clk,rst;
    reg[31:2] npc;

    wire[31:2] pc;

    initial
    begin
        clk = 0;
        rst = 0;
        npc = 32'h0000_000c;            // npc = 0x0000_000c low 30 bits
    end
    
    always #25 clk = ~clk;

    //always #50 npc = npc + 1;               // setup-time ???

    always @(posedge clk)
    begin
        #45 npc = npc + 1;
    end

    pc PC( .clk(clk), .rst(rst), .npc(npc), .pc(pc));


endmodule