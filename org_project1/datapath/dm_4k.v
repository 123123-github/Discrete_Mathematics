module dm_4k( addr, din, we, clk, dout);
    input[11:2]     addr;       // address bus
    input[31:0]     din;        // 32-bit input data
    input           we;         // memory write enable
    input           clk;        // clock

    output[31:0]    dout;       // 32-bit memory output

    reg[31:0]       dm[1023:0]; // 1024 32-bit data memory
    
    assign dout = dm[ addr[11:2]];
    
    always @(posedge clk)
    begin
        if (we)
            dm[ addr[11:2]] <= din;
    end



    // initial by 1-10
    initial
    begin
        //$readmemh("D:\\Exp\\Project1\\code.txt", im);
        $readmemh("D:\\Exp\\Project1\\dm.txt", dm);
    end

    


endmodule


/*
    dout is the corresponding addr data (last clock posedge)
    
*/