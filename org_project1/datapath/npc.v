module npc( pc, j_target, jump, br_target, branch, zero, npc);
    input[31:2]     pc;
    input[25:0]     j_target;
    input           jump;
    input[31:2]     br_target;          
    input           branch, zero;

    output[31:2]    npc;                        // next pc
    

    wire[31:2]  pc_plus_4;
    wire[31:2]  j_addr, br_addr;                // the final address (jump OR branch)
    wire[31:2]  addr_after_br;                  // the result select by branch

    assign pc_plus_4 = pc + 1;                  // because pc is 30-bit
    assign j_addr = { pc[31:28], j_target};
    assign br_addr = br_target + pc_plus_4;
    
    assign addr_after_br = (branch & zero)? br_addr: pc_plus_4;         // equal to mux2 
    assign npc = jump? j_addr: addr_after_br;                           


endmodule