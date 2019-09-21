# DL167
 4bit CPU DL167 is an address extension of DL166.

#Registers
0:0000 ACC
1:0001 GR1
2:0010 GR2
3:0011 GR3
4:0100 GR4
5:0101 INPUT PORT(read only)
6:0110 OUTPUT PORT(write only)
7:0111 Program Counter(PC low nibble)
8:1000 Program Counter(PC hi nibble)
#Instruction Set
01-000-sss ADD (sss) acc=acc+(sss)
01-001-sss OR (sss) acc=acc | (sss)
01-010-sss AND (sss) acc = acc & (sss)
01-011-sss XOR (sss) acc = acc ^ (sss)

01-100-sss INC (sss) (sss)=(sss)+1
01-101-sss NOT (sss) (sss)=!(sss)
01-110-sss RSHIFT (sss)(sss)=(sss)>>1
01-111-sss LSHIFT (sss)(sss)=(sss)<<1

10-00-#### JNC #### if (C==0) PC=#### else PC=PC+1
10-01-#### JMP #### PC_LOW = ####,PC_HI = R4
10-10-#### SET #### acc=####

00-ddd-sss MOV (ddd),(sss)

#Files
DL167.v
CPU and Memory Verilog-HDL Source files

testbench3.v
Test bench for DL167

asm.txt
sample code to test DL167.
// this program is counting up R0 register and show this on LEDs.
00: mov R0,f
01: mov R4,R0
02: jmp 0

f0:inc r0
f1:mov r6,r0
f2:jmp 0
