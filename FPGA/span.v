module span();
endmodule 
//等精度测时间间隔模块
module getSpan_equal(input clk_en,input clk_base,
								input clk_test_1,input clk_test_2,
								output reg[31:0] count_1,output reg[31:0] count_2,
								output reg isCount = 0,output reg pin_r);
	reg[31:0] count_1_hide;
	reg[31:0] count_2_hide;
	//门控
	always@(posedge clk_test_1)begin
		if(clk_en)
			isCount <= 1;
		else begin
			isCount <= 0;
		end
	end
	//计数1 clk_1与clk_2相同
	always@(posedge clk_base)begin
		if(isCount)begin
			if(clk_test_2 == clk_test_1)
				count_1_hide<= count_1_hide+1;
			end
		else begin
			count_1_hide <= 0;
		end		
	end
	//计数2 clk_1与clk_2不同
	always@(posedge clk_base)begin
		if(isCount)begin
			if(clk_test_2 != clk_test_1)
				count_2_hide<= count_2_hide+1;
			end
		else begin
			count_2_hide <= 0;
		end
	end
	//保存数据
	always@(negedge isCount)begin
		count_1  <= count_1_hide;
		count_2  <= count_2_hide;
	end
	//测量超前
	always@(posedge clk_test_1)begin
		if(clk_test_2)
			pin_r = 1;
		else
			pin_r = 0;
		end
endmodule 