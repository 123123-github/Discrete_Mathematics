/*


*/

module t_npc();
    reg[31:2]     pc;
    reg[25:0]     j_target;
    reg           jump;
    reg[31:2]     br_target;          
    reg           branch, zero;

    wire[31:2]    npc;                        // next pc
    

    initial
    begin
        pc = 32'h6000_0001;
        j_target = 32'h6000_7000;
        br_target = 32'h6000_b000;

        jump = 0;
        branch = 0;
        zero = 1;
    end



    initial
    fork
        #100 jump = 1;

        #150 jump = 0;
        #200 branch = 1;
        #200 zero = 0;
        #300 zero = 1;
    join

    npc NPC( .pc(pc), .j_target(j_target), .jump(jump), .br_target(br_target), .branch(branch), .zero(zero), .npc(npc));
    

endmodule