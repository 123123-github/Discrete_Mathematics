module im_4k( addr, dout);
    input[11:2]  addr;          // address bus

    output[31:0]  dout;         // 32-bit memory output

    reg[31:0]  im[1023:0];      // 1024 32-bit instruction memory

    parameter src = "D:\\Exp\\Project1\\code.txt";
    
    initial
    begin
        //$readmemh("D:\\Exp\\Project1\\code.txt", im);
        $readmemh(src, im);
    end
    
    assign dout = im[ addr[11:2]];

endmodule

/*
    instruction.dat is used to init the instruction memory
    1024=2^10, so 3 bit hex addresses is enough(in *.dat file)
*/