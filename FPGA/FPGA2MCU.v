/*
* 保存MCU地址
* module SAVE_ADDR
* */
module SAVE_ADDR(input NADV,input [15:0] AD_IN,
					  input A16,input A17,input A18,
					  output reg[18:0] ADDR);
	always@(posedge NADV)begin
		ADDR <= {A18,A17,A16,AD_IN};
		end
endmodule
/*
* 根据地址选择器件
* module SELECT_ADDR
* */
module SELECT_ADDR(input [18:0] ADDR,
						 output reg [2:0]ch,
						 output reg [3:0]REG_FLAG);
	always @(*)begin
		case(ADDR[18:7])
			{4'b1010,8'b0}:begin
				ch <= ADDR[6:4];
				REG_FLAG <= ADDR[3:0];
			end
			default:	begin
				ch <= 0;
			end
		endcase
	end
endmodule
/*
* 根据地址选择通道
* module SELECT_CHANNEL
* */
module SELECT_CHANNEL(
						 input [2:0]CH,
						 output reg CH_1=0,
						 output reg CH_2=0,
						 output reg CH_3=0);
	always @(*)begin
		case(CH)
			3'b001:begin
				CH_1 <=1;
				CH_2 <=0;
				CH_3 <=0;
			end
			3'b010:begin
				CH_1 <=0;
				CH_2 <=1;
				CH_3 <=0;
			end
			3'b011:begin
				CH_1 <=0;
				CH_2 <=0;
				CH_3 <=1;
			end
			default:begin
				CH_1 <=0;
				CH_2 <=0;
				CH_3 <=0;
			end
		endcase
	end
endmodule
/*
* 缓冲器发送数据
* module BUFF_SEND_DATA
* */
module BUFF_SEND_DATA(input[15:0] DATA_IN,input READ,input en,
					  output reg[15:0] DATA_OUT,output reg FINISH);
	wire READ_2 = READ;
	always@(negedge READ_2 or posedge READ)begin
			if(READ_2 == 0)
				if(en)begin
					DATA_OUT <= DATA_IN;
					end
			if(READ == 1)begin
				DATA_OUT <= 16'bz;
				FINISH <= !FINISH;
				end
		end
endmodule
/*
* 缓冲器
* module BUFF
* */
module BUFF(input en,input write,input [15:0] DATA_IN,
				output reg[15:0] DATA_OUT,output reg FINISH);
	always@(posedge write)begin
		if(en == 1)
			DATA_OUT <= DATA_IN;
			FINISH <= !FINISH;
			//flag <= !flag;
		end
endmodule 
//高速数据同步器
module HighSpeedSync(input CLK_EN,input int_clk,input[31:0] DATA_IN,
							output reg[31:0] DATA_OUT);
	always@(posedge CLK_EN)begin
		if(~int_clk)begin
			DATA_OUT = DATA_IN;
		end
	end
endmodule 
//8位计数器
module counter_8(input int_clk,output reg [7:0]DATA_OUT);
	always@(posedge int_clk)begin
		DATA_OUT = DATA_OUT+1;
	end
endmodule 