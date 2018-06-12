module mux2( sl, a, b, y);
    parameter WIDTH = 32;

    input               sl;         // select
    input[WIDTH-1:0]    a,b;

    output[WIDTH-1:0]   y;          // result
    
//    reg[WIDTH-1:0]      y;
    // always @(sl or a or b)
    //     if ( !sl )
    //         y = a;
    //     else
    //         y = b;

    assign y = sl? b: a;

endmodule

/*
    0 ---> a
    1 ---> b
*/