module DL167(reset,clk,btn,led);
	input reset;
	input clk;
	input[3:0] btn;
	output[3:0]led;
	
	reg[24:0] counter;
	cpu cpu0(reset,counter[22],btn,led);
	always @(posedge clk) counter = counter + 1;
endmodule


module cpu(reset,clk,btn,led);
	input reset;
	input clk;
	input[3:0] btn;
	output[3:0]led;
	
	wire[3:0] imd;
	wire[2:0] op1,ddd,sss;
	wire[1:0] op0,op2;
	wire[7:0] dout;
	
	reg 		c_flag=1'b0;
	reg [4:0]regs[8:0];
	
//	assign regs[5]=btn;
	assign led= regs[6];
	
	rom memory(dout, {regs[8],regs[7]});
	
	assign op0=dout[7:6];//[1:0]
	assign op1=dout[5:3];//[2:0]
	assign op2=dout[5:4];//[1:0]
	assign ddd=dout[5:3];//[2:0]
	assign sss=dout[2:0];//[2:0]
	assign imd=dout[3:0];//[3:0]

	initial begin
		regs[0]=0;
		regs[1]=0;
		regs[2]=0;
		regs[3]=0;
		regs[4]=0;
		regs[7]=0;
		regs[8]=0;
	end
	always @(posedge clk) begin
		regs[5]=btn;
		case(op0)
//			4'b0000:begin a=a+im;c_flag=a[4];end	//ADD A,im
			2'b00: regs[ddd]=regs[sss];
			2'b01: 
				case(op1)
					3'b000:begin regs[0]=regs[0]+regs[sss];c_flag=regs[4][0];end//ADD take care c_flag
					3'b001:regs[0]=regs[0]|regs[sss];
					3'b010:regs[0]=regs[0]&regs[sss];
					3'b011:regs[0]=regs[0]^regs[sss];
					3'b100:begin regs[sss]=regs[sss]+1;c_flag=regs[4][sss]; end //INC take care c_flag
					3'b101:regs[sss]=!regs[sss];
					3'b110:begin c_flag=regs[0][sss];regs[sss]=regs[sss]>>1;end	//RSHIFT take care c_flag
					3'b111:begin c_flag=regs[3][sss];regs[sss]=regs[sss]<<1;end	//LSHIFT take care c_flag
				endcase
			2'b10:
				case(op2)
					2'b00:{regs[8],regs[7]}= (c_flag)?{regs[8],regs[7]}+1:{regs[4],imd};	//JNC
					2'b01:{regs[8],regs[7]}={regs[4],imd};									//JMP
					2'b10:regs[0]=imd;					
				endcase
		endcase
		
		if(dout[7:4]!=4'b1000 && dout[7:4]!=4'b1001) {regs[8],regs[7]}={regs[8],regs[7]}+1;
		if(reset==0) {regs[8],regs[7]} = 0;

//	regs[7]=regs[7]+1;
	end
endmodule 

module rom(dout, addr);
	output [7:0] dout;
	input [7:0] addr;
	reg[7:0] mem[256:0];
	initial begin
		`include "asm.txt"
	end
	assign dout = mem[addr];
endmodule