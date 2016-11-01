module impulse_measure(input clk_base,
							  input clk_0,input clk_45,input clk_90,input clk_135,input clk_180,
							  input clk_test,input en,input rst_n,
							  output reg[31:0] counter_0,
							  output reg[31:0] counter_45,
							  output reg[31:0] counter_90,
							  output reg[31:0] counter_135,
							  output reg[31:0] counter_180);
endmodule
//等精度测占空比模块
module getImpulse_equal(input clk_en,input clk_base,input clk_test,
					output reg[31:0] base_count_1,output reg[31:0] base_count_2,
					output reg isCount = 0);
	reg[31:0] base_count_1_hide;
	reg[31:0] base_count_2_hide;
	//门控
	always@(posedge clk_test)begin
		if(clk_en)
			isCount <= 1;
		else begin
			isCount <= 0;
		end
	end
	//标准时钟计数
	always@(posedge clk_base)begin
		if(isCount)begin
			if(clk_test)
				base_count_2_hide<= base_count_2_hide+1;
			else
				base_count_1_hide<= base_count_1_hide+1;
		end
		else begin
			base_count_2_hide <= 0;
			base_count_1_hide <= 0;
		end		
	end
	//保存数据
	always@(negedge isCount)begin
		base_count_1  <= base_count_1_hide;
		base_count_2  <= base_count_2_hide;
	end
endmodule 
//低频 测试使用计数方式
module getImpulse_cycle(input clk_base,input clk_test,
					output reg[31:0] base_count,
					output reg isCount = 0);
	reg[31:0] base_count_hide;
	//门控
	always@(posedge clk_test)begin
		if(isCount)
			isCount <= 0;
		else 
			isCount <= 1;
	end
	//标准时钟计数
	always@(posedge clk_base)begin
		if(isCount)begin
			if(clk_test)
				base_count_hide<= base_count_hide+1;
		end
		else 
			base_count_hide <= 0;
		
	end
	//保存数据
	always@(negedge isCount)begin
		base_count  <= base_count_hide;
	end
endmodule
/*
* 根据地址选择寄存器
* module SELECT_REG
READ_EQU_B_1_L 读取高电平时钟低计数使能
READ_EQU_B_1_H 读取高电平时钟高计数使能
READ_EQU_B_2_L 读取低电平时钟低计数使能
READ_EQU_B_2_H 读取低电平时钟高计数使能
READ_EQU_T_L 读取被测时钟低计数使能
READ_EQU_T_H 读取被测时钟高计数使能
* */
module SELECT_REG_I(input [2:0] ADDR,input En,
						output reg READ_EQU_B_1_L = 0,
						output reg READ_EQU_B_1_H = 0,
						output reg READ_EQU_B_2_L = 0,
						output reg READ_EQU_B_2_H = 0,
						output reg READ_EQU_T_L = 0,
						output reg READ_EQU_T_H = 0,
						output reg IRQ_REGISTER = 0);
	always @(*)begin
		if(En)
			case(ADDR)
				3'b000:begin
					READ_EQU_B_1_L <= 1;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 0;
				end
				3'b001:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 1;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 0;
				end
				3'b010:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 1;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 0;
				end
				3'b011:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 1;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 0;
				end
				3'b100:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 1;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 0;
				end
				3'b101:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 1;
					IRQ_REGISTER <= 0;
				end
				3'b110:begin
					READ_EQU_B_1_L <= 0;
					READ_EQU_B_1_H <= 0;
					READ_EQU_B_2_L <= 0;
					READ_EQU_B_2_H <= 0;
					READ_EQU_T_L <= 0;
					READ_EQU_T_H <= 0;
					IRQ_REGISTER <= 1;
				end
			endcase
		else begin
			READ_EQU_B_1_L <= 0;
			READ_EQU_B_1_H <= 0;
			READ_EQU_B_2_L <= 0;
			READ_EQU_B_2_H <= 0;
			READ_EQU_T_L <= 0;
			READ_EQU_T_H <= 0;
			IRQ_REGISTER <= 0;
		end
	end
endmodule 