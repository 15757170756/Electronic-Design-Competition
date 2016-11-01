/*
* 8分频测试
* */
module Divs(input clk_in,output reg clk_div8);
	//分频数
	parameter N=8;
	reg [8:0]cnt=0;
	always@(posedge clk_in)begin
		if(cnt==N/2-1) begin
			clk_div8 <= !clk_div8;
			cnt<=0;
		end
		else
			cnt <= cnt + 1;
	end
endmodule
/*
* 分频至20Hz
* */
module Divs_20Hz(input clk_100m,output reg clk_20hz);
	//分频数
	parameter N=5000000;
	reg [26:0]cnt;
	always@(posedge clk_100m)begin
		if(cnt==N/2-1) begin
			clk_20hz <= !clk_20hz;
			cnt<=0;
		end
		else
			cnt <= cnt + 1;
	end
endmodule 