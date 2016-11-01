//
module getFreq(input clk_en,input clk_base,input clk_test,
					output reg[31:0] base_count,output reg[31:0] test_count,
					output reg isCount = 0);
	
endmodule 
//低频 测试使用计数方式
module getFreq_cycle(input clk_base,input clk_test,
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
//等精度测频模块
module getFreq_equal(input clk_en,input clk_base,input clk_test,
					output reg[31:0] base_count,output reg[31:0] test_count,output reg isCount = 0);
	reg[31:0] base_count_hide;
	reg[31:0] test_count_hide;
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
			base_count_hide<= base_count_hide+1;
		end
		else 
			base_count_hide <= 0;
		
	end
	//测试时钟计数
	always@(posedge clk_test)begin
		if(isCount)begin
			test_count_hide<= test_count_hide+1;
		end
		else 
			test_count_hide = 0;
		
	end
	//保存数据
	always@(negedge isCount)begin
		test_count  <= test_count_hide;
		base_count  <= base_count_hide;
	end
endmodule 
/*
* 根据地址选择寄存器
* module SELECT_REG
READ_EQU_B_L 读取基准时钟低计数使能
READ_EQU_B_H 读取基准时钟高计数使能
READ_EQU_T_L 读取被测时钟低计数使能
READ_EQU_T_H 读取被测时钟高计数使能
READ_EQU_T_L 读取基准时钟2低计数使能
READ_EQU_T_H 读取基准时钟2高计数使能
IRQ_REGISTER 读取寄存器使能
* */
module SELECT_REG(input [3:0] ADDR,input En,
						output reg READ_EQU_BH_L = 0,
						output reg READ_EQU_BH_H = 0,
						output reg READ_EQU_TH_L = 0,
						output reg READ_EQU_TH_H = 0,
						output reg READ_EQU_BL_L = 0,
						output reg READ_EQU_BL_H = 0,
						output reg READ_EQU_TL_L = 0,
						output reg READ_EQU_TL_H = 0,
						output reg READ_EQU_B2_L = 0,
						output reg READ_EQU_B2_H = 0,
						output reg IRQ_REGISTER_1 = 0,
						output reg IRQ_REGISTER_2 = 0);
	always @(*)begin
		if(En)
			case(ADDR)
				4'b0000:begin
					READ_EQU_BH_L <= 1;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0001:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 1;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0010:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 1;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0011:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 1;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0100:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 1;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0101:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 1;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0110:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 1;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b0111:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 1;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b1000:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 1;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b1001:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 1;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
				end
				4'b1010:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 1;
					IRQ_REGISTER_2 <= 0;
				end
				4'b1011:begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 1;
				end
			endcase
		else begin
					READ_EQU_BH_L <= 0;
					READ_EQU_BH_H <= 0;
					READ_EQU_TH_L <= 0;
					READ_EQU_TH_H <= 0;
					
					READ_EQU_BL_L <= 0;
					READ_EQU_BL_H <= 0;
					READ_EQU_TL_L <= 0;
					READ_EQU_TL_H <= 0;
					
					READ_EQU_B2_L <= 0;
					READ_EQU_B2_H <= 0;
					IRQ_REGISTER_1 <= 0;
					IRQ_REGISTER_2 <= 0;
		end
	end
endmodule

//清除中断源
module INTSERVICE(input en,
						input [15:0] AD_MIX,
						input WRITE, 
						input INT_1,INT_2,
						input clk_100M,
						output reg [15:0] DATA_OUT=0,
						output reg INT_OUT = 1);
	reg [1:0] INT_1_save = 2'b0;
	reg [1:0] INT_2_save = 2'b0;
	reg [1:0] WRITE_save = 2'b0;
	wire INT_1_n,INT_2_n,WRITE_p;
	assign INT_1_n = INT_1_save[1]&(~INT_1_save[0]);
	assign INT_2_n = INT_2_save[1]&(~INT_2_save[0]);
	assign INT_1_p = INT_1_save[0]&(~INT_1_save[1]);
	assign INT_2_p = INT_2_save[0]&(~INT_2_save[1]);
	assign WRITE_p = (~WRITE_save[1])&(~WRITE_save[0]);
	//时钟同步保存
	always@(posedge clk_100M)begin
		INT_1_save <= {INT_1_save[0],INT_1};
		INT_2_save <= {INT_2_save[0],INT_2};
		WRITE_save <= {WRITE_save[0],WRITE};
		end
	
	always@(WRITE_p or 
			  INT_1_n or INT_2_n or
			  INT_1_p or INT_2_p)begin
		if(INT_1_n)begin
			DATA_OUT [0] <= 1;
			DATA_OUT [2] <= 0;
		end
		else if(INT_1_p)begin
			DATA_OUT [0] <= 0;
			DATA_OUT [2] <= 1;
		end
		if(INT_2_n)begin
			DATA_OUT [1] <= 1;
			DATA_OUT [3] <= 0;
			INT_OUT <= 0;
		end
		else if(INT_2_p)begin
			DATA_OUT [3] <= 1;
			
		end
		else if(WRITE_p)
			if(en)begin
				DATA_OUT <= DATA_OUT&(~AD_MIX);
				INT_OUT <= 1;
			end
	end	
endmodule 