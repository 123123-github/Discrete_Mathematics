module sigext ( a, y);
    input[15:0]     a;          // come from instruction[15:0]        

    output[31:0]    y;
	
	assign y = { { 16{a[15]}}, a};
	
endmodule

/*
module sigext ( a, y);
    parameter WIDTH= 16;

    input[WIDTH-1:0]    a;          // come from instruction[15:0]        

    output[31:0]        y;
	
	assign y = { { (32-WIDTH){a[WIDTH-1]} }, a};    // (a[WIDTH-1]){ 32-WIDTH }a 
	
endmodule
*/
