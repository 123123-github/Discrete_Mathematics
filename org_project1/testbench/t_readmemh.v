/*
    date:   2018/06/01
    tag:    test readmemh
*/
module t_readmemh();
    reg[7:0] mem[1024:0];

    integer i;

    initial
    begin
        $readmemh("D:\\Exp\\Project1\\datapath\\instruction.dat", mem);
        for (i=0; i<68; i=i+1)
            $display("Mem[%d] = %h", i, mem[i]);
    end
      
endmodule