# ins   op                      shamt       func
add     000000    rs  rt  rd    00000       100000  
sub     000000    rs  rt  rd    00000       100010  
and     000000    rs  rt  rd    00000       100100  
or      000000    rs  rt  rd    00000       100101  
slt     000000    rs  rt  rd    00000       101010

lw      100011    rs  rt            offset_16
sw      101011    rs  rt            offset_16       
beq     000100    rs  rt            offset_16

j       000010              target_26



# R-type
add         rd      rs      rt;
# I-type
lw          rt      rs      imm16;
# J-type
j           target



# Src   rs & rt
# Dst   rd | rt