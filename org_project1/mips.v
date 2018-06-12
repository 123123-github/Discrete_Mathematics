`timescale 1ns / 1ps

module mips( clk, rst);
    input           clk;    // clock
    input           rst;    // reset

    
    // *************** Module Port Signal Declaration ***************
    // moduel Control
    wire            RegWrite;
    wire            RegDst;
    wire            MemtoReg;
    
    wire            Jump;
    wire            Branch;

    wire            ALUSrc;
    wire[1:0]       ALUOp;
    
    wire            MemWrite;
    
    // module PC
    wire[31:2]      pc_in;
    wire[31:2]      pc;

    // module IM
    wire[11:2]      im_addr;
    wire[31:0]      instr;

    // split the instruction
    wire[4:0]       rs,rt,rd;
    wire[5:0]       op,func;
    wire[25:0]      j_target;
    wire[15:0]      imm16;
    wire[31:0]      imm32;

    // module RegFile
    wire[4:0]       rw;
    wire[31:0]      busW;
    wire[31:0]      busA, busB;
    
    // module ALU
    wire[3:0]       alu_op;
    wire[31:0]      alu_a, alu_b;
    wire[31:0]      alu_y;
    wire            zero;

    // module DM
    wire[11:2]      dm_addr;
    wire[31:0]      dm_in;
    wire[31:0]      dm_out;

    // moduel NPC
    wire[31:2]      br_target;
    wire[31:2]      npc;


    // *************** Connect All Modules ***************
    // ---------- PC ----------
    pc PC(
        .clk(clk), 
        .rst(rst), 
        .npc(pc_in),
        .pc(pc)
    );

    assign pc_in =      npc;


    // ---------- NPC ----------
    npc NPC( 
        .pc(pc), 
        .j_target(j_target), 
        .jump(Jump), 
        .br_target(br_target), 
        .branch(Branch), 
        .zero(zero), 
        .npc(npc)
    );
    
    assign j_target =   instr[25:0];
    assign br_target =  imm32[31:2];


    // ---------- IM ----------
    im_4k IM(
        .addr(im_addr), 
        .dout(instr)
    );
    
    assign im_addr =    pc;
        
    // ---------- RegFile ----------
    regfile RegFile( 
        .clk(clk),
        .we(RegWrite), 
        .rs(rs),
        .rt(rt),
        .rw(rw),
        .din(busW),
        .a(busA),
        .b(busB)
    );

    assign rs    =  instr[25:21];
    assign rt    =  instr[20:16];
    assign rd    =  instr[15:11];
    assign rw =     RegDst? rd: rt;  
    assign busW =   MemtoReg? dm_out: alu_y;

    // ---------- ALU ----------
    alu ALU(
        .op(alu_op),
        .A(alu_a),
        .B(alu_b),
        .y(alu_y),
        .zero(zero)
    );
    
    assign imm16 =  instr[15:0];
    assign imm32 =  { { 16{imm16[15]}}, imm16};     // signExt

    assign alu_a =  busA;
    assign alu_b =  ALUSrc? imm32: busB;

    // ---------- DM ----------
    dm_4k DM( 
        .addr(dm_addr),
        .din(dm_in),
        .we(MemWrite),
        .clk(clk),
        .dout(dm_out)
    );

    assign dm_addr =    alu_y[11:2];
    assign dm_in   =    busB;
    
    // ---------- ALUCtr ----------
    aluctr ALUCtr(
        .ALUOp(ALUOp),
        .func(func),
        .alu_op(alu_op)
    );

    assign func  =  instr[5:0]; 
    
    // ---------- Main Control ----------
    ctrl CTRL( 
        .op(op),
        .regWrite(RegWrite), 
        .memtoReg(MemtoReg), 
        .regDst(RegDst), 
        .aluOp(ALUOp), 
        .aluSrc(ALUSrc), 
        .branch(Branch), 
        .jump(Jump), 
        .memWrite(MemWrite)
    );
    
    assign op    =  instr[31:26];
    

endmodule