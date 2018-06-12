|ALUctr             |Function   |  
|-                  |:-:          |
|0000               |AND        |
|0001               |OR         |
|0010               |add        |
|0110               |sub        |
|0111               |slt        |
|1100               |NOR        |

|Instruction opcode |ALUOp|Instruction operation|Funct field|Desired ALU action|ALU control input|
|-|:-:|:-:|-|-|-|-|
|lw|00|load word|XXXXXX|add|0010|
|sw|00|store word|XXXXXX|add|0010|
|Branch equal|01|branch equal|XXXXXX|subtract|0110|
|R-type|10 |add |100000| add| 0010|
|R-type |10 |subtract| 100010| subtract| 0110|
|R-type |10 |AND |100100| and| 0000|
|R-type |10| OR |100101| or| 0001|
|R-type| 10 |slt| 101010 |set on less than| 0111|

|ALUOp| Funct field|
Operation| ALUOp1 ALUOp0 F5 F4 F3 F2 F1 F0
0 0 X X X X X X 0010
X 1 X X X X X X 0110
1 X X X 0 0 0 0 0010
1 X X X 0 0 1 0 0110
1 X X X 0 1 0 0 0000
1 X X X 0 1 0 1 0001
1 X X X 1 0 1 0 0111